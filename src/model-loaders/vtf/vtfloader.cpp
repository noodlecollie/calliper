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

                    QString texturePath = stringNode->GetValue();
                    texturePath.replace('\\', '/');
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
    }
}
