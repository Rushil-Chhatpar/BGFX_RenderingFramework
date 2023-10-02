#pragma once

#include "DataTypes.h"
#include "Game.h"
#include "Scene/Scene.h"

class MarioKartScene : public fw::Scene
{
public:
    MarioKartScene(fw::GameCore* GameCore);
    virtual ~MarioKartScene();

    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(int viewID) override;

    virtual void Editor_DisplayObjectList() override;

    void PowerupPos();

protected:

    struct MarioKartTransform
    {
        vec3 pos;
        vec3 scale;
        vec3 rotate;

        MarioKartTransform(vec3 newPos, vec3 newScale, vec3 newRotate)
        {
            pos = vec3(newPos.x, newPos.y, newPos.z);
            scale = vec3(newScale.x, newScale.y, newScale.z);
            rotate = vec3(newRotate.x, newRotate.y, newRotate.z);
        }
    };

    Player* m_pPlayer;
    fw::GameObject* test;
    fw::GameObject* m_pFollowCamera;
    fw::GameObject* m_Mario;
    fw::GameObject* m_Coin;
    fw::GameObject* m_OilSpill;
    fw::GameObject* m_SpeedBoost;
    fw::GameObject* m_Border;
    std::vector<MarioKartTransform> m_CoinTransform;
    std::vector<MarioKartTransform> m_OilTransform;
    std::vector<MarioKartTransform> m_SpeedTransform;
    std::vector<MarioKartTransform> m_BorderTransform;
    float Elapsed = 0.0f;


};