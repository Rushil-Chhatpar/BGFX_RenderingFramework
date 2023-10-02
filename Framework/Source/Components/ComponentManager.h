#pragma once

namespace fw
{
    class Component;
    class ComponentManager
    {
    public:
        ComponentManager();
        ~ComponentManager();

        void AddComponent(Component* pComponent);

        void RemoveComponent(Component* pComponent);

        void UpdateTransform();

        void SyncTransform();

        void Draw();

        void UpdatePhysicsMovement(float deltaTime);

        void OnCoinPickup(float deltaTime);

        void UpdateforCollision(float deltaTime);

        void AddtoManager(Component* pComponent);

        void RemovefromManager(Component* pComponent);

        void UpdateOrbitCameraView(float deltaTime);

    protected:
        std::map<const char*, std::vector<Component*>> m_Components;
    };
}