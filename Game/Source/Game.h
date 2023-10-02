//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Framework.h"
#include "DataTypes.h"

class Player;
class Controller;
class fw::ComponentManager;
class fw::Scene;
class fw::ResourceManager;

enum EditorViews
{
    EditorView_Game,
    EditorView_ImGui,
    EditorView_Box2DDebugDraw,
};


class Game : public fw::GameCore
{
public:
    enum State
    {
        State_Playing,
        State_Paused,
    };

public:
    Game(fw::FWCore& fwCore);
    virtual ~Game() override;

    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    fw::Box2DContactListener* GetContactListener() { return m_pContactListener; }
    fw::ResourceManager* GetResourceManager() { return m_pResourceManager; }
    fw::ComponentManager* GetComponentManager() { return m_pComponentManager; }


    float GetElapsedTime() { return m_Elapsed; }

    vec2 GetGameWindowSize() { return m_GameWindowSize; }

    void SetSelectedGameObject(fw::GameObject* pObject) { m_pSelectedGameObject = pObject; }
    fw::GameObject* GetSelectedGameObject() { return m_pSelectedGameObject; }

    State GetState() { return m_State; }

protected:
    // Interface.
    fw::ImGuiManager* m_pImGuiManager = nullptr;
    void Editor_DisplayObjectList();

    // Resources.
    std::map<std::string, fw::Mesh*> m_pMeshes;
    std::map<std::string, fw::ShaderProgram*> m_pShaders;
    std::map<std::string, fw::Texture*> m_pTextures;
    std::map<std::string, fw::Material*> m_pMaterials;

    fw::ResourceManager* m_pResourceManager;

    // GameObjects and Helpers.
    Controller* m_pController = nullptr;

    fw::Camera* m_pCamera = nullptr;
    Player* m_pPlayer = nullptr;
    std::vector<fw::GameObject*> m_Objects;

    fw::Box2DContactListener* m_pContactListener;

    fw::Scene* m_pPhysicsScene;
    fw::Scene* m_pMarioKartScene;
    fw::Scene* m_pHeightMapScene;
    fw::Scene* m_ActiveScene;

    //ImGui window size
    vec2 m_GameWindowSize;

private: //trash
    float m_Elapsed = 0.0f;
    float m_TimeSinceLastPhysicsStep = 0.0f;

    State m_State;

protected: //FBO stuff
    bgfx::TextureHandle m_FBOTextureHandle;
    bgfx::FrameBufferHandle m_FBO;

    bgfx::TextureHandle m_DepthHandle;

    void Editor_DrawGameView();
    void Editor_CreateMainFrame();
    void Editor_DisplayMainMenu();
    void Editor_DrawInspector();

    void SaveScene();
    void LoadScene();
    void SaveCompleteFile(const char* filename, const char* string);

    fw::GameObject* m_pSelectedGameObject = nullptr;
    
    fw::ComponentManager* m_pComponentManager;
};
