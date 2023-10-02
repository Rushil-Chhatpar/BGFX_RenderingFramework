#pragma once
#include "Math/Vector.h"
//#include "DataTypes.h"
#include "../../Game/Source/DataTypes.h"
#include "Components/Components.h"
//#include "Meshes/VertexFormats.h"
#include "../../Game/Source/Meshes/VertexFormats.h"

class Controller;

class HeightMapComponent : public fw::Render
{
    friend class ComponentManager;
public:
    HeightMapComponent(fw::GameObject* pGameObject, vec2 worldScale, fw::Mesh* pMesh, fw::Material* pMaterial)
        : Render(pGameObject, pMesh, pMaterial)
        , m_WorldScale(worldScale)
    {}

    virtual void CreateHeightMap(const char* filename);

    virtual void Rebuild();

    virtual float GetHeightAtXZ(float x, float z);

    virtual ivec2 GetVertIndexAtXZ(float x, float z);

    ivec2 m_VertScale;
    vec2 m_WorldScale;
    VertexFormat_Pos3dUVNormal* m_Verts;
    uint16* g_Indices;
};

class PlayerLogicComponent : public fw::Component
{
    friend class ComponentManager;
public:
    PlayerLogicComponent(fw::GameObject* pGameObject, Controller* pController)
        : fw::Component(pGameObject)
        , m_pController(pController)
    {}

    static const char* GetStaticType() { return "PlayerLogicComponent"; }
    virtual const char* GetName() override { return GetStaticType(); }

    virtual void AddtoInspector() override;

    virtual void SaveToJSON(nlohmann::json& json) override;
    virtual void LoadFromJSON(nlohmann::json& json) override;

    void Update(float deltaTime);

    void SetHeightMap(HeightMapComponent* heightMap)            { m_pHeightMap = heightMap; }

    void SetCamera(fw::GameObject* camera)                      { m_Camera = camera; }

protected:
    HeightMapComponent* m_pHeightMap = nullptr;
    Controller* m_pController;
    fw::GameObject* m_Camera;
};

class OrbitCameraComponent : public fw::Component
{
    friend class ComponentManager;
public:
    OrbitCameraComponent(fw::GameObject* pGameObject, Controller* pController, fw::GameObject* targetObject, float armLength, float yOffset)
        : fw::Component(pGameObject)
        , m_pController(pController)
        , m_TargetObject(targetObject)
        , m_TargetArmLength(armLength)
        , m_yOffset(yOffset)
        , m_Eye(0,0,0)
        , m_At(0, 0, 0)
        , m_Up(0, 0, 0)
    {}

    static const char* GetStaticType() { return "OrbitCameraComponent"; }
    virtual const char* GetName() override { return GetStaticType(); }

    virtual void AddtoInspector() override;

    virtual void SaveToJSON(nlohmann::json& json) override;
    virtual void LoadFromJSON(nlohmann::json& json) override;

    void UpdateCameraView(float deltaTime);

protected:
    Controller* m_pController;

    fw::GameObject* m_TargetObject;

    float m_TargetArmLength;
    float m_yOffset;

    vec3 m_Eye;
    vec3 m_At;
    vec3 m_Up;
};

class WaterComponent : public HeightMapComponent
{
    friend class ComponentManager;
public:
    WaterComponent(fw::GameObject* pGameObject, vec2 worldScale, fw::Mesh* pMesh, fw::Material* pMaterial)
        : HeightMapComponent(pGameObject, worldScale, pMesh, pMaterial)
    {}

    virtual void CreateWater(ivec2 vertScale);

};

class LightComponent : public fw::Component
{
    friend class ComponentManager;
public:
    LightComponent(fw::GameObject* pObject, vec4 color, float range)
        : fw::Component(pObject)
        , m_Color(color)
        , m_Range(range)
    {}

    static const char* GetStaticType() { return "LightComponent"; }
    virtual const char* GetName() override { return GetStaticType(); }

    virtual void AddtoInspector() override;

    virtual void SaveToJSON(nlohmann::json& json) override;
    virtual void LoadFromJSON(nlohmann::json& json) override;

    vec4 m_Color;
    float m_Range;
};