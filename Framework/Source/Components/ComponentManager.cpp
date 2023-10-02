#include "Framework.h"
#include "ComponentManager.h"
#include "Components/Components.h"
#include "../../Game/Source/GameComponents/GameComponents.h"


namespace fw
{
    fw::ComponentManager::ComponentManager()
    {
    }

    ComponentManager::~ComponentManager()
    {
        for (auto transforms : m_Components[Transform::GetStaticType()])
            delete transforms;
        for (auto renders : m_Components[Render::GetStaticType()])
            delete renders;
        for (auto bodies : m_Components[Physics::GetStaticType()])
            delete bodies;
    }

    void ComponentManager::AddComponent(Component* pComponent)
    {
        m_Components[pComponent->GetName()].push_back(pComponent);
        pComponent->GetGameObject()->AddComponent(pComponent);
    }

    void ComponentManager::RemoveComponent(Component* pComponent)
    {
        for (int i = 0; i < m_Components[pComponent->GetName()].size(); i++)
        {
            if (m_Components[pComponent->GetName()].at(i) == pComponent)
            {
                m_Components[pComponent->GetName()].erase(m_Components[pComponent->GetName()].begin() + i); //why use variables when u can code in 1 line ;)
                pComponent->GetGameObject()->RemoveComponent(pComponent);
            }
        }
    }

    void ComponentManager::UpdateTransform()
    {
        for (auto transforms : m_Components[Transform::GetStaticType()])
        {
            Transform* transform = static_cast<Transform*>(transforms);
            transform->UpdateTransform();
        }
    }

    void ComponentManager::SyncTransform()
    {
        for (auto bodies : m_Components[Physics::GetStaticType()])
        {
            Physics* body = static_cast<Physics*>(bodies);
            body->SyncTransform();
        }
    }

    void ComponentManager::Draw()
    {
        for (auto renders : m_Components[Render::GetStaticType()])
        {
            Render* render = static_cast<Render*>(renders);
            render->Draw();
        }
    }

    void ComponentManager::UpdatePhysicsMovement(float deltaTime)
    {
        for (auto physicsBody : m_Components[MarioKartPhysicsComponent::GetStaticType()])
        {
            MarioKartPhysicsComponent* body = static_cast<MarioKartPhysicsComponent*>(physicsBody);
            body->UpdatePhysicsMovement(deltaTime);
        }
    }

    void ComponentManager::OnCoinPickup(float deltaTime)
    {
        for (auto coins : m_Components[CoinComponent::GetStaticType()])
        {
            CoinComponent* coin = static_cast<CoinComponent*>(coins);
            coin->OnCoinPickup(deltaTime);
        }
    }

    void ComponentManager::UpdateforCollision(float deltaTime)
    {
        for (auto collisions : m_Components[CollisionReactionComponent::GetStaticType()])
        {
            CollisionReactionComponent* collision = static_cast<CollisionReactionComponent*>(collisions);
            collision->UpdateforCollision(deltaTime);
        }
    }

    void ComponentManager::AddtoManager(Component* pComponent)
    {
        m_Components[pComponent->GetName()].push_back(pComponent);
    }

    void ComponentManager::RemovefromManager(Component* pComponent)
    {
        for (int i = 0; i < m_Components[pComponent->GetName()].size(); i++)
        {
            if (m_Components[pComponent->GetName()].at(i) == pComponent)
            {
                m_Components[pComponent->GetName()].erase(m_Components[pComponent->GetName()].begin() + i);
            }
        }
    }

    void ComponentManager::UpdateOrbitCameraView(float deltaTime)
    {
        for (auto cameras : m_Components[OrbitCameraComponent::GetStaticType()])
        {
            OrbitCameraComponent* camera = static_cast<OrbitCameraComponent*>(cameras);
            camera->UpdateCameraView(deltaTime);
        }
    }

}
