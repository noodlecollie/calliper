#ifndef CVIRTUALSCENESAMPLE_H
#define CVIRTUALSCENESAMPLE_H

namespace Polycode
{
	class Scene;
	class SceneRenderTexture;
	class SceneLight;
	class ScenePrimitive;
}

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
};

#endif // CVIRTUALSCENESAMPLE_H