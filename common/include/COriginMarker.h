#ifndef CORIGINMARKER_H
#define CORIGINMARKER_H

#include "PolySceneMesh.h"

using namespace Polycode;

class COriginMarker : public SceneMesh
{
public:
	COriginMarker();
	virtual ~COriginMarker();

private:
	void constructGeometry();
};

#endif	// CORIGINMARKER_H