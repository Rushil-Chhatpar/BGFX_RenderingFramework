#include "Framework.h"
#include "Events/GameEvents.h"
#include "Objects/Controller.h"
#include "HeightMapScene.h"
#include "../../Game/Source/GameComponents/GameComponents.h"

HeightMapScene::HeightMapScene(fw::GameCore* GameCore)
    : fw::Scene(GameCore)
{
    fw::GameObject* test = new fw::GameObject(m_pGameCore, "Test");
    m_Objects.push_back(test);
    m_pComponentManager->AddComponent(new fw::Transform(test, vec3(-3, -10, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    //m_pComponentManager->AddComponent(new HeightMapComponent(test, vec2(10, 10), nullptr, m_pGame->GetResourceManager()->GetMaterial("GreyScaleHeightMap2")));
    //test->GetComponent< HeightMapComponent>()->CreateHeightMap("Data/Textures/GreyScaleHeightMap2.png");
    m_pComponentManager->AddComponent(new WaterComponent(test, vec2(50, 50), nullptr, m_pGame->GetResourceManager()->GetMaterial("Water")));
    test->GetComponent< WaterComponent>()->CreateWater(ivec2(64, 64));

    //m_pGame->GetResourceManager()->GetMaterial("GreyScaleHeightMap")->SetDebugMode(false);

    test = new fw::GameObject(m_pGameCore, "Height Map");
    m_Objects.push_back(test);
    m_pComponentManager->AddComponent(new fw::Transform(test, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new HeightMapComponent(test, vec2(50, 50), nullptr, m_pGame->GetResourceManager()->GetMaterial("GreyScaleHeightMap2")));
    test->GetComponent<HeightMapComponent>()->CreateHeightMap("Data/Textures/GreyScaleHeightMap2.png");

    m_PlayerObj = new fw::GameObject(m_pGameCore, "Something");
    m_Objects.push_back(m_PlayerObj);
    m_pComponentManager->AddComponent(new fw::Transform(m_PlayerObj, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(m_PlayerObj, m_pGame->GetResourceManager()->GetMesh("Something"), m_pGame->GetResourceManager()->GetMaterial("Something")));
    m_pComponentManager->AddComponent(new PlayerLogicComponent(m_PlayerObj, m_pController));
    //m_pComponentManager->AddComponent(new fw::FollowComponent(m_PlayerObj, 7.0f, 10.0f));
    m_PlayerObj->GetComponent<PlayerLogicComponent>()->SetHeightMap(test->GetComponent<HeightMapComponent>());

    m_pGame->GetResourceManager()->GetMaterial("Something")->SetDebugMode(false);

    m_FollowCamera = new fw::GameObject(m_pGameCore, "Camera");
    m_Objects.push_back(m_FollowCamera);
    m_pComponentManager->AddComponent(new fw::Transform(m_FollowCamera, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::CameraComponent(m_FollowCamera));
    m_pComponentManager->AddComponent(new OrbitCameraComponent(m_FollowCamera, m_pController, m_PlayerObj, 7.0f, 10.0f));
    vec2 gameSize = m_pGame->GetGameWindowSize();
    m_FollowCamera->GetComponent<fw::CameraComponent>()->SetAspectRatio(gameSize.x / gameSize.y);

    m_PlayerObj->GetComponent<PlayerLogicComponent>()->SetCamera(m_FollowCamera);

    fw::Uniforms* pUniforms = m_pGameCore->GetUniforms();

    vec4 lightColor = vec4(1, 1, 1, 1);
    //vec4 lightPosition[4] = {
    //    vec4(0.0f, 0.0f, 0.0f, 1.0f),
    //    vec4(0.0f, -5.0f, 0.0f, 1.0f),
    //    vec4(5.0f, 0.0f, 0.0f, 1.0f),
    //    vec4(-5.0f, 0.0f, 0.0f, 1.0f),
    //};
    bgfx::setUniform(pUniforms->GetUniform("u_LightColor"), &lightColor);

    //vec4 lightPosition = vec4(25.0f, 10.0f, 25.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(0.0f, 20.0f, 20.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(10.0f, 20.0f, 20.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(50.0f, 20.0f, 50.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(50.0f, 20.0f, 0.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(0.0f, 20.0f, 50.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);
    //lightPosition = vec4(20.0f, 20.0f, 20.0f, 1.0f);
    //m_LightPositions.push_back(lightPosition);

    //vec4 lightPositions[8] = {
    //    m_LightPositions[0],
    //    m_LightPositions[1],
    //    m_LightPositions[2],
    //    m_LightPositions[3],
    //    m_LightPositions[4],
    //    m_LightPositions[5],
    //    m_LightPositions[6],
    //    m_LightPositions[7]
    //};

    test = new fw::GameObject(m_pGameCore, "Light");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(1, 1, 1, 1), 0));

    test = new fw::GameObject(m_pGameCore, "Light2");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(50, 50, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(1, 1, 0, 1), 10));

    test = new fw::GameObject(m_pGameCore, "Light3");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(25, 25, 25), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(1, 1, 1, 1), 15.0f));

    test = new fw::GameObject(m_pGameCore, "Light4");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(0, 0, 50), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(0, 0, 1, 1), 25.0f));

    test = new fw::GameObject(m_pGameCore, "Light5");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(50, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(0, 0, 0, 1), 5.0f));

    test = new fw::GameObject(m_pGameCore, "Light6");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(50, 0, 50), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(0, 1, 1, 1), 27.0f));

    test = new fw::GameObject(m_pGameCore, "Light7");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(25, 100, 25), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(0, 1, 0, 1), 10.0f));

    test = new fw::GameObject(m_pGameCore, "Light8");
    m_Objects.push_back(test);
    m_Lights.push_back(test);
    test->AddComponent(new fw::Transform(test, vec3(50, 20, 50), vec3(1, 1, 1), vec3(0, 0, 0)));
    test->AddComponent(new LightComponent(test, vec4(0, 0, 1, 1), 10.0f));

    //bgfx::setUniform(pUniforms->GetUniform("u_LightPosition"), &lightPositions, 8);

    
}

