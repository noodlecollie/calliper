#ifndef VTFLOADER_H
#define VTFLOADER_H

#include "model-loaders_global.h"

#include <QSet>
#include <QHash>
#include <QString>

#include "file-formats/vpk/vpkfilecollection.h"

#include "rendersystem/interface-classes/rendermaterial/rendermaterial.h"
#include "rendersystem/interface-classes/definitions/texturedefs.h"

namespace ModelLoaders
{
    class MODELLOADERSSHARED_EXPORT VTFLoader
    {
    public:
        VTFLoader();

        void loadMaterials(const FileFormats::VPKFileCollection& vpkFiles);

    private:
        void findReferencedVtfs();
        void loadReferencedVtfs();
        QByteArray getData(const FileFormats::VPKFilePointer& vpk, const FileFormats::VPKIndexTreeRecordPointer& record);
        void populateMaterial(const QSharedPointer<RenderSystem::RenderMaterial>& material, const QJsonDocument& vmt);

        QSet<FileFormats::VPKFilePointer> m_VmtFileSet;
        QSet<FileFormats::VPKFilePointer> m_VtfFileSet;
        QHash<QString, RenderSystem::TextureDefs::TextureId> m_ReferencedVtfs;

        QList<FileFormats::VPKIndexTreeRecordPointer> m_CurrentRecordSet;
        quint16 m_iCurrentArchiveIndex;
    };
}

#endif // VTFLOADER_H
