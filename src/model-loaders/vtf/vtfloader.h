#ifndef VTFLOADER_H
#define VTFLOADER_H

#include "model-loaders_global.h"
#include "vpk/vpkfilecollection.h"
#include "stores/materialstore.h"
#include "stores/texturestore.h"
#include <QSet>
#include <QHash>
#include <QString>

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT VTFLoader
    {
    public:
        VTFLoader(Model::MaterialStore* materialStore, Model::TextureStore* textureStore);

        void loadMaterials(const FileFormats::VPKFileCollection& vpkFiles);

    private:
        void findReferencedVtfs();
        void loadReferencedVtfs();
        QByteArray getData(const FileFormats::VPKFilePointer& vpk, const FileFormats::VPKIndexTreeRecordPointer& record);
        void populateMaterial(Renderer::RenderMaterialPointer& material, const QJsonDocument& vmt);

        Model::MaterialStore* m_pMaterialStore;
        Model::TextureStore* m_pTextureStore;

        QSet<FileFormats::VPKFilePointer> m_VmtFileSet;
        QSet<FileFormats::VPKFilePointer> m_VtfFileSet;
        QHash<QString, quint32> m_ReferencedVtfs;

        QList<FileFormats::VPKIndexTreeRecordPointer> m_CurrentRecordSet;
        quint16 m_iCurrentArchiveIndex;
    };
}

#endif // VTFLOADER_H
