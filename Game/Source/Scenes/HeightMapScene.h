#pragma once

#include "DataTypes.h"
#include "Game.h"
#include "Scene/Scene.h"

class HeightMapScene : public fw::Scene
{
public:
    HeightMapScene(fw::GameCore* GameCore);
    virtual ~HeightMapScene();

    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(int viewID) override;

    virtual void Editor_DisplayObjectList() override;

    void SetupLightUniforms();

protected:
    fw::GameObject* m_FollowCamera;
    fw::GameObject* m_PlayerObj;
    std::vector<fw::GameObject*> m_Lights;
};