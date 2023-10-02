#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Game.h"
#include "Scene/Scene.h"

class PhysicsScene :public fw::Scene
{
public:
    PhysicsScene(fw::GameCore* GameCore);
    virtual ~PhysicsScene();

    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(int viewID) override;

    virtual void Editor_DisplayObjectList() override;

protected:
    Player* m_pPlayer;
    

};