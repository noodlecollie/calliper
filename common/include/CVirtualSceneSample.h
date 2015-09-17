#ifndef CVIRTUALSCENESAMPLE_H
#define CVIRTUALSCENESAMPLE_H

namespace Polycode
{
	class Scene;
	class SceneRenderTexture;
	class SceneLight;
	class ScenePrimitive;
}

class COriginMarker;
class CArrowLineMesh;

using namespace Polycode;

class CVirtualSceneSample
{
public:
	CVirtualSceneSample();

	Scene*				m_pScene;
	SceneRenderTexture*	m_pRenderTexture;
	SceneLight*			m_pLightA;
	SceneLight*			m_pLightB;
	ScenePrimitive*		m_pShape;
	ScenePrimitive*		m_pShape2;
	COriginMarker*		m_pOriginMarker;
	CArrowLineMesh*		m_pArrow;
};

#endif // CVIRTUALSCENESAMPLE_H