#include "vtfloader.h"
#include "vpk/vpkindex.h"
#include "VTFLib/src/VMTFile.h"
#include "materials/rendermaterial.h"
#include "VTFLib/src/VMTNode.h"
#include <QtDebug>
#include "general/generalutil.h"
#include <QImageReader>
#include <QBuffer>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include "keyvalues/keyvaluesparser.h"

namespace ModelLoaders
{
    namespace
    {
        typedef QHash<QString, Renderer::ShaderDefs::TextureUnit> VmtPropertyTable;

        const VmtPropertyTable& getVmtPropertyTable()
        {
            using namespace Renderer;

            static VmtPropertyTable vmtPropertyToShaderDef;

            if ( vmtPropertyToShaderDef.isEmpty() )
            {
                vmtPropertyToShaderDef.insert("$basetexture", ShaderDefs::MainTexture);
            }

            return vmtPropertyToShaderDef;
        }

        void getTexturesFromVmtRecursive(const QString& key, const QJsonValue value,
                                QHash<Renderer::ShaderDefs::TextureUnit, QString>& textureTable)
        {
            if ( value.isString() && !key.isNull() && !key.isEmpty() )
            {
                const VmtPropertyTable& propTable = getVmtPropertyTable();
                if ( !propTable.contains(key) )
                    return;

                QString texturePath = CalliperUtil::General::normaliseResourcePathSeparators(value.toString());
                textureTable.insert(propTable.value(key), texturePath);
                return;
            }

            if ( value.isObject() )
            {
                QJsonObject object = value.toObject();

                for ( QJsonObject::const_iterator it = object.constBegin();
                      it != object.constEnd();
                      ++it )
                {
                    getTexturesFromVmtRecursive(it.key(), it.value(), textureTable);
                }

                return;
            }

            if ( value.isArray() )
            {
                QJsonArray array = value.toArray();

                for ( QJsonArray::const_iterator it = array.constBegin();
                      it != array.constEnd();
                      ++it )
                {
                    // Arrays can be produced from multiple KV items with the same key,
                    // so essentially all items here have the same key
                    // (ie. the one that was passed in).
                    // TODO: Is this -always- what we want?
                    getTexturesFromVmtRecursive(key, *it, textureTable);
                }

                return;
            }

            // We don't care about anything else right now.
            return;
        }
    }

