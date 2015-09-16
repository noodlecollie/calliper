#include "CVirtualSceneSample.h"
#include "Polycode.h"

CVirtualSceneSample::CVirtualSceneSample()
{
	m_pScene = new Scene(Scene::SCENE_3D, true);
	m_pRenderTexture = new SceneRenderTexture(m_pScene, m_pScene->getDefaultCamera(), 256, 256);

	m_pScene->clearColor.setColor(0.35f, 0.71f, 1.0f, 1.0f);
	m_pScene->useClearColor = true;

	Number customFalloff = 0.006;
	m_pLightA = new SceneLight(SceneLight::POINT_LIGHT, m_pScene, 999999, customFalloff, customFalloff, customFalloff);
	m_pScene->addLight(m_pLightA);

	m_pLightB = new SceneLight(SceneLight::POINT_LIGHT, m_pScene, 999999, customFalloff, customFalloff, customFalloff);
	m_pScene->addLight(m_pLightB);

	m_pLightA->setPosition(9999, 9999, 9999);
	m_pLightB->setPosition(-9999, -9999, -9999);

	m_pShape = new ScenePrimitive(ScenePrimitive::TYPE_TORUS, 0.8, 0.3, 30, 20);
	m_pShape->setPosition(0.0, 0.0, 0.0);
	m_pScene->addEntity(m_pShape);
	
	m_pShape2 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 1, 1);
	m_pShape2->setPosition(5, 0, 0);
	m_pScene->addEntity(m_pShape2);

	Camera* c = m_pScene->getDefaultCamera();
	c->setPosition(0, 0, 10);
}