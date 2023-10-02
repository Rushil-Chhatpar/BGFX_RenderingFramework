#include "Scene.h"
#include "Framework.h"
#include "../../Game/Source/Objects/Controller.h"
#include "../../Game/Source/Game.h"


namespace fw
{
    Scene::Scene(GameCore* GameCore)
        : m_pGameCore(GameCore)
    {
        m_pGame = static_cast<Game*>(m_pGameCore);
        // Create a controller.
        m_pController = new Controller();

        //Create a component Manager
        m_pComponentManager = m_pGame->GetComponentManager();

        // Create some GameObjects.
        m_pCamera = new Camera(m_pGameCore, vec3(5, 5, 0));
    }

    Scene::~Scene()
    {
        delete m_pComponentManager;
        delete m_pController;
        delete m_pCamera;
        delete m_pPhysicsWorld;
    }

    void Scene::StartFrame(float deltaTime)
    {
        m_pController->StartFrame();
    }

    void Scene::OnEvent(fw::Event* pEvent)
    {

    }

    void Scene::Update(float deltaTime)
    {
        //update Transforms
        m_pComponentManager->UpdateTransform();

        //Sync Body and Transform
        m_pComponentManager->SyncTransform();

    }

    void Scene::Draw(int viewID)
    {
        //Draw Objects
        m_pComponentManager->Draw();
    }

    void Scene::Editor_DisplayObjectList()
    {
        for (GameObject* pObject : m_Objects)
        {
            ImGui::Text("%s", pObject->GetName().c_str());
        }
    }

    void Scene::SaveToJSON(nlohmann::json& json)
    {
        json["Objects"] = nlohmann::json::array();

        for (GameObject* pObject : m_Objects)
        {
            nlohmann::json jsonObject;
            pObject->SaveToJSON(jsonObject);
            json["Objects"].push_back(jsonObject);
        }
    }

    void Scene::LoadFromJSON(nlohmann::json& json)
    {
        for (GameObject* pObject : m_Objects)
        {
            pObject->LoadFromJSON(json);
        }
    }

    void Scene::EnableGameObject(GameObject* pObject)
    {
        if (pObject->IsActive() == true)
            return;

        if (pObject->GetComponent<Transform>() != nullptr)
            m_pComponentManager->AddtoManager(pObject->GetComponent<Transform>());
        if (pObject->GetComponent<Render>() != nullptr)
            m_pComponentManager->AddtoManager(pObject->GetComponent<Render>());
        if (pObject->GetComponent<Physics>() != nullptr)
        {
            m_pComponentManager->AddtoManager(pObject->GetComponent<Physics>());
            pObject->GetComponent<Physics>()->m_pBody->SetEnabled(true);
        }

        pObject->SetActive(true);
    }

    void Scene::DisableGameObject(GameObject* pObject)
    {
        if (pObject->IsActive() == false)
            return;

        if (pObject->GetComponent<Transform>() != nullptr)
            m_pComponentManager->RemovefromManager(pObject->GetComponent<Transform>());
        if (pObject->GetComponent<Render>() != nullptr)
            m_pComponentManager->RemovefromManager(pObject->GetComponent<Render>());
        if (pObject->GetComponent<Physics>() != nullptr)
        {
            m_pComponentManager->RemovefromManager(pObject->GetComponent<Physics>());
            pObject->GetComponent<Physics>()->m_pBody->SetEnabled(false);
        }

        pObject->SetActive(false);
    }
}