#include "CVirtualSceneSample.h"
#include "Polycode.h"
#include "CCommonMeshes.h"

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
	m_pShape->setPosition(0.0, 5.0, 0.0);
	ResourcePool *globalPool = Services()->getResourceManager()->getGlobalPool();
	m_pShape->setMaterialByName("UnlitUntextured", globalPool);
	m_pScene->addChild(m_pShape);
	
	m_pShape2 = new ScenePrimitive(ScenePrimitive::TYPE_BOX, 1,1,1);
	m_pShape2->loadTexture("native/test.png");
	m_pShape2->setPosition(5, 0, 0);
	m_pScene->addChild(m_pShape2);

	m_pOriginMarker = new COriginMarker();
	m_pScene->addChild(m_pOriginMarker);
	
	m_pArrow = new CArrowLineMesh();
	m_pScene->addChild(m_pArrow);

	Camera* c = m_pScene->getDefaultCamera();
	c->setPosition(0, 0, 10);
}