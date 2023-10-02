#include "PhysicsScene.h"
#include "Events/GameEvents.h"
#include "Objects/Controller.h"
#include "Objects/Player.h"
//#include "../../Framework/Source/Math/Vector.h"
#include "Meshes/VertexFormats.h"

class MyDebugDraw : public b2Draw
{
public:
    fw::Uniforms* m_pUniforms;
    fw::ShaderProgram* m_Shader;
    int m_ViewID;


    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {}

    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override 
    {
        VertexFormat_PosColor verts[4] =
        {
            { vec2(-0.5f, 0.5f),0, 255, 0, 255},    //top left                             //0
            { vec2(0.5f, 0.5f), 0,   0, 0, 255},     //top right                          //1
            { vec2(0.5f, -0.5f),0,   0, 0, 255 },  //bottom right                       //2
            { vec2(-0.5f, -0.5f), 0, 255, 0, 255},   //bottom left                        //3
        };                               

        uint16 indices[6] =
        {
            0,1,2,0,2,3
        };

        DrawShape(verts, indices, 4, 6); 
    }

    /// Draw a circle.
    virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override {}

    /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override {}

    /// Draw a line segment.
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {}

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform(const b2Transform& xf) override {}

    /// Draw a point.
    virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override {}

    void DrawShape(VertexFormat_PosColor* triangleVerts, uint16* triangleIndices, uint32_t numVertices, uint32_t numIndices)
    {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        int maxVerts = bgfx::getAvailTransientVertexBuffer(numVertices, VertexFormat_PosColor::format);
        int maxIndices = bgfx::getAvailTransientIndexBuffer(numIndices);
        if ((numVertices != maxVerts) || (numIndices != maxIndices))
        {
            // Not enough space in transient buffer, quit drawing the rest...
            return;
        }

        mat4 worldMatrix;
        worldMatrix.SetIdentity();
        bgfx::setUniform(m_pUniforms->GetUniform("u_WorldMatrix"), &worldMatrix);

        bgfx::allocTransientVertexBuffer(&tvb, numVertices, VertexFormat_PosColor::format);
        bgfx::allocTransientIndexBuffer(&tib, numIndices);

        VertexFormat_PosColor* vertData = (VertexFormat_PosColor*)tvb.data;
        memcpy(vertData, triangleVerts, numIndices * sizeof(VertexFormat_PosColor::format));

        uint16* indexData = (uint16*)tib.data;
        memcpy(indexData, triangleIndices, numIndices * sizeof(uint16));

        uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA |
            BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

        

        bgfx::setState(state);
        bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
        bgfx::setIndexBuffer(&tib, 0, numIndices);
        bgfx::submit(EditorView_Box2DDebugDraw, m_Shader->GetProgram());
    }

};

