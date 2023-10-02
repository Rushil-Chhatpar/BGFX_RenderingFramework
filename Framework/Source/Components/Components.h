#pragma once
#include "BaseComponent.h"
#include "Math/Vector.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
//#include "../../Game/Source/Meshes/VertexFormats.h"

struct VertexFormat_Pos3dUV;
class Controller;
namespace fw
{
    /// Transform ====================================================================================
    class Transform : public Component
    {
        friend class ComponentManager;
    public:
        Transform(GameObject* pGameObject, vec3 pos, vec3 scale, vec3 rot)
            : m_Position(pos)
            , m_Scale(scale)
            , m_Rotation(rot)
            , Component(pGameObject)
        {}
        static const char* GetStaticType()              { return "Transform"; }
        virtual const char* GetName() override          { return GetStaticType(); }
        vec3 m_Position;
        vec3 m_Scale;
        vec3 m_Rotation;

        mat4 GetTransformMatrix()                       { return m_TransformMatrix; }

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;


    protected:
        void UpdateTransform();

        mat4 m_TransformMatrix;
    };

    
    /// Render =======================================================================================
    class Render : public Component
    {
        friend class ComponentManager;
    public:
        Render(GameObject* pGameObject, Mesh* pMesh, Material* pMaterial)
            : m_pMesh(pMesh)
            , m_pMaterial(pMaterial)
            , Component(pGameObject)
        {}
        static const char* GetStaticType()              { return "Render"; }
        virtual const char* GetName() override          { return GetStaticType(); }
        Mesh* m_pMesh;
        Material* m_pMaterial;

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

    protected:
        virtual void Draw();

    };

    /// Physics =======================================================================================
    class Physics : public Component
    {
        friend class ComponentManager;
    public:
        Physics(GameObject* pGameObject, b2World* pWorld, b2BodyType type, float density)
            : Component(pGameObject)
        {
            m_pBody = CreateBody(pWorld, type, density);
        }

        static const char* GetStaticType() { return "Physics"; }
        virtual const char* GetName() override { return GetStaticType(); }

        b2Body* m_pBody;

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;
        
        void SetBody(b2Body* body) { m_pBody = body; }

        b2Body* CreateBody(b2World* pWorld, b2BodyType type, float density);

    protected:
        void SyncTransform();


    };

    /// Camera =========================================================================================
    class CameraComponent : public Component
    {
        friend class ComponentManager;
    public:
        CameraComponent(GameObject* pGameObject)
            : Component(pGameObject)
            , m_AspectRatio(1.0f)
            , m_ProjectionScale(0.1f, 0.1f)
            , m_Position(0,0,0)
        {}

        static const char* GetStaticType() { return "CameraComponent"; }
        virtual const char* GetName() override { return GetStaticType(); }

        void SetProjectionScale(vec2 scale) { m_ProjectionScale = scale; }
        void SetAspectRatio(float32 aspectRatio) { m_AspectRatio = aspectRatio; }

        vec2 m_ProjectionScale;
        float32 m_AspectRatio;
        vec3 m_Position;

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

        void EnableCamera(int viewID);
    protected:

    };

    /// Follow ========================================================================================
    class FollowComponent : public Component
    {
        friend class ComponentManager;
    public:
        FollowComponent(GameObject* pGameObject, float targetArmLength, float yOffset)
            : Component(pGameObject)
            , m_TargetArmLength(targetArmLength)
            , m_yOffset(yOffset)
        {}

        static const char* GetStaticType() { return "FollowComponent"; }
        virtual const char* GetName() override { return GetStaticType(); }

        void SetFollowObject(GameObject* pObject) { m_FollowObject = pObject; }

        float m_TargetArmLength;
        float m_yOffset;

        GameObject* m_FollowObject = nullptr;

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

        void SetViewMatrix();

    protected:

    };

    /// MarioKart Physics Component
    
    class MarioKartPhysicsComponent : public Component
    {
        friend class ComponentManager;
    public:
        MarioKartPhysicsComponent(GameObject* pGameObject, Controller* pController)
            : Component(pGameObject)
            , m_pController(pController)
        {}

