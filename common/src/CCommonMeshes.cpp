#include "CCommonMeshes.h"
#include "Polycode.h"

COriginMarker::COriginMarker() : SceneMesh(Mesh::LINE_MESH)
{
	mesh->indexedMesh = false;
	mesh->useVertexColors = true;
	
	mesh->addVertex(0, 0, 0);
	mesh->addVertex(1, 0, 0);
	mesh->addColor(1, 0, 0, 1);
	mesh->addColor(1, 0, 0, 1);
	
	mesh->addVertex(0, 0, 0);
	mesh->addVertex(0, 1, 0);
	mesh->addColor(0, 1, 0, 1);
	mesh->addColor(0, 1, 0, 1);
	
	mesh->addVertex(0, 0, 0);
	mesh->addVertex(0, 0, 1);
	mesh->addColor(0, 0, 1, 1);
	mesh->addColor(0, 0, 1, 1);
	
	setMaterialByName("UnlitUntextured", Services()->getResourceManager()->getGlobalPool());
	//setLocalBoundingBox(1,1,1);
	//setAnchorPoint(1,1,1);
}

CArrowLineMesh::CArrowLineMesh() : SceneMesh(Mesh::LINE_MESH)
{
	mesh->indexedMesh = true;
	mesh->useVertexColors = true;
	
	mesh->addVertex(0,0.125f,0);
	mesh->addVertex(1.5f,0.125f,0);
	mesh->addVertex(1.5f,0.25f,0);
	mesh->addVertex(2.0f,0,0);
	mesh->addVertex(1.5f,-0.25f,0);
	mesh->addVertex(1.5f,-0.125f,0);
	mesh->addVertex(0,-0.125f,0);
	
	mesh->addColor(1,0,0,1);
	mesh->addColor(1,0,0,1);
	mesh->addColor(1,0,0,1);
	mesh->addColor(1,0,0,1);
	mesh->addColor(1,0,0,1);
	mesh->addColor(1,0,0,1);
	mesh->addColor(1,0,0,1);
	
	for (unsigned int i = 0; i < (unsigned int)mesh->getVertexCount()-1; i++)
	{
		mesh->addIndex(i);
		mesh->addIndex(i+1);
	}
	
	setMaterialByName("UnlitUntextured", Services()->getResourceManager()->getGlobalPool());
	//setLocalBoundingBox(2, 0.5, 0.01);
	//setAnchorPoint(-1, 0, 0);
}