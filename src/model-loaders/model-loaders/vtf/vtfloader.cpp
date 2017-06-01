#include "vtfloader.h"

#include <QImageReader>
#include <QBuffer>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QOpenGLPixelTransferOptions>

#include "VTFLib/src/VMTFile.h"
#include "VTFLib/src/VMTNode.h"
#include "VTFLib/src/VTFFile.h"
#include "VTFLib/src/VTFLib.h"

#include "calliperutil/general/generalutil.h"

#include "file-formats/vpk/vpkindex.h"
#include "file-formats/keyvalues/keyvaluesparser.h"

#include "rendersystem/interface-classes/texture/namedopengltexture.h"
#include "rendersystem/endpoints/materialstoreendpoint.h"
#include "rendersystem/endpoints/texturestoreendpoint.h"
#include "rendersystem/interface-classes/definitions/materialdefs.h"

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
            QString matPath = (record->path() + "/" + record->fileName()).toLower();
            int removalIndex = matPath.indexOf("materials/");
            if ( removalIndex >= 0 )
            {
                matPath = matPath.mid(removalIndex + static_cast<int>(strlen("materials/")));
            }
            return matPath;
        }

        bool setTextureFormat(RenderSystem::NamedOpenGLTexture& texture, VTFImageFormat format)
        {
            switch (format)
            {
                case IMAGE_FORMAT_DXT1:
                texture.setFormat(QOpenGLTexture::RGBA_DXT1);
                break;

                case IMAGE_FORMAT_DXT3:
                texture.setFormat(QOpenGLTexture::RGBA_DXT3);
                break;

                case IMAGE_FORMAT_DXT5:
                texture.setFormat(QOpenGLTexture::RGBA_DXT5);
                break;

            default:
                return false;
            }

            return true;
        }

        bool loadVtf(const QByteArray& vtfData, RenderSystem::NamedOpenGLTexture& texture)
        {
            VTFLib::CVTFFile vtfFile;
            if ( !vtfFile.Load(static_cast<const vlVoid*>(vtfData.constData()), vtfData.length(), false) )
            {
                return false;
            }

            if ( !setTextureFormat(texture, vtfFile.GetFormat()) )
            {
                qWarning() << "Currently unsupported format" << VTFLib::CVTFFile::ImageFormatName(vtfFile.GetFormat())
                           << "for texture" << texture.path();
                return false;
            }

            if ( !vtfFile.GetData(0,0,0,0) )
            {
                qWarning() << "VTF for texture" << texture.path()
                           << "has no data.";
                return false;
            }

            texture.setSize(vtfFile.GetWidth(), vtfFile.GetHeight());
            texture.allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32);

            if ( vtfFile.GetFormat() == IMAGE_FORMAT_RGBA8888 )
            {
                texture.setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32, vtfFile.GetData(0, 0, 0, 0));
            }
            else
            {
                int size = static_cast<int>(VTFLib::CVTFFile::ComputeImageSize(vtfFile.GetWidth(), vtfFile.GetHeight(),
                                                              1,1,
                                                              vtfFile.GetFormat()));
                texture.setCompressedData(size, vtfFile.GetData(0,0,0,0));
            }

            texture.create();

            return true;
        }
    }

    VTFLoader::VTFLoader()
        : m_VmtFileSet(),
          m_VtfFileSet(),
          m_ReferencedVtfs(),
          m_CurrentRecordSet(),
          m_iCurrentArchiveIndex(~0)
    {
    }

    void VTFLoader::loadMaterials(const FileFormats::VPKFileCollection &vpkFiles)
    {
        using namespace RenderSystem;

        TextureStoreEndpoint::TextureStoreAccessor textureStore = TextureStoreEndpoint::textureStore();

        m_VmtFileSet = vpkFiles.filesContainingExtension("vmt");
        m_VtfFileSet = vpkFiles.filesContainingExtension("vtf");

        if ( m_VmtFileSet.isEmpty() || m_VtfFileSet.isEmpty() )
        {
            return;
        }

        findReferencedVtfs();
        loadReferencedVtfs();

        // Clean up any remaining VTFs - the files could have referenced some that don't actually exist.
        foreach ( TextureDefs::TextureId textureId, m_ReferencedVtfs.values() )
        {
            qDebug() << "Cleaning up unused texture" << textureId << textureStore->texture(textureId).toStrongRef()->path();
            textureStore->removeTexture(textureId);
        }

        m_ReferencedVtfs.clear();
    }

    void VTFLoader::findReferencedVtfs()
    {
        using namespace RenderSystem;

        MaterialStoreEndpoint::MaterialStoreAccessor materialStore = MaterialStoreEndpoint::materialStore();

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

                MaterialDefs::MaterialId materialId = materialStore->createMaterial(matPath);
                QSharedPointer<RenderMaterial> material = materialStore->material(materialId).toStrongRef();
                populateMaterial(material, doc);
            }

            vpk->closeArchive();
        }
    }

    void VTFLoader::loadReferencedVtfs()
    {
        using namespace RenderSystem;

        TextureStoreEndpoint::TextureStoreAccessor textureStore = TextureStoreEndpoint::textureStore();

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

                TextureDefs::TextureId textureId = m_ReferencedVtfs.value(fullPath);

                QByteArray vtfData = getData(vpk, record);
                if ( vtfData.isEmpty() )
                {
                    qDebug() << "VTF data is empty";
                    m_ReferencedVtfs.remove(fullPath);
                    textureStore->removeTexture(textureId);
                    continue;
                }

                QSharedPointer<NamedOpenGLTexture> texture = textureStore->texture(textureId).toStrongRef();

                if ( !loadVtf(vtfData, *texture) )
                {
                    qDebug().nospace() << "Failed to read " << fullPath << ": " << VTFLib::LastError.Get();
                    textureStore->removeTexture(textureId);
                    m_ReferencedVtfs.remove(fullPath);
                    continue;
                }

                m_ReferencedVtfs.remove(fullPath);
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

    void VTFLoader::populateMaterial(const QSharedPointer<RenderSystem::RenderMaterial> &material, const QJsonDocument &vmt)
    {
        using namespace RenderSystem;

        TextureStoreEndpoint::TextureStoreAccessor textureStore = TextureStoreEndpoint::textureStore();

        if ( !vmt.isObject() )
        {
            return;
        }

        QJsonObject root = vmt.object();
        QJsonObject::const_iterator rootKey = root.constBegin();
        if ( rootKey == root.constEnd() )
        {
            return;
        }

        QString shaderType = rootKey.key();

        if ( !noCaseCompare(shaderType, "UnlitGeneric") &&
             !noCaseCompare(shaderType, "LightmappedGeneric") &&
             !noCaseCompare(shaderType, "WorldVertexTransition"))
        {
            return;
        }

        QJsonValue vProperties = rootKey.value();
        if ( !vProperties.isObject() )
        {
            return;
        }

        // Annoyingly we can't just index by key because of non-case-sensitivity.
        QJsonObject properties = vProperties.toObject();
        for ( QJsonObject::const_iterator it = properties.constBegin(); it != properties.constEnd(); ++it )
        {
            if ( !noCaseCompare(it.key(), "$basetexture") )
            {
                continue;
            }

            QJsonValue val = it.value();
            if ( !val.isString() )
            {
                continue;
            }

            QString vtfPath = CalliperUtil::General::normaliseResourcePathSeparators(val.toString().toLower());

            if ( !m_ReferencedVtfs.contains(vtfPath) )
            {
                TextureDefs::TextureId textureId = textureStore->createBlankTexture(vtfPath);
                m_ReferencedVtfs.insert(vtfPath, textureId);
            }

            material->addTextureUnitMapping(TextureDefs::MainTexture, vtfPath);
            return;
        }
    }
}
