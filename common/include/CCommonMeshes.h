#ifndef CCOMMONMESHES_H
#define CCOMMONMESHES_H

#include "PolySceneMesh.h"

using namespace Polycode;

class COriginMarker : public SceneMesh
{
public:
	COriginMarker();
	virtual ~COriginMarker() {}
};

class CArrowLineMesh : public SceneMesh
{
public:
	CArrowLineMesh();
	virtual ~CArrowLineMesh() {}
};

#endif	// CCOMMONMESHES_H