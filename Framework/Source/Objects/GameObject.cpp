//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"
#include "../../Game/Source/Game.h"


namespace fw {

GameObject::GameObject(GameCore* pGameCore, std::string name)
    : m_pGameCore( pGameCore )
    , m_Name( name )
    , m_IsActive(true)
    //, m_Position( pos )
    //, m_pMesh( pMesh )
    //, m_pMaterial( pMaterial )
{
    //m_Components.push_back(new Render(this, m_pMesh, m_pMaterial));
    //m_Components.push_back(new Transform(this, m_Position, m_Scale, m_Rotation));
}

GameObject::~GameObject()
{
    //if( m_pBody )
    //{
    //    m_pGameCore->GetPhysicsWorld()->DestroyBody( m_pBody );
    //}
}

void GameObject::Update(float deltaTime)
{
    //if( m_pBody )
    //{
    //    b2Vec2 pos = m_pBody->GetPosition();
    //    m_Position.Set( pos.x, pos.y, 0 );
    //    float rot = m_pBody->GetAngle();
    //    m_Rotation.Set(0, 0, -rot);
    //}
}

//void GameObject::Draw(Camera* pCamera)
//{
//    Uniforms* pUniforms = m_pGameCore->GetUniforms();
//
//    Render* render = nullptr;
//    Transform* transform = nullptr;
//
//    for (auto components : m_Components)
//    {
//        if (render == nullptr)
//            render = dynamic_cast<Render*>(components);
//        if (transform == nullptr)
//            transform = dynamic_cast<Transform*>(components);
//    }
//
//    mat4 worldMatrix;
//    //worldMatrix.CreateSRT( transform->m_Scale, transform->m_Rotation, transform->m_Position);
//    worldMatrix.CreateSRT(m_Scale, m_Rotation, m_Position);
//
//    bgfx::setUniform( pUniforms->GetUniform("u_WorldMatrix"), &worldMatrix );
//    //bgfx::setUniform( pUniforms->GetUniform("u_Scale"), &m_Scale.x );
//    //bgfx::setUniform( pUniforms->GetUniform("u_Rotation"), &m_Rotation.x );
//    //bgfx::setUniform( pUniforms->GetUniform("u_Position"), &m_Position.x );
//
//    render->m_pMesh->Draw(0, pUniforms, render->m_pMaterial);
//    //m_pMesh->Draw( 0, pUniforms, m_pMaterial );
//}

b2Body* GameObject::CreateBody(b2World* pWorld, b2BodyType type)
{
    Transform* transform = GetComponent<Transform>();
    if (transform == nullptr)
        return nullptr;

    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position.Set( transform->m_Position.x, transform->m_Position.y );
    bodyDef.userData.pointer = (uintptr_t)this;

    b2Body* pBody = pWorld->CreateBody( &bodyDef );

    b2PolygonShape boxShape;
    boxShape.SetAsBox(transform->m_Scale.x / 2, transform->m_Scale.y/ 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    pBody->CreateFixture(&fixtureDef);

    return pBody;
}

vec3 GameObject::GetPosition()
{
    return GetComponent<Transform>()->m_Position;
}

vec3 GameObject::GetRotation()
{
    return GetComponent<Transform>()->m_Rotation;
}

vec3 GameObject::GetScale()
{
    return GetComponent<Transform>()->m_Scale;
}

void GameObject::SetPosition(vec3 pos)
{
    if (GetComponent<Transform>() != nullptr)
        GetComponent<Transform>()->m_Position = pos;
}

void GameObject::SetRotation(vec3 rot)
{
    if (GetComponent<Transform>() != nullptr)
        GetComponent<Transform>()->m_Rotation = rot;
}

void GameObject::SetScale(vec3 scale)
{
    if (GetComponent<Transform>() != nullptr)
        GetComponent<Transform>()->m_Scale = scale;
}

void GameObject::BeginContact(GameObject* contact)
{
    CollisionReactionComponent* collisionReaction = GetComponent<CollisionReactionComponent>();
    if (collisionReaction == nullptr)
        return;

    if (contact != nullptr)
        collisionReaction->BeginContact(contact);
}

void GameObject::EndContact(GameObject* contact)
{

}

void GameObject::AddComponent(Component* pComponent)
{
    m_Components.push_back(pComponent);
}

void GameObject::RemoveComponent(Component* pComponent)
{    //    m_pGameCore->GetPhysicsWorld()->DestroyBody( m_pBody );
    if (pComponent->GetName() == Physics::GetStaticType())
    {
        Physics* component = static_cast<Physics*>(pComponent);
        m_pGameCore->GetPhysicsWorld()->DestroyBody(component->m_pBody);
    }

    for (int i = 0; i < m_Components.size(); i++)
    {
        if (m_Components[i] == pComponent)
        {
            m_Components.erase(m_Components.begin() + i);
        }
    }
}

void GameObject::AddtoInspector()
{
    for (Component* pComponent : m_Components)
    {
        pComponent->AddtoInspector();
    }
}

vec3 GameObject::GetForwardVector()
{
    return GetComponent<Transform>()->GetTransformMatrix().GetAt();
}

void GameObject::SaveToJSON(nlohmann::json& json)
{
    json["Components"] = nlohmann::json::array();

    for (Component* pComponent : m_Components)
    {
        nlohmann::json jsonComponent;
        pComponent->SaveToJSON(jsonComponent);
        json["Components"].push_back(jsonComponent);
    }
}

void GameObject::LoadFromJSON(nlohmann::json& json)
{
    for (Component* pComponent : m_Components)
    {
        pComponent->LoadFromJSON(json);
    }
}

void GameObject::Disable()
{
    if (m_IsActive == false)
        return;

    Game* pGame = static_cast<Game*>(m_pGameCore);
    for (Component* component : m_Components)
    {
        if (component->GetName() == Physics::GetStaticType())
        {
            GetComponent<Physics>()->m_pBody->SetEnabled(false);
        }

        pGame->GetComponentManager()->RemovefromManager(component);
    }
}

void GameObject::Enable()
{
    if (m_IsActive == true)
        return;

    Game* pGame = static_cast<Game*>(m_pGameCore);
    for (Component* component : m_Components)
    {
        if (component->GetName() == Physics::GetStaticType())
        {
            GetComponent<Physics>()->m_pBody->SetEnabled(true);
        }

        pGame->GetComponentManager()->AddtoManager(component);
    }
}


} // namespace fw
