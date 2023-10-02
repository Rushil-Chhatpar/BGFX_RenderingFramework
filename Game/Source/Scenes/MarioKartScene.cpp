#include "Framework.h"
#include "Events/GameEvents.h"
#include "Objects/Controller.h"
#include "MarioKartScene.h"
#include "../../Game/Source/GameComponents/GameComponents.h"

MarioKartScene::MarioKartScene(fw::GameCore* GameCore)
    :fw::Scene(GameCore)
{
    PowerupPos();

    b2Vec2 gravity(0, 0);
    m_pPhysicsWorld = new b2World(gravity);
    m_pPhysicsWorld->SetContactListener(m_pGame->GetContactListener());

    fw::GameObject* bg = new fw::GameObject(m_pGameCore, "Background");
    m_Objects.push_back(bg);
    m_pComponentManager->AddComponent(new fw::Transform(bg, vec3(2, 0, 0), vec3(151, 151, 151), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(bg, m_pGame->GetResourceManager()->GetMesh("Background"), m_pGame->GetResourceManager()->GetMaterial("Background")));

    fw::GameObject* map = new fw::GameObject(m_pGameCore, "Map");
    m_Objects.push_back(map);
    m_pComponentManager->AddComponent(new fw::Transform(map, vec3(2, -0.5, 0), vec3(150, 150, 1), vec3(90, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(map, m_pGame->GetResourceManager()->GetMesh("Image"), m_pGame->GetResourceManager()->GetMaterial("Map")));

    //fw::GameObject* map = new fw::GameObject(m_pGameCore, "Map");
    //m_Objects.push_back(map);
    //m_pComponentManager->AddComponent(new fw::Transform(map, vec3(2, -0.5, 0), vec3(150, 150, 1), vec3(90, 0, 0)));
    //m_pComponentManager->AddComponent(new fw::Render(map, m_pGame->GetResourceManager()->GetMesh("Image"), m_pGame->GetResourceManager()->GetMaterial("Map")));

    m_pFollowCamera = new fw::GameObject(m_pGameCore, "Camera");
    m_Objects.push_back(m_pFollowCamera);
    m_pComponentManager->AddComponent(new fw::Transform(m_pFollowCamera, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::CameraComponent(m_pFollowCamera));
    vec2 gameSize = m_pGame->GetGameWindowSize();
    m_pFollowCamera->GetComponent<fw::CameraComponent>()->SetAspectRatio(gameSize.x / gameSize.y);

    // Create Borders
    for (int i = 0; i < m_BorderTransform.size(); i++)
    {
        m_Border = new fw::GameObject(m_pGameCore, "Border");
        m_Objects.push_back(m_Border);
        m_pComponentManager->AddComponent(new fw::Transform(m_Border, m_BorderTransform[i].pos, m_BorderTransform[i].scale, m_BorderTransform[i].rotate));
        m_pComponentManager->AddComponent(new fw::Render(m_Border, m_pGame->GetResourceManager()->GetMesh("Border"), m_pGame->GetResourceManager()->GetMaterial("Border")));
        m_pComponentManager->AddComponent(new fw::Physics(m_Border, m_pPhysicsWorld, b2_staticBody, 0.01f));
    }

    // Create Coins
    for (int i = 0; i < m_CoinTransform.size(); i++)
    {
        m_Coin = new fw::GameObject(m_pGameCore, "Coin");
        m_Objects.push_back(m_Coin);
        m_pComponentManager->AddComponent(new fw::Transform(m_Coin, m_CoinTransform[i].pos, m_CoinTransform[i].scale, m_CoinTransform[i].rotate));
        m_pComponentManager->AddComponent(new fw::Render(m_Coin, m_pGame->GetResourceManager()->GetMesh("Coin"), m_pGame->GetResourceManager()->GetMaterial("Coin")));
        m_pComponentManager->AddComponent(new fw::Physics(m_Coin, m_pPhysicsWorld, b2_dynamicBody, 0.01f));
        m_pComponentManager->AddComponent(new fw::CollisionReactionComponent(m_Coin, fw::CollisionReactionComponent::ObjectType::Coin));
        m_pComponentManager->AddComponent(new fw::CoinComponent(m_Coin));
    }

    // Create Oil Spills
    for (int i = 0; i < m_OilTransform.size(); i++)
    {
        m_OilSpill = new fw::GameObject(m_pGameCore, "Oil Spill");
        m_Objects.push_back(m_OilSpill);

        m_pComponentManager->AddComponent(new fw::Transform(m_OilSpill, m_OilTransform[i].pos, m_OilTransform[i].scale, m_OilTransform[i].rotate));
        m_pComponentManager->AddComponent(new fw::Render(m_OilSpill, m_pGame->GetResourceManager()->GetMesh("Image"), m_pGame->GetResourceManager()->GetMaterial("Oil")));
        m_pComponentManager->AddComponent(new fw::Physics(m_OilSpill, m_pPhysicsWorld, b2_dynamicBody, 0.01f));
        m_pComponentManager->AddComponent(new fw::CollisionReactionComponent(m_OilSpill, fw::CollisionReactionComponent::ObjectType::Oil));
    }

    // Create Speed Boosts
    for (int i = 0; i < m_SpeedTransform.size(); i++)
    {
        m_SpeedBoost = new fw::GameObject(m_pGameCore, "Speed Boost");
        m_Objects.push_back(m_SpeedBoost);

        m_pComponentManager->AddComponent(new fw::Transform(m_SpeedBoost, m_SpeedTransform[i].pos, m_SpeedTransform[i].scale, m_SpeedTransform[i].rotate));
        m_pComponentManager->AddComponent(new fw::Render(m_SpeedBoost, m_pGame->GetResourceManager()->GetMesh("Boost"), m_pGame->GetResourceManager()->GetMaterial("SpeedBoost")));
        m_pComponentManager->AddComponent(new fw::Physics(m_SpeedBoost, m_pPhysicsWorld, b2_dynamicBody, 0.01f));
        m_pComponentManager->AddComponent(new fw::CollisionReactionComponent(m_SpeedBoost, fw::CollisionReactionComponent::ObjectType::SpeedBoost));
    }

    m_Mario = new fw::GameObject(m_pGameCore, "Mario");
    m_Objects.push_back(m_Mario);
    m_pComponentManager->AddComponent(new fw::Transform(m_Mario, vec3(0, 0, 0), vec3(1, 1, 1), vec3(360, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(m_Mario, m_pGame->GetResourceManager()->GetMesh("Coin"), m_pGame->GetResourceManager()->GetMaterial("Mario")));
    m_pComponentManager->AddComponent(new fw::Physics(m_Mario, m_pPhysicsWorld, b2_dynamicBody, 1.0f));
    m_pComponentManager->AddComponent(new fw::FollowComponent(m_Mario, 5.0f, 0.9f));
    m_pComponentManager->AddComponent(new fw::MarioKartPhysicsComponent(m_Mario, m_pController));
    m_pComponentManager->AddComponent(new fw::CoinCountComponent(m_Mario));

    //FOR JAZ
    m_Mario->GetComponent<fw::MarioKartPhysicsComponent>()->SetSpeed(0.01f);
    m_Mario->GetComponent<fw::MarioKartPhysicsComponent>()->SetAngularSpeed(4.0f);

    //test = new fw::GameObject(m_pGameCore, "Test");
    //m_Objects.push_back(test);
    //m_pComponentManager->AddComponent(new fw::Transform(test, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    //m_pComponentManager->AddComponent(new HeightMapComponent(test, ivec2(3, 5), vec2(2, 2), nullptr, m_pGame->GetResourceManager()->GetMaterial("Background")));
    //test->GetComponent< HeightMapComponent>()->CreateHeightMap();
    //m_pGame->GetResourceManager()->GetMaterial("Background")->SetDebugMode(false);
}

MarioKartScene::~MarioKartScene()
{
}

void MarioKartScene::OnEvent(fw::Event* pEvent)
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

        m_pFollowCamera->GetComponent<fw::CameraComponent>()->SetAspectRatio((float)width / height);
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

void MarioKartScene::Update(float deltaTime)
{
    vec2 gameSize = m_pGame->GetGameWindowSize();

    for (fw::GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    Elapsed += deltaTime;

    if (m_pGame->GetState() == Game::State_Playing)
    {
        m_TimeSinceLastPhysicsStep += deltaTime;
        while (m_TimeSinceLastPhysicsStep >= 1 / 60.0f)
        {
            m_pPhysicsWorld->Step(1 / 60.0f, 8, 3);
            m_TimeSinceLastPhysicsStep -= 1 / 60.0f;
        }
    }

    //testing
    //m_Coin->GetComponent<fw::CoinComponent>()->OnCoinPickup(deltaTime);

    //update the view matrix before syncing transform and physics body. Because of draw call lag

    //m_Mario->GetComponent<fw::MarioKartPhysicsComponent>()->UpdatePhysicsMovement(deltaTime);
    m_pFollowCamera->GetComponent<fw::CameraComponent>()->SetAspectRatio(gameSize.x / gameSize.y);

    m_Mario->GetComponent<fw::FollowComponent>()->SetViewMatrix();

    //update coin pickup animation
    m_pComponentManager->OnCoinPickup(deltaTime);

    //update collision update
    m_pComponentManager->UpdateforCollision(deltaTime);

    //update MarioKart Physics
    m_pComponentManager->UpdatePhysicsMovement(deltaTime);

    Scene::Update(deltaTime);
}

void MarioKartScene::Draw(int viewID)
{
    m_pFollowCamera->GetComponent<fw::CameraComponent>()->EnableCamera(viewID);

    //m_pPhysicsWorld->DebugDraw();

    //test->GetComponent<HeightMapComponent>()->Draw();
    Scene::Draw(viewID);
}

void MarioKartScene::Editor_DisplayObjectList()
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

void MarioKartScene::PowerupPos()
{
    // Coin Positions / Scale / Rotation
    m_CoinTransform.push_back(MarioKartTransform(vec3(-13.757, 0, 45.067), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-11.686, 0, 47.276), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-9.191, 0, 49.232), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(6.010, 0, 48.168), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(8.561, 0, 46.397), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(10.528, 0, 44.063), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(64.857, 0, -27.356), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(66.991, 0, -27.287), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(62.624, 0, -27.753), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(61.176, 0, -61.372), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(58.855, 0, -59.058), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(56.610, 0, -57.029), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(46.419, 0, -61.777), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(45.804, 0, -64.306), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(46.338, 0, -66.329), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-11.019, 0, -34.925), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-13.855, 0, -32.109), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-16.412, 0, -29.793), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-57.394, 0, -24.502), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-54.873, 0, -19.744), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-50.594, 0, -16.701), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(11.904, 0, 1.357), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(14.284, 0, 4.057), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(16.702, 0, 6.636), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(17.434, 0, 8.607), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(18.081, 0, 11.078), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(17.886, 0, 13.723), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(16.707, 0, 15.468), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(14.417, 0, 17.824), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(12.532, 0, 19.887), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-19.342, 0, 18.263), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-19.374, 0, 15.545), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-19.374, 0, 20.521), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-38.142, 0, 15.816), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-38.328, 0, 18.051), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_CoinTransform.push_back(MarioKartTransform(vec3(-38.531, 0, 20.404), vec3(1, 1, 1), vec3(0, 0, 0)));

    // Oil Spill Positions / Scale / Rotation
    m_OilTransform.push_back(MarioKartTransform(vec3(-2.836, -0.5, 42.396), vec3(3, 3, 3), vec3(90, 0, 0)));
    m_OilTransform.push_back(MarioKartTransform(vec3(-40.627, -0.5, -66.909), vec3(3, 3, 3), vec3(90, 0, 0)));
    m_OilTransform.push_back(MarioKartTransform(vec3(-12.481, -0.5, -5.311), vec3(3, 3, 3), vec3(90, 0, 0)));

    // Speed Boost Positions / Scale / Rotation
    m_SpeedTransform.push_back(MarioKartTransform(vec3(62.870, -1.5, 38.668), vec3(4, 2, 2), vec3(0, 180, 0)));
    m_SpeedTransform.push_back(MarioKartTransform(vec3(4.2, -1.5, -27.5), vec3(4, 2, 2), vec3(0, 270, 0)));
    m_SpeedTransform.push_back(MarioKartTransform(vec3(-61.7, -1.5, -38.8), vec3(4, 2, 2), vec3(0, 0, 0)));
    m_SpeedTransform.push_back(MarioKartTransform(vec3(-41.8, -1.5, 60.2), vec3(4, 2, 2), vec3(0, 90, 0)));

    // Border Positions / Scale / Rotation
    m_BorderTransform.push_back(MarioKartTransform(vec3(69, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(41, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(27, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(13, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-1, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-15, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-29, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-43, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-57, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-65, 0, -74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, 66.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, 52.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, 38.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, 24.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, 10.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, -3.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, -17.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, -31.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, -45.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, -59.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(76.5, 0, -67.8), vec3(14, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-65.8, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-51.8, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-37.8, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-23.8, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-9.8, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(4.2, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(18.2, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(32.2, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(46.2, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(60.2, 0, 74.3), vec3(14, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(70, 0, 74.3), vec3(14, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, -67.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, -53.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, -39.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, -25.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, -11.8), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, 2.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, 16.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, 30.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, 44.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, 58.2), vec3(14, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-72.3, 0, 66.8), vec3(14, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(0.5, 0, 7.5), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-11.5, 0, 7.5), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(7.3, 0, 8.7), vec3(3.5, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(5.5, 0, 10), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-6.5, 0, 10), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-14.1, 0, 10), vec3(12.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-19.7, 0, 7.5), vec3(5.8, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-25.2, 0, 5.2), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-37.3, 0, 5.2), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-49.3, 0, 5.2), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-61.3, 0, 5.2), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-65.8, 0, 5.2), vec3(12.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-22.8, 0, 1.7), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-34.8, 0, 1.7), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-37.3, 0, 1.7), vec3(12.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-49.3, 0, -3), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-61.3, 0, -3), vec3(12.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-65.8, 0, -3), vec3(12.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-43, 0, -0.65), vec3(5.7, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-17.5, 0, 4.5), vec3(5.7, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(2, 0, -41.7), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-5.6, 0, -48.2), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-5.6, 0, -62.3), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-5.6, 0, -66.8), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(9.6, 0, -48.2), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(9.6, 0, -62.3), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(9.6, 0, -66.8), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-38.9, 0, -47.65), vec3(14.3, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-31.3, 0, -32.8), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-46.6, 0, -32.8), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-46.6, 0, -41.1), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-31.3, 0, -41.1), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-46.6, 0, -29.5), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-40, 0, -22.9), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-35.5, 0, -22.9), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -45.2), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -39.3), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -33.5), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -27.6), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -21.8), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -15.9), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -10), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, -4.1), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, 1.7), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, 7.6), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, 13.4), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, 19.3), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, 25.1), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(53.6, 0, 31), vec3(4.7, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55.45, 0, -42.3), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55.45, 0, -30.6), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55.45, 0, -18.7), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55.45, 0, -7.1), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55.45, 0, 4.6), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(55.45, 0, 16.3), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, -36.5), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, -24.6), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, -1.3), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, 10.5), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, 22.2), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, -12.9), vec3(5.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.75, 0, -48.1), vec3(6.8, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(44.4, 0, -51), vec3(14.6, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(37.6, 0, -43.6), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-28.4, 0, -26.4), vec3(4.5, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-26.7, 0, -19.4), vec3(3.5, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-23.2, 0, -22.9), vec3(4.5, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-20.5, 0, -21.2), vec3(4.5, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-28.9, 0, -21.2), vec3(4.5, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-24.3, 0, -17.7), vec3(3.5, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-25.6, 0, -24.6), vec3(4.5, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(7.3, 0, -14.1), vec3(3.5, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(37.6, 0, -29.5), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(37.6, 0, -26), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(30.1, 0, -19.5), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(16, 0, -19.5), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(1.9, 0, -19.5), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-12.2, 0, -19.5), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-14.4, 0, -19.5), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(0.9, 0, -15.9), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-13.2, 0, -15.9), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-17.8, 0, -15.9), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(31.2, 0, -12.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(17.1, 0, -12.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(13.7, 0, -12.3), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(37.76, 0, -4.8), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(37.76, 0, 1.1), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(39.55, 0, 7.7), vec3(4.5, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(41.3, 0, 15.2), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(41.3, 0, 26.9), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(33.8, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(19.7, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(5.6, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-8.5, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-22.6, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-36.7, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-40.1, 0, 33.3), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-46.6, 0, 39.2), vec3(12.8, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-39.05, 0, 45.15), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-33, 0, 45.15), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-26.4, 0, 40.95), vec3(9.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-18.9, 0, 36.9), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(-4.8, 0, 36.9), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(9.3, 0, 36.9), vec3(14.1, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(23.1, 0, 36.9), vec3(14.1, 1, 1), vec3(0, 180, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(29.5, 0, 43.35), vec3(13.9, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(36.9, 0, 49.8), vec3(13.9, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(45.5, 0, 49.8), vec3(13.9, 1, 1), vec3(0, 0, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.95, 0, 42.4), vec3(14.1, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(51.95, 0, 38.5), vec3(14.1, 1, 1), vec3(0, 270, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(0.3, 0, 69.1), vec3(9.2, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(1.4, 0, 69.1), vec3(9.2, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(0.3, 0, 58.6), vec3(7, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(1.4, 0, 58.6), vec3(7, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(42.4, 0, 55), vec3(9.4, 1, 1), vec3(0, 90, 0)));
    m_BorderTransform.push_back(MarioKartTransform(vec3(41.3, 0, 55), vec3(9.4, 1, 1), vec3(0, 90, 0)));

}