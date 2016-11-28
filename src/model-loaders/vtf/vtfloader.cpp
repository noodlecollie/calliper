#include "vtfloader.h"
#include "vpk/vpkindex.h"
#include "VTFLib/src/VMTFile.h"
#include "materials/rendermaterial.h"
#include "VTFLib/src/VMTNode.h"
#include <QtDebug>

namespace ModelLoaders
{
    namespace
    {
        void getTexturesFromVmtRecursive(const VTFLib::Nodes::CVMTNode* node,
                                QHash<Renderer::ShaderDefs::TextureUnit, QString>& textureTable)
        {
            qDebug() << "Node name:" << node->GetName();

            switch ( node->GetType() )
            {
                case NODE_TYPE_GROUP:
                {
                    const VTFLib::Nodes::CVMTGroupNode* groupNode = dynamic_cast<const VTFLib::Nodes::CVMTGroupNode*>(node);

                    for ( quint32 i = 0; i < groupNode->GetNodeCount(); i++ )
                    {
                        getTexturesFromVmtRecursive(groupNode->GetNode(i), textureTable);
                    }

                    return;
                }

                default:
                {
                    return;
                }
            }
        }
    }

    namespace VTFLoader
    {
        void loadMaterials(const FileFormats::VPKFileCollection &vpkFiles,
                           Model::MaterialStore *materialStore,
                           Model::TextureStore *textureStore)
        {
            using namespace FileFormats;
            using namespace Renderer;

            QSet<QString> referencedVtfs;
            QSet<VPKFilePointer> vmtVpks = vpkFiles.filesContainingExtension("vmt");

            foreach ( const VPKFilePointer& file, vmtVpks )
            {
                // These are ordered by archive number.
                QList<VPKIndexTreeRecordPointer> records = file->index().recordsForExtension("vmt");
                quint16 currentArchiveIndex = ~0;

                foreach ( const VPKIndexTreeRecordPointer record, records )
                {
                    if ( record->item()->archiveIndex() != currentArchiveIndex )
                    {
                        currentArchiveIndex = record->item()->archiveIndex();
                        file->openArchive(currentArchiveIndex);
                    }

                    QByteArray vmtData = file->readFromCurrentArchive(record->item());
                    if ( vmtData.isEmpty() )
                        continue;

                    VTFLib::CVMTFile vmtFile;
                    if ( !vmtFile.Load(vmtData.constData(), vmtData.length()) )
                        continue;

                    QHash<ShaderDefs::TextureUnit, QString> textureTable;
                    getTexturesFromVmtRecursive(vmtFile.GetRoot(), textureTable);
                }

                file->closeArchive();
            }
        }

        void debugVmtLoading()
        {
            const char* vmtData =
                    "LightmappedGeneric\n"
                    "{\n"
                    "    $basetexture coast\\shingle_01\n"
                    "    $surfaceprop gravel\n"
                    "}\n";

            VTFLib::CVMTFile vmtFile;
            vmtFile.Load(vmtData, strlen(vmtData));
            QHash<Renderer::ShaderDefs::TextureUnit, QString> textureTable;
            getTexturesFromVmtRecursive(vmtFile.GetRoot(), textureTable);
        }
    }
}
