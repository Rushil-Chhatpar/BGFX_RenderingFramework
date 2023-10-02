//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"
#include "CoreHeaders.h"

class Game;
namespace fw
{

    class Camera;
    class GameCore;
    class Material;
    class Mesh;
    class Uniforms;
    class Component;
    class Transform;

    class GameObject
    {
    public:
        GameObject(GameCore* pGameCore, std::string name);
        virtual ~GameObject();

        virtual void Update(float deltaTime);
        //virtual void Draw(Camera* pCamera);

        // Getters.
        std::string GetName() { return m_Name; }


        b2Body* CreateBody(b2World* pWorld, b2BodyType type);

        vec3 GetPosition();
        vec3 GetRotation();
        vec3 GetScale();

        // Setters.
        void SetPosition(vec3 pos);
        void SetRotation(vec3 rot);
        void SetScale(vec3 scale);

        GameCore* GetGameCore() { return m_pGameCore; }



        //COMPONENTS STUFF

        template<typename Type> Type* GetComponent()
        {
            for (Component* component : m_Components)
            {
                if (component->GetName() == Type::GetStaticType())
                {
                    return static_cast<Type*>(component);
                }
            }
            return nullptr;
        }

        virtual void BeginContact(GameObject* contact);
        virtual void EndContact(GameObject* contact);

        void AddComponent(Component* pComponent);
        void RemoveComponent(Component* pComponent);

        void SetActive(bool isActive) { m_IsActive = isActive; }
        bool IsActive() { return m_IsActive; }

        void AddtoInspector();

        vec3 GetForwardVector();

        void SaveToJSON(nlohmann::json& json);
        void LoadFromJSON(nlohmann::json& json);

        void Disable();
        void Enable();

    protected:
        GameCore* m_pGameCore = nullptr;

        std::string m_Name;

        std::vector<Component*> m_Components;

        bool m_IsActive;
    };



} // namespace fw