        static const char* GetStaticType() { return "MarioKartPhysicsComponent"; }
        virtual const char* GetName() override { return GetStaticType(); }

        float GetSpeed() { return m_Speed; }

        void SetSpeed(float speed) { m_Speed = speed; }
        void SetAngularSpeed(float speed) { m_AngularSpeed = speed; }

        Controller* m_pController;

        float m_Speed = 0.0f;
        float m_AngularSpeed = 0.0f;

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

        void UpdatePhysicsMovement(float deltaTime);
    protected:
    };

    /// Coin Component
    class CoinComponent : public Component
    {
        friend class ComponentManager;
    public:
        CoinComponent(GameObject* pGameObject)
            : Component(pGameObject)
            , m_AnimationTimer(0.0f)
            , m_pTarget(nullptr)
        {}

        static const char* GetStaticType() { return "CoinComponent"; }
        virtual const char* GetName() override { return GetStaticType(); }

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;


        void PlayAnimationTimer() { m_AnimationTimer = 1.0f; }

        bool ShouldPlayAnimation() { return m_AnimationTimer > 0.0f; }

        void SetTarget(GameObject* pObject) { m_pTarget = pObject; }

    protected:
        void OnCoinPickup(float deltaTime);
        float m_AnimationTimer;
        GameObject* m_pTarget;
    };

    /// Collision Reaction Component
    class CollisionReactionComponent : public Component
    {
        friend class ComponentManager;
    public:
        enum class ObjectType
        {
            Coin,
            Oil,
            SpeedBoost,
        };
        CollisionReactionComponent(GameObject* pGameObject, ObjectType objectType)
            : Component(pGameObject)
            , m_ObjectType(objectType)
        {}

        static const char* GetStaticType() { return "CollisionReactionComponent"; }
        virtual const char* GetName() override { return GetStaticType(); }

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

        ObjectType GetObjectType() { return m_ObjectType; }
        void SetObjectType(ObjectType objectType) { m_ObjectType = objectType; }

        void BeginContact(GameObject* pObject);

        void UpdateforCollision(float deltaTime);
        
        void CoinReaction(GameObject* pObject);
        void OilReaction(GameObject* pObject);
        void SpeedBoostReaction(GameObject* pObject);

    protected:
        ObjectType m_ObjectType;
    private:
        float m_Elapsed = 0.0f;
    };

    /// Coin Count Component
    class CoinCountComponent : public Component
    {
        friend class ComponentManager;
    public:
        CoinCountComponent(GameObject* pGameObject)
            : Component(pGameObject)
            , m_CoinCount(0)
        {}

        static const char* GetStaticType() { return "CoinCountComponent"; }
        virtual const char* GetName() override { return GetStaticType(); }

        virtual void AddtoInspector() override;

        virtual void SaveToJSON(nlohmann::json& json) override;
        virtual void LoadFromJSON(nlohmann::json& json) override;

        void IncrementCoinCount() { m_CoinCount++; }

    protected:
        int m_CoinCount;


    };

    /// Height Map Component

    //class HeighMapComponent : public Render
    //{
    //    friend class ComponentManager;
    //public:
    //    HeighMapComponent(GameObject* pGameObject, Mesh* pMesh, Material* pMaterial, ivec2 vertScale, vec2 worldScale)
    //        : Render(pGameObject, pMesh, pMaterial)
    //    {}

    //    //static const char* GetStaticType() { return "HeightMapComponent"; }
    //    //virtual const char* GetName() override { return GetStaticType(); }

    //    virtual void AddtoInspector() override;

    //    virtual void SaveToJSON(nlohmann::json& json) override;
    //    virtual void LoadFromJSON(nlohmann::json& json) override;

    //    void CreateHeightMap();

    //protected:

    //    virtual void Draw() override;

    //    ivec2 m_VertScale;
    //    vec2 m_WorldScale;
    //    VertexFormat_Pos3dUV* m_Verts;
    //};

}