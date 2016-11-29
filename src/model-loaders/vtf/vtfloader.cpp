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

        void getTexturesFromVmtRecursive(const VTFLib::Nodes::CVMTNode* node,
                                QHash<Renderer::ShaderDefs::TextureUnit, QString>& textureTable)
        {
            switch ( node->GetType() )
            {
                case NODE_TYPE_GROUP:
                {
                    // Recursively explore this group.
                    const VTFLib::Nodes::CVMTGroupNode* groupNode = dynamic_cast<const VTFLib::Nodes::CVMTGroupNode*>(node);
                    for ( quint32 i = 0; i < groupNode->GetNodeCount(); i++ )
                    {
                        getTexturesFromVmtRecursive(groupNode->GetNode(i), textureTable);
                    }

                    return;
                }

                case NODE_TYPE_STRING:
                {
                    const VTFLib::Nodes::CVMTStringNode* stringNode = dynamic_cast<const VTFLib::Nodes::CVMTStringNode*>(node);
                    const VmtPropertyTable& propTable = getVmtPropertyTable();

                    QString nodeName = QString(stringNode->GetName()).toLower();
                    if ( !propTable.contains(nodeName) )
                        return;

                    QString texturePath = CalliperUtil::General::normaliseResourcePathSeparators(stringNode->GetValue());
                    textureTable.insert(propTable.value(nodeName), texturePath);
                    return;
                }

                // For now, this is all we support.
                default:
                {
                    return;
                }
            }
        }

        void getTexturesFromVmtRecursive(const QString& key, const QJsonValue value,
                                QHash<Renderer::ShaderDefs::TextureUnit, QString>& textureTable)
        {
            if ( value.isString() )
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
                    getTexturesFromVmtRecursive(key, it.value(), textureTable);
                }

                return;
            }

            // We don't care about anything else right now.
            return;
        }
    }

    namespace VTFLoader
    {
        // TODO: Clean up this function, it's sprawling.
        void loadMaterials(const FileFormats::VPKFileCollection &vpkFiles,
                           Model::MaterialStore *materialStore,
                           Model::TextureStore *textureStore)
        {
            using namespace FileFormats;
            using namespace Renderer;

            QHash<QString, quint32> referencedVtfs;
            QSet<VPKFilePointer> vmtVpks = vpkFiles.filesContainingExtension("vmt");

            // Read all the VMTs and keep track of all the VTFs they reference.
            foreach ( const VPKFilePointer& file, vmtVpks )
            {
                // These are ordered by archive number.
                QList<VPKIndexTreeRecordPointer> records = file->index().recordsForExtension("vmt");
                quint16 currentArchiveIndex = ~0;

                foreach ( const VPKIndexTreeRecordPointer& record, records )
                {
                    if ( record->item()->archiveIndex() != currentArchiveIndex )
                    {
                        currentArchiveIndex = record->item()->archiveIndex();
                        file->openArchive(currentArchiveIndex);
                    }

                    QByteArray vmtData = file->readFromCurrentArchive(record->item());
                    if ( vmtData.isEmpty() )
                        continue;

                    qDebug() << "Reading VMT file:" << record->fullPath();

                    /*
                    VTFLib::CVMTFile vmtFile;
                    if ( !vmtFile.Load(vmtData.constData(), vmtData.length()) )
                        continue;

                    typedef QHash<ShaderDefs::TextureUnit, QString> TextureUnitToPathTable;
                    TextureUnitToPathTable textureTable;
                    getTexturesFromVmtRecursive(vmtFile.GetRoot(), textureTable);
                    */

                    // TODO: Use our own KV parser here.

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
                }

                file->closeArchive();
            }


            // Now we have created all materials and hooked them up with their appropriate texture IDs.
            // Next, go through each VMT that was referenced and load it.
            QSet<VPKFilePointer> vtfVpks = vpkFiles.filesContainingExtension("vtf");
            foreach ( const VPKFilePointer& file, vtfVpks )
            {
                QList<VPKIndexTreeRecordPointer> records = file->index().recordsForExtension("vtf");
                quint16 currentArchiveIndex = ~0;

                foreach ( const VPKIndexTreeRecordPointer& record, records )
                {
                    if ( !referencedVtfs.contains(record->fullPath()) )
                        continue;

                    quint32 textureId = referencedVtfs.value(record->fullPath());

                    if ( record->item()->archiveIndex() != currentArchiveIndex )
                    {
                        currentArchiveIndex = record->item()->archiveIndex();
                        file->openArchive(currentArchiveIndex);
                    }

                    QByteArray vtfData = file->readFromCurrentArchive(record->item());
                    if ( vtfData.isEmpty() )
                    {
                        textureStore->destroyTexture(referencedVtfs.value(record->fullPath()));
                        continue;
                    }

                    QBuffer vtfDataBuffer(&vtfData);
                    QImageReader imageReader(&vtfDataBuffer, "vtf");
                    QImage image = imageReader.read();
                    if ( image.isNull() )
                    {
                        qDebug() << "Failed to read" << record->fullPath() << "into a QImage";
                        textureStore->destroyTexture(textureId);
                        continue;
                    }

                    OpenGLTexturePointer texture = textureStore->getTexture(textureId);
                    if ( texture->textureStoreId() != textureId )
                    {
                        Q_ASSERT_X(false, Q_FUNC_INFO, "Texture ID mismatch, should never happen!");
                        continue;
                    }

                    texture->setData(image);
                }

                file->closeArchive();
            }
        }
    }
}