PhysicsScene::PhysicsScene(fw::GameCore* GameCore)
    : Scene(GameCore)
{
    
    b2Vec2 gravity(0, -20);
    m_pPhysicsWorld = new b2World(gravity);
    m_pPhysicsWorld->SetContactListener(m_pGame->GetContactListener());

    MyDebugDraw* pDebugDraw = new MyDebugDraw;
    pDebugDraw->SetFlags(b2Draw::e_shapeBit);
    pDebugDraw->m_pUniforms = m_pGameCore->GetUniforms();
    pDebugDraw->m_Shader = m_pGame->GetResourceManager()->GetShader("VertexColor");
    pDebugDraw->m_ViewID = EditorView_Box2DDebugDraw;
    m_pPhysicsWorld->SetDebugDraw(pDebugDraw);


    //SET GAME OBJECTS

    fw::GameObject* bg = new fw::GameObject(m_pGameCore, "Background");
    m_Objects.push_back(bg);
    m_pComponentManager->AddComponent(new fw::Transform(bg, vec3(5, 9, 0), vec3(29.5, 10.5f, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(bg, m_pGame->GetResourceManager()->GetMesh("Image"), m_pGame->GetResourceManager()->GetMaterial("Background")));

    fw::GameObject* map = new fw::GameObject(m_pGameCore, "Map");
    m_Objects.push_back(map);
    m_pComponentManager->AddComponent(new fw::Transform(map, vec3(2, -1, 0), vec3(200, 200, 1), vec3(90, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(map, m_pGame->GetResourceManager()->GetMesh("Coin"), m_pGame->GetResourceManager()->GetMaterial("Map")));
    
    fw::GameObject* tempObject = new fw::GameObject(m_pGameCore, "Temp");
    m_Objects.push_back(tempObject);
    m_pComponentManager->AddComponent(new fw::Transform(tempObject, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(tempObject, m_pGame->GetResourceManager()->GetMesh("Die"), m_pGame->GetResourceManager()->GetMaterial("Die")));
    m_pComponentManager->AddComponent(new fw::Physics(tempObject, m_pPhysicsWorld, b2_staticBody, 1.0f));
    
    fw::GameObject* tempObject2 = new fw::GameObject(m_pGameCore, "Temp2");
    m_Objects.push_back(tempObject2);
    m_pComponentManager->AddComponent(new fw::Transform(tempObject2, vec3(1, 1, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(tempObject2, m_pGame->GetResourceManager()->GetMesh("Die"), m_pGame->GetResourceManager()->GetMaterial("Die")));
    m_pComponentManager->AddComponent(new fw::Physics(tempObject2, m_pPhysicsWorld, b2_staticBody, 1.0f));

    fw::GameObject* Coin = new fw::GameObject(m_pGameCore, "Coin");
    m_Objects.push_back(Coin);
    m_pComponentManager->AddComponent(new fw::Transform(Coin, vec3(5, 5, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(Coin, m_pGame->GetResourceManager()->GetMesh("Coin"), m_pGame->GetResourceManager()->GetMaterial("Coin")));
    m_pComponentManager->AddComponent(new fw::Physics(Coin, m_pPhysicsWorld, b2_staticBody, 1.0f));

    m_pPlayer = new Player(m_pGameCore, m_pController, "Player");
    m_Objects.push_back(m_pPlayer);
    m_pComponentManager->AddComponent(new fw::Transform(m_pPlayer, vec3(0, 5, 0), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(m_pPlayer, m_pGame->GetResourceManager()->GetMesh("Sprite"), m_pGame->GetResourceManager()->GetMaterial("MegaMan")));
    m_pComponentManager->AddComponent(new fw::Physics(m_pPlayer, m_pPhysicsWorld, b2_dynamicBody, 1.0f));

    fw::GameObject* tempObject3 = new fw::GameObject(m_pGameCore, "Temp3");
    m_Objects.push_back(tempObject3);
    m_pComponentManager->AddComponent(new fw::Transform(tempObject3, vec3(2, 2, -10), vec3(1, 1, 1), vec3(0, 0, 0)));
    m_pComponentManager->AddComponent(new fw::Render(tempObject3, m_pGame->GetResourceManager()->GetMesh("Cube"), m_pGame->GetResourceManager()->GetMaterial("VertexColor")));
    m_pComponentManager->AddComponent(new fw::Physics(tempObject3, m_pPhysicsWorld, b2_staticBody, 1.0f));
        
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::OnEvent(fw::Event* pEvent)
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

void PhysicsScene::Update(float deltaTime)
{
    for (fw::GameObject* pObject : m_Objects)
    {
        pObject->Update(deltaTime);
    }

    vec2 gameSize = m_pGame->GetGameWindowSize();
    m_pCamera->SetAspectRatio(gameSize.x/gameSize.y);

    m_pCamera->Update(deltaTime);


    if (m_pGame->GetState() == Game::State_Playing)
    {
        m_TimeSinceLastPhysicsStep += deltaTime;
        while (m_TimeSinceLastPhysicsStep >= 1 / 60.0f)
        {
            m_pPhysicsWorld->Step(1 / 60.0f, 8, 3);
            m_TimeSinceLastPhysicsStep -= 1 / 60.0f;
        }
    }

    m_Objects.back()->SetRotation(vec3(0, m_pGame->GetElapsedTime() * 30, 0));

    Scene::Update(deltaTime);

}

void PhysicsScene::Draw(int viewID)
{

    m_pCamera->Enable(viewID);

    m_pPhysicsWorld->DebugDraw();

    Scene::Draw(viewID);
}

void PhysicsScene::Editor_DisplayObjectList()
{
    //Scene::Editor_DisplayObjectList();
    for (fw::GameObject* pObject : m_Objects)
    {
        bool isSelected = pObject == m_pGame->GetSelectedGameObject();
        if(ImGui::Selectable(pObject->GetName().c_str(), &isSelected))
        {
            m_pGame->SetSelectedGameObject(pObject);
        }
        
    }
}
