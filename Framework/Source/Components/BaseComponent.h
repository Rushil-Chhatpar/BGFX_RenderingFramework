#pragma once
#include "Framework.h"
#include "DataTypes.h"
#include "Objects/GameObject.h"

namespace fw
{
    class Component
    {
    public:
        Component(GameObject* pGameObject)
            :m_pGameObject(pGameObject)
        {}
        virtual ~Component() {}

        virtual const char* GetName() = 0;

        GameObject* GetGameObjectPointer() { return m_pGameObject; }

        GameObject* GetGameObject() { return m_pGameObject; }

        virtual void AddtoInspector() = 0;

        virtual void SaveToJSON(nlohmann::json& json) = 0;
        virtual void LoadFromJSON(nlohmann::json& json) = 0;

    protected:
        GameObject* m_pGameObject = nullptr;

    };
}