HeightMapScene::~HeightMapScene()
{
}

void HeightMapScene::OnEvent(fw::Event* pEvent)
{
    m_pController->OnEvent(pEvent);

    // Remove object from GameObject list and delete it.
    if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find(m_Objects.begin(), m_Objects.end(), pObject);
        m_Objects.erase(it);

        delete pObject;
    }

    // Set the new aspect ratio in the camera.
    if (pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType())
    {
        int width = m_pGameCore->GetFramework()->GetWindowClientWidth();
        int height = m_pGameCore->GetFramework()->GetWindowClientHeight();

        m_pCamera->SetAspectRatio((float)width / height);
    }

    //Contact events
    if (pEvent->GetType() == fw::ContactEvent::GetStaticEventType())
    {
        fw::ContactEvent* pContactEvent = static_cast<fw::ContactEvent*>(pEvent);
        if (pContactEvent->GetEventType() == fw::ContactEvent::EventType::BeginContact)
            pContactEvent->BeginContact();
        else if (pContactEvent->GetEventType() == fw::ContactEvent::EventType::EndContact)
            pContactEvent->EndContact();
    }
}

void HeightMapScene::Update(float deltaTime)
{
    vec2 gameSize = m_pGame->GetGameWindowSize();
    m_pCamera->SetAspectRatio(gameSize.x / gameSize.y);
    m_PlayerObj->GetComponent<PlayerLogicComponent>()->Update(deltaTime);

    //m_FollowCamera->GetComponent<OrbitCameraComponent>()->UpdateCameraView(deltaTime);
    m_pComponentManager->UpdateOrbitCameraView(deltaTime);

    m_Lights[0]->GetComponent<fw::Transform>()->m_Position = m_PlayerObj->GetComponent<fw::Transform>()->m_Position;
    m_Lights[0]->GetComponent<fw::Transform>()->m_Position.y += 5.0f;

    m_pCamera->Update(deltaTime);
    Scene::Update(deltaTime);
}

void HeightMapScene::Draw(int viewID)
{
    //m_Objects[0]->GetComponent<HeightMapComponent>()->Draw();
    m_FollowCamera->GetComponent<fw::CameraComponent>()->EnableCamera(viewID);
    //m_PlayerObj->GetComponent<fw::FollowComponent>()->SetViewMatrix();
    SetupLightUniforms();
    Scene::Draw(viewID);
}

void HeightMapScene::Editor_DisplayObjectList()
{
    for (fw::GameObject* pObject : m_Objects)
    {
        bool isSelected = pObject == m_pGame->GetSelectedGameObject();
        if (ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            m_pGame->SetSelectedGameObject(pObject);
        }
    }
}

void HeightMapScene::SetupLightUniforms()
{
    std::vector<vec3> lightPos;
    std::vector<vec4> lightColor;
    std::vector<vec4> lightRange;
    for (int i = 0; i < m_Lights.size(); i++)
    {
        fw::Transform* pTransform = m_Lights[i]->GetComponent<fw::Transform>();
        lightPos.push_back(pTransform->m_Position);
        lightColor.push_back(m_Lights[i]->GetComponent<LightComponent>()->m_Color);
        lightRange.push_back(vec4(m_Lights[i]->GetComponent<LightComponent>()->m_Range, 0, 0, 0));
    }

    vec3 lightPositions[8] = {
    lightPos[0],
    lightPos[1],
    lightPos[2],
    lightPos[3],
    lightPos[4],
    lightPos[5],
    lightPos[6],
    lightPos[7]
};

    vec4 lightrange[8] = {
    lightRange[0],
    lightRange[1],
    lightRange[2],
    lightRange[3],
    lightRange[4],
    lightRange[5],
    lightRange[6],
    lightRange[7]
    };


    fw::Uniforms* pUniforms = m_pGameCore->GetUniforms();
    bgfx::setUniform(pUniforms->GetUniform("u_LightPosition"), &lightPositions, 8);
    bgfx::setUniform(pUniforms->GetUniform("u_LightRange"), &lightRange, 8);
}