    /*
    namespace VTFLoader
    {
        // TODO: Clean up this function, it's sprawling.
        void loadMaterials(const FileFormats::VPKFileCollection &vpkFiles,
                           Model::MaterialStore *materialStore,
                           Model::TextureStore *textureStore)
        {
            using namespace FileFormats;
            using namespace Renderer;

            QSet<VPKFilePointer> vmtVpks = vpkFiles.filesContainingExtension("vmt");
            QSet<VPKFilePointer> vtfVpks = vpkFiles.filesContainingExtension("vtf");

            if ( vmtVpks.count() < 1 || vtfVpks.count() < 1 )
            {
                return;
            }

            QHash<QString, quint32> referencedVtfs;

            // Read all the VMTs and keep track of all the VTFs they reference.
            foreach ( const VPKFilePointer& file, vmtVpks )
            {
                // These are ordered by archive number.
                QList<VPKIndexTreeRecordPointer> records = file->index().recordsForExtension("vmt");
                quint16 currentArchiveIndex = ~0;

                quint32 fileNumber = 0;
                foreach ( const VPKIndexTreeRecordPointer& record, records )
                {
                    if ( record->item()->archiveIndex() != currentArchiveIndex )
                    {
                        currentArchiveIndex = record->item()->archiveIndex();
                        file->openArchive(currentArchiveIndex);
                    }

                    QByteArray vmtData = file->readFromCurrentArchive(record->item());
                    if ( vmtData.isEmpty() )
                    {
                        fileNumber++;
                        continue;
                    }

                    KeyValuesParser parser(vmtData);
                    QString error;
                    QJsonDocument doc = parser.toJsonDocument(&error);
                    if ( !error.isEmpty() )
                    {
                        fileNumber++;
                        continue;
                    }

                    typedef QHash<ShaderDefs::TextureUnit, QString> TextureUnitToPathTable;
                    TextureUnitToPathTable textureTable;
                    getTexturesFromVmtRecursive(QString(), doc.object(), textureTable);

                    RenderMaterialPointer material = materialStore->createMaterial(record->fullPath());

                    for ( TextureUnitToPathTable::const_iterator it = textureTable.constBegin();
                          it != textureTable.constEnd();
                          ++it )
                    {
                        QString path = it.value();

                        // This will just return a pre-existing texture if the path is the same.
                        // In this case the ID will be the same as well.
                        quint32 textureId = textureStore->createEmptyTexture(path)->textureStoreId();
                        referencedVtfs.insert(path, textureId);

                        material->addTexture(it.key(), textureId);
                    }

                    fileNumber++;
                }

                file->closeArchive();
            }

            // Now we have created all materials and hooked them up with their appropriate texture IDs.
            // Next, go through each VMT that was referenced and load it.
            foreach ( const VPKFilePointer& file, vtfVpks )
            {
                QList<VPKIndexTreeRecordPointer> records = file->index().recordsForExtension("vtf");
                quint16 currentArchiveIndex = ~0;

                foreach ( const VPKIndexTreeRecordPointer& record, records )
                {
                    if ( !referencedVtfs.contains(record->fullPath()) )
                        continue;

                    QString fullPath = record->fullPath();
                    qDebug() << "Reading VTF file:" << fullPath;

                    quint32 textureId = referencedVtfs.value(fullPath);

                    if ( record->item()->archiveIndex() != currentArchiveIndex )
                    {
                        currentArchiveIndex = record->item()->archiveIndex();
                        file->openArchive(currentArchiveIndex);
                    }

                    QByteArray vtfData = file->readFromCurrentArchive(record->item());
                    if ( vtfData.isEmpty() )
                    {
                        textureStore->destroyTexture(textureId);
                        referencedVtfs.remove(fullPath);
                        continue;
                    }

                    QBuffer vtfDataBuffer(&vtfData);
                    QImageReader imageReader(&vtfDataBuffer, "vtf");
                    QImage image = imageReader.read();
                    if ( image.isNull() )
                    {
                        qDebug() << "Failed to read" << record->fullPath() << "into a QImage";
                        textureStore->destroyTexture(textureId);
                        referencedVtfs.remove(fullPath);
                        continue;
                    }

                    OpenGLTexturePointer texture = textureStore->getTexture(textureId);
                    if ( texture->textureStoreId() != textureId )
                    {
                        Q_ASSERT_X(false, Q_FUNC_INFO, "Texture ID mismatch, should never happen!");
                        continue;
                    }

                    texture->setData(image.mirrored());
                    referencedVtfs.remove(fullPath);
                }

                file->closeArchive();
            }

            // Clean up any remaining VTFs - the files could have referenced some that don't actually exist.
            foreach ( quint32 textureId, referencedVtfs.values() )
            {
                textureStore->destroyTexture(textureId);
            }
        }
    }
    */

    VTFLoader::VTFLoader(Model::MaterialStore *materialStore, Model::TextureStore *textureStore)
        : m_pMaterialStore(materialStore),
          m_pTextureStore(textureStore)
    {
        Q_ASSERT_X(m_pMaterialStore, Q_FUNC_INFO, "Material store cannot be null!");
        Q_ASSERT_X(m_pTextureStore, Q_FUNC_INFO, "Texture store cannot be null!");
    }

    void VTFLoader::loadMaterials(const FileFormats::VPKFileCollection &vpkFiles)
    {
        m_VmtFileSet = vpkFiles.filesContainingExtension("vmt");
        m_VtfFileSet = vpkFiles.filesContainingExtension("vtf");

        if ( m_VmtFileSet.isEmpty() || m_VtfFileSet.isEmpty() )
        {
            return;
        }

        findReferencedVtfs();
    }

    void VTFLoader::findReferencedVtfs()
    {
        m_ReferencedVtfs.clear();

        foreach ( const FileFormats::VPKFilePointer& vpk, m_VmtFileSet )
        {
            // These are ordered by archive number.
            m_CurrentRecordSet = vpk->index().recordsForExtension("vmt");
            m_iCurrentArchiveIndex = ~0;

            foreach ( const FileFormats::VPKIndexTreeRecordPointer& record, m_CurrentRecordSet )
            {
                QByteArray vmtData = getVmtData(vpk, record);
                if ( vmtData.isEmpty() )
                {
                    qDebug() << "VMT data is empty";
                    continue;
                }

                FileFormats::KeyValuesParser parser(vmtData);
                QString error;
                QJsonDocument doc = parser.toJsonDocument(&error);
                if ( doc.isNull() )
                {
                    qDebug() << "Error parsing" << record->fullPath() << "-" << error;
                    continue;
                }
            }
        }
    }

    QByteArray VTFLoader::getVmtData(const FileFormats::VPKFilePointer &vpk, const FileFormats::VPKIndexTreeRecordPointer &record)
    {
        if ( record->item()->archiveIndex() != m_iCurrentArchiveIndex )
        {
            m_iCurrentArchiveIndex = record->item()->archiveIndex();
            vpk->openArchive(m_iCurrentArchiveIndex);
        }

        return vpk->readFromCurrentArchive(record->item());
    }
}
