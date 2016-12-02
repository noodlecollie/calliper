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
        inline bool noCaseCompare(const QString& a, const QString& b)
        {
            return a.compare(b, Qt::CaseInsensitive) == 0;
        }

        QString materialPath(const FileFormats::VPKIndexTreeRecordPointer& record)
        {
            QString matPath = (record->path() + "/" + record->filename()).toLower();
            int removalIndex = matPath.indexOf("materials/");
            if ( removalIndex >= 0 )
            {
                matPath = matPath.mid(removalIndex + static_cast<int>(strlen("materials/")));
            }
            return matPath;
        }
    }

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
        loadReferencedVtfs();

        // Clean up any remaining VTFs - the files could have referenced some that don't actually exist.
        foreach ( quint32 textureId, m_ReferencedVtfs.values() )
        {
            qDebug() << "Cleaning up unused texture" << textureId;
            m_pTextureStore->destroyTexture(textureId);
        }

        m_ReferencedVtfs.clear();
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
                QByteArray vmtData = getData(vpk, record);
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

                QString matPath = materialPath(record);

                Renderer::RenderMaterialPointer material = m_pMaterialStore->createMaterial(matPath);
                populateMaterial(material, doc);
            }

            vpk->closeArchive();
        }
    }

    void VTFLoader::loadReferencedVtfs()
    {
        foreach ( const FileFormats::VPKFilePointer& vpk, m_VtfFileSet )
        {
            // These are ordered by archive number.
            m_CurrentRecordSet = vpk->index().recordsForExtension("vtf");
            m_iCurrentArchiveIndex = ~0;

            foreach ( const FileFormats::VPKIndexTreeRecordPointer& record, m_CurrentRecordSet )
            {
                QString fullPath = materialPath(record);
                if ( !m_ReferencedVtfs.contains(fullPath) )
                {
                    continue;
                }

                quint32 textureId = m_ReferencedVtfs.value(fullPath);

                QByteArray vtfData = getData(vpk, record);
                if ( vtfData.isEmpty() )
                {
                    qDebug() << "VTF data is empty";
                    m_ReferencedVtfs.remove(fullPath);
                    continue;
                }

                QBuffer vtfDataBuffer(&vtfData);
                QImageReader imageReader(&vtfDataBuffer, "vtf");
                QImage image = imageReader.read();

                if ( image.isNull() )
                {
                    qDebug() << "Failed to read" << fullPath << "into a QImage";
                    m_pTextureStore->destroyTexture(textureId);
                    m_ReferencedVtfs.remove(fullPath);
                    continue;
                }

                Renderer::OpenGLTexturePointer texture = m_pTextureStore->getTexture(textureId);
                if ( texture->textureStoreId() != textureId )
                {
                    Q_ASSERT_X(false, Q_FUNC_INFO, "Texture ID mismatch, should never happen!");
                    m_ReferencedVtfs.remove(fullPath);
                    continue;
                }

                texture->setData(image.mirrored());
                texture->create();
                int removed = m_ReferencedVtfs.remove(fullPath);
                qDebug() << "Removing referenced VTF" << fullPath << "returned" << removed;
            }

            vpk->close();
        }
    }

    QByteArray VTFLoader::getData(const FileFormats::VPKFilePointer &vpk, const FileFormats::VPKIndexTreeRecordPointer &record)
    {
        if ( record->item()->archiveIndex() != m_iCurrentArchiveIndex )
        {
            m_iCurrentArchiveIndex = record->item()->archiveIndex();
            vpk->openArchive(m_iCurrentArchiveIndex);
        }

        return vpk->readFromCurrentArchive(record->item());
    }

    void VTFLoader::populateMaterial(Renderer::RenderMaterialPointer &material, const QJsonDocument &vmt)
    {
        if ( !vmt.isObject() )
            return;

        QJsonObject root = vmt.object();
        QJsonObject::const_iterator rootKey = root.constBegin();
        if ( rootKey == root.constEnd() )
            return;

        QString shaderType = rootKey.key();

        if ( !noCaseCompare(shaderType, "UnlitGeneric") &&
             !noCaseCompare(shaderType, "LightmappedGeneric") &&
             !noCaseCompare(shaderType, "WorldVertexTransition"))
        {
            return;
        }

        QJsonValue vProperties = rootKey.value();
        if ( !vProperties.isObject() )
            return;

        // Annoyingly we can't just index by key because of non-case-sensitivity.
        QJsonObject properties = vProperties.toObject();
        for ( QJsonObject::const_iterator it = properties.constBegin(); it != properties.constEnd(); ++it )
        {
            if ( !noCaseCompare(it.key(), "$basetexture") )
                continue;

            QJsonValue val = it.value();
            if ( !val.isString() )
                continue;

            QString vtfPath = CalliperUtil::General::normaliseResourcePathSeparators(val.toString().toLower());

            if ( !m_ReferencedVtfs.contains(vtfPath) )
            {
                quint32 textureId = m_pTextureStore->createEmptyTexture(vtfPath)->textureStoreId();
                m_ReferencedVtfs.insert(vtfPath, textureId);
            }

            material->addTexture(Renderer::ShaderDefs::MainTexture, m_ReferencedVtfs.value(vtfPath, 0));
            return;
        }
    }
}
