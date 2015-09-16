#include "COriginMarker.h"
#include "PolyMesh.h"

COriginMarker::COriginMarker() : SceneMesh(Mesh::LINE_MESH)
{
}

COriginMarker::~COriginMarker()
{
}

void COriginMarker::constructGeometry()
{
	mesh->indexedMesh = false;

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
}