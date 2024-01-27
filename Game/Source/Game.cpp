//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "DataTypes.h"
#include "Game.h"
#include "Events/GameEvents.h"
#include "Objects/Player.h"
#include "Objects/Controller.h"
#include "Meshes/Shapes.h"
#include "Meshes/VertexFormats.h"
#include "Scenes/PhysicsScene.h"
#include "Scenes/MarioKartScene.h"
#include "Scenes/HeightMapScene.h"
#include "Meshes/GameMesh.h"

using json = nlohmann::json;

Game::Game(fw::FWCore& fwCore)
    : GameCore(fwCore)
{
    //create a texture and a framebuffer object
    m_FBOTextureHandle = bgfx::createTexture2D(4096, 4096, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT);
    m_DepthHandle = bgfx::createTexture2D(4096, 4096, false, 1, bgfx::TextureFormat::D32F, BGFX_TEXTURE_RT);
    bgfx::TextureHandle textures[] = { m_FBOTextureHandle , m_DepthHandle};
    m_FBO = bgfx::createFrameBuffer(2, textures);
    
    m_State = State_Playing;

    // General renderer settings.
    
    bgfx::setViewFrameBuffer(EditorView_Game, m_FBO);
    bgfx::setViewClear(EditorView_Game, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0);
    bgfx::setViewRect(EditorView_Game, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight());

    bgfx::setViewFrameBuffer(EditorView_Box2DDebugDraw, m_FBO);
    bgfx::setViewClear(EditorView_Box2DDebugDraw, BGFX_CLEAR_NONE, 0x000030ff, 1.0f, 0);
    bgfx::setViewRect(EditorView_Box2DDebugDraw, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight());


    bgfx::setViewClear(EditorView_ImGui, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0);
    bgfx::setViewRect(EditorView_ImGui, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight());

    m_GameWindowSize = vec2(1280, 720);

    // Create some manager objects.
    m_pImGuiManager = new fw::ImGuiManager(&m_FWCore, EditorView_ImGui);
    m_pEventManager = new fw::EventManager();

    //Create a Resource Manager
    m_pResourceManager = new fw::ResourceManager();

    // Create uniforms.
    m_pUniforms = new fw::Uniforms();
    m_pUniforms->CreateUniform("u_Position", bgfx::UniformType::Vec4);
    m_pUniforms->CreateUniform("u_Rotation", bgfx::UniformType::Vec4);
    m_pUniforms->CreateUniform("u_Scale", bgfx::UniformType::Vec4);

    m_pUniforms->CreateUniform("u_CameraPosition", bgfx::UniformType::Vec4);
    m_pUniforms->CreateUniform("u_ProjectionScale", bgfx::UniformType::Vec4);

    m_pUniforms->CreateUniform("u_DiffuseColor", bgfx::UniformType::Vec4);
    m_pUniforms->CreateUniform("u_TextureColor", bgfx::UniformType::Sampler);
    m_pUniforms->CreateUniform("u_UVScale", bgfx::UniformType::Vec4);
    m_pUniforms->CreateUniform("u_UVOffset", bgfx::UniformType::Vec4);

    m_pUniforms->CreateUniform("u_Time", bgfx::UniformType::Vec4);

    m_pUniforms->CreateUniform("u_WorldMatrix", bgfx::UniformType::Mat4);
    m_pUniforms->CreateUniform("u_ViewMatrix", bgfx::UniformType::Mat4);
    m_pUniforms->CreateUniform("u_ProjMatrix", bgfx::UniformType::Mat4);

    m_pUniforms->CreateUniform("u_NormalMatrix", bgfx::UniformType::Mat4);

    m_pUniforms->CreateUniform("u_LightColor", bgfx::UniformType::Vec4);
    m_pUniforms->CreateUniform("u_LightPosition", bgfx::UniformType::Vec4, 8);

    m_pUniforms->CreateUniform("u_CameraEyePosition", bgfx::UniformType::Vec4);

    m_pUniforms->CreateUniform("u_LightRange", bgfx::UniformType::Vec4, 8);


    // Create vertex formats.
    InitVertexFormats();

    // Create some meshes.
    //m_pMeshes["Triangle"] = CreateTriangleMesh();
    //m_pMeshes["Square"] = CreateSquareMesh();
    //m_pMeshes["Sprite"] = CreateSpriteMesh();
    //m_pMeshes["Cube"] = CreateCubeMesh();
    //m_pMeshes["Die"] = CreateDieMesh();
    m_pResourceManager->AddMesh("Triangle", CreateTriangleMesh());
    m_pResourceManager->AddMesh("Square", CreateSquareMesh());
    m_pResourceManager->AddMesh("Sprite", CreateSpriteMesh());
    m_pResourceManager->AddMesh("Cube", CreateCubeMesh());
    m_pResourceManager->AddMesh("Die", CreateDieMesh());
    m_pResourceManager->AddMesh("Coin", CreateCoinMesh());
    m_pResourceManager->AddMesh("Image", CreateImageMesh());
    m_pResourceManager->AddMesh("Border", CreateBorderMesh());
    m_pResourceManager->AddMesh("Boost", CreateBoostMesh());
    m_pResourceManager->AddMesh("Background", CreateBackgroundMesh());

    // Load obj meshes
    m_pResourceManager->AddMesh("OBJTest", new GameMesh(VertexFormat_Pos3dUVNormal::format, "Data/Meshes/Space_Ship_with_guns.obj"));
    m_pResourceManager->AddMesh("Something", new GameMesh(VertexFormat_Pos3dUVNormal::format, "Data/Meshes/Something.obj"));


    // Load some shaders.
    //m_pShaders["SolidColor"] = new fw::ShaderProgram("Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin");
    //m_pShaders["VertexColor"] = new fw::ShaderProgram("Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin");
    //m_pShaders["Texture"] = new fw::ShaderProgram("Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin");
    m_pResourceManager->AddShader("SolidColor", new fw::ShaderProgram("Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin"));
    m_pResourceManager->AddShader("VertexColor", new fw::ShaderProgram("Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin"));
    m_pResourceManager->AddShader("Texture", new fw::ShaderProgram("Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin"));
    m_pResourceManager->AddShader("DebugNormals", new fw::ShaderProgram("Data/Shaders/", "DebugNormals.vert.bin", "DebugNormals.frag.bin"));
    m_pResourceManager->AddShader("Light", new fw::ShaderProgram("Data/Shaders/", "Light.vert.bin", "Light.frag.bin"));
    m_pResourceManager->AddShader("Water", new fw::ShaderProgram("Data/Shaders/", "Water.vert.bin", "Water.frag.bin"));



    // Load some textures.

    //m_pTextures["MegaMan"] = new fw::Texture("Data/Textures/MegaMan.png");
    //m_pTextures["Die"] = new fw::Texture("Data/Textures/Die.png");
    m_pResourceManager->AddTexture("MegaMan", new fw::Texture("Data/Textures/MegaMan.png"));
    m_pResourceManager->AddTexture("Die", new fw::Texture("Data/Textures/Die.png"));
    m_pResourceManager->AddTexture("Coin", new fw::Texture("Data/Textures/Coin.png"));
    m_pResourceManager->AddTexture("Map", new fw::Texture("Data/Textures/Map.png"));
    m_pResourceManager->AddTexture("Background", new fw::Texture("Data/Textures/Background.png"));
    m_pResourceManager->AddTexture("Oil", new fw::Texture("Data/Textures/Oil.png"));
    m_pResourceManager->AddTexture("SpeedBoost", new fw::Texture("Data/Textures/Booster_Pad.png"));
    m_pResourceManager->AddTexture("Mario", new fw::Texture("Data/Textures/KartMarioBoth.png"));
    m_pResourceManager->AddTexture("Border", new fw::Texture("Data/Textures/Border.png"));
    m_pResourceManager->AddTexture("Something", new fw::Texture("Data/Textures/Something.png"));
    m_pResourceManager->AddTexture("GreyScaleHeightMap", new fw::Texture("Data/Textures/GreyScaleHeightMap.png"));
    m_pResourceManager->AddTexture("GreyScaleHeightMap2", new fw::Texture("Data/Textures/GreyScaleHeightMap2.png"));
    m_pResourceManager->AddTexture("Water", new fw::Texture("Data/Textures/Water.png"));
    m_pResourceManager->AddTexture("PoolWater", new fw::Texture("Data/Textures/PoolWater.png"));


    
    // Create some materials.
    //m_pMaterials["Red"] = new fw::Material(m_pShaders["SolidColor"], nullptr, fw::color4f::Red(), false);
    //m_pMaterials["Blue"] = new fw::Material(m_pShaders["SolidColor"], nullptr, fw::color4f::Blue(), false);
    //m_pMaterials["Green"] = new fw::Material(m_pShaders["SolidColor"], nullptr, fw::color4f::Green(), false);
    //m_pMaterials["VertexColor"] = new fw::Material(m_pShaders["VertexColor"], nullptr, fw::color4f::White(), false);
    //m_pMaterials["MegaMan"] = new fw::Material(m_pShaders["Texture"], m_pTextures["MegaMan"], fw::color4f::White(), true);
    //m_pMaterials["Die"] = new fw::Material(m_pShaders["Texture"], m_pTextures["Die"], fw::color4f::White(), true);
    m_pResourceManager->AddMaterial("Red", new fw::Material(m_pResourceManager->GetShader("SolidColor"), nullptr, fw::color4f::Red(), false));
    m_pResourceManager->AddMaterial("Blue", new fw::Material(m_pResourceManager->GetShader("SolidColor"), nullptr, fw::color4f::Blue(), false));
    m_pResourceManager->AddMaterial("Green", new fw::Material(m_pResourceManager->GetShader("SolidColor"), nullptr, fw::color4f::Green(), false));
    m_pResourceManager->AddMaterial("VertexColor", new fw::Material(m_pResourceManager->GetShader("VertexColor"), nullptr, fw::color4f::White(), false));
    m_pResourceManager->AddMaterial("MegaMan", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("MegaMan"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Die", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Die"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Coin", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Coin"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Map", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Map"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Background", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Background"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Mario", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Mario"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Oil", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Oil"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("SpeedBoost", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("SpeedBoost"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Border", new fw::Material(m_pResourceManager->GetShader("Texture"), m_pResourceManager->GetTexture("Border"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Something", new fw::Material(m_pResourceManager->GetShader("Light"), m_pResourceManager->GetTexture("Something"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("GreyScaleHeightMap", new fw::Material(m_pResourceManager->GetShader("Light"), m_pResourceManager->GetTexture("GreyScaleHeightMap"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("GreyScaleHeightMap2", new fw::Material(m_pResourceManager->GetShader("Light"), m_pResourceManager->GetTexture("GreyScaleHeightMap2"), fw::color4f::White(), true));
    m_pResourceManager->AddMaterial("Water", new fw::Material(m_pResourceManager->GetShader("Water"), m_pResourceManager->GetTexture("Water"), fw::color4f::White(), true));


    m_pContactListener = new fw::Box2DContactListener(m_pEventManager);

    m_pComponentManager = new fw::ComponentManager();

    
    //DisableGameObject(tempObject2);
    //EnableGameObject(tempObject2);

    //m_pPhysicsScene = new PhysicsScene(this);
    //m_pMarioKartScene = new MarioKartScene(this);
    //m_pMarioKartScene = new HeightMapScene(this);
    m_ActiveScene = new MarioKartScene(this);
}

Game::~Game()
{
    bgfx::destroy(m_FBO);
    bgfx::destroy(m_FBOTextureHandle);

    for (fw::GameObject* pObject : m_Objects)
    {
        delete pObject;
    }

    delete m_pCamera;
    delete m_pController;

    //delete m_pPhysicsWorld;

    for (auto& meshPair : m_pMeshes)
    {
        delete meshPair.second;
    }

    for (auto& materialPair : m_pMaterials)
    {
        delete materialPair.second;
    }

    for (auto& texturePair : m_pTextures)
    {
        delete texturePair.second;
    }

    for (auto& shaderPair : m_pShaders)
    {
        delete shaderPair.second;
    }

    delete m_pUniforms;

    delete m_pEventManager;
    delete m_pImGuiManager;
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame(deltaTime);

    //// Reset the controller.
    //m_pController->StartFrame();
    m_ActiveScene->StartFrame(deltaTime);
    // Dispatch events.
    m_pEventManager->DispatchAllEvents(this);
}

void Game::OnEvent(fw::Event* pEvent)
{
    // Process events.
    //m_pController->OnEvent(pEvent);

    //m_pPhysicsScene->OnEvent(pEvent);
    m_ActiveScene->OnEvent(pEvent);

    //// Remove object from GameObject list and delete it.
    //if (pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType())
    //{
    //    RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
    //    fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

    //    auto it = std::find(m_Objects.begin(), m_Objects.end(), pObject);
    //    m_Objects.erase(it);

    //    delete pObject;
    //}

    //// Set the new aspect ratio in the camera.
    //if (pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType())
    //{
    //    int width = m_FWCore.GetWindowClientWidth();
    //    int height = m_FWCore.GetWindowClientHeight();

    //    m_pCamera->SetAspectRatio((float)width / height);
    //}

    // Pass "WM_CHAR" events to imgui to handle text input.
    if (pEvent->GetType() == fw::OnCharEvent::GetStaticEventType())
    {
        int character = static_cast<fw::OnCharEvent*>(pEvent)->GetValue();
        m_pImGuiManager->AddInputCharacter(character);
    }

    //if (pEvent->GetType() == fw::ContactEvent::GetStaticEventType())
    //{
    //    fw::ContactEvent* pContactEvent = static_cast<fw::ContactEvent*>(pEvent);
    //    if (pContactEvent->GetEventType() == fw::ContactEvent::EventType::BeginContact)
    //        pContactEvent->BeginContact();
    //    else if (pContactEvent->GetEventType() == fw::ContactEvent::EventType::EndContact)
    //        pContactEvent->EndContact();
    //}

}

void Game::Update(float deltaTime)
{
    //m_pPhysicsScene->Update(deltaTime);
    m_ActiveScene->Update(deltaTime);

    //ImGui::ShowDemoWindow();




    //m_pCamera->Update(deltaTime);



    m_Elapsed += deltaTime;

    
    Editor_CreateMainFrame();
    Editor_DisplayObjectList();
    Editor_DrawGameView();
    Editor_DisplayMainMenu();
    Editor_DrawInspector();
}

void Game::Draw()
{
    //m_pPhysicsScene->Draw(EditorView_Game);
    m_ActiveScene->Draw(EditorView_Game);

    int viewID = 0;

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform(m_pUniforms->GetUniform("u_Time"), &time);

    //// Program the view and proj uniforms from the camera.
    //m_pCamera->Enable(viewID);

    ////Draw Objects
    //m_pComponentManager->Draw();

    m_pImGuiManager->EndFrame();
}


void Game::Editor_DisplayObjectList()
{
    ImGui::Begin("Object List");

    for (fw::GameObject* pObject : m_Objects)
    {
        ImGui::Text("%s", pObject->GetName().c_str());
    }

    //m_pPhysicsScene->Editor_DisplayObjectList();
    m_ActiveScene->Editor_DisplayObjectList();

    ImGui::End(); // "Object List"
}

void Game::Editor_DrawGameView()
{
    int viewID = 0;
    if(ImGui::Begin("Grab the Tab"))
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 size = contentMax - contentMin;

        if (size.x <= 0)
            size.x = 1;
        if (size.y <= 0)
            size.y = 1;

        m_GameWindowSize.Set(size.x, size.y);

        bgfx::setViewRect(EditorView_Game, 0, 0, size.x, size.y);
        bgfx::setViewRect(EditorView_Box2DDebugDraw, 0, 0, size.x, size.y);

        vec2 uv0 = vec2(0, 1);
        vec2 uv1 = vec2(size.x/4096.0f, 1 - size.y/4096.0f);
        ImGui::Image(fw::imguiTexture(m_FBOTextureHandle), ImVec2(size.x, size.y), uv0, uv1);
    }

    ImGui::End();
}

void Game::Editor_CreateMainFrame()
{
    // Setup a main window with no frame and a dockspace that covers the entire viewport.
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    if (ImGui::Begin("Main Dock", nullptr, flags))
    {
        ImGuiID dockspaceID = ImGui::GetID("My Dockspace");
        ImGui::DockSpace(dockspaceID);
    }
    ImGui::End();
}

void Game::Editor_DisplayMainMenu()
{
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("heh"))
        {
            int bp = 1; //just do something
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void Game::Editor_DrawInspector()
{
    if(ImGui::Begin("Inspector"))
    {
        if (m_pSelectedGameObject != nullptr)
            m_pSelectedGameObject->AddtoInspector();
    }
    ImGui::End();
}

void Game::SaveScene()
{
    nlohmann::json json;

    m_pPhysicsScene->SaveToJSON(json);

    std::string jsonString = json.dump(4);
    SaveCompleteFile("Data/Scenes/Test.scene", jsonString.c_str());

}

void Game::LoadScene()
{
    nlohmann::json json;

    m_pPhysicsScene->LoadFromJSON(json);

    // Inside Scene::LoadFromJSON(nlohmann::json& json)
    //for( GameObject* pObject : m_pGameObjects )
    //{
    //    pObject->LoadFromJSON( json );
    //}

    // Inside GameObject::LoadFromJSON(nlohmann::json& json)
    //for( Component* pComponent : m_pComponents )
    //{
    //    pComponent->LoadFromJSON( json );
    //}

    // Inside Component::LoadFromJSON(nlohmann::json& json)
    //m_Position = vec3( json["Position"][0], json["Position"][1], json["Position"][2] );
}

void Game::SaveCompleteFile(const char* filename, const char* string)
{
    FILE* filehandle;
    errno_t error = fopen_s(&filehandle, filename, "wb");

    if (filehandle)
    {
        fwrite(string, strlen(string), 1, filehandle);
        fclose(filehandle);
    }
}
