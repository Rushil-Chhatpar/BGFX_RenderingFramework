#pragma once


#include "DataTypes.h"

class Controller;
class Game;


namespace fw
{
    class GameCore;
    class Event;
    class GameObject;
    class Camera;
    class ComponentManager;

    class Scene
    {
    public:
        Scene(GameCore* GameCore);
        virtual ~Scene();

        virtual void StartFrame(float deltaTime);
        virtual void OnEvent(Event* pEvent);
        virtual void Update(float deltaTime);
        virtual void Draw(int viewID);

        virtual void Editor_DisplayObjectList();

        void SaveToJSON(nlohmann::json& json);
        void LoadFromJSON(nlohmann::json& json);
        
    protected:
        ComponentManager* m_pComponentManager;

        Controller* m_pController = nullptr;

        Camera* m_pCamera = nullptr;

        std::vector<GameObject*> m_Objects;

        b2World* m_pPhysicsWorld = nullptr;

        void EnableGameObject(GameObject* pObject);
        void DisableGameObject(GameObject* pObject);

        float m_TimeSinceLastPhysicsStep = 0.0f;

        fw::GameCore* m_pGameCore;
        Game* m_pGame;
    };
}