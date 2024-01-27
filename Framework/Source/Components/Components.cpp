#include "Components.h"
#include "Framework.h"
#include "../../Game/Source/Objects/Controller.h"

void fw::Transform::AddtoInspector()
{
    ImGui::DragFloat3("Position", &m_Position.x);
    ImGui::DragFloat3("Rotation", &m_Rotation.x);
    ImGui::DragFloat3("Scale", &m_Scale.x);
}

void fw::Transform::SaveToJSON(nlohmann::json& json)
{
    json["Position"] = { m_Position.x, m_Position.y, m_Position.z };
    json["Rotation"] = { m_Rotation.x, m_Rotation.y, m_Rotation.z };
    json["Scale"] = { m_Scale.x, m_Scale.y, m_Scale.z };
}

void fw::Transform::LoadFromJSON(nlohmann::json& json)
{
    m_Position = vec3(json["Position"][0], json["Position"][1], json["Position"][2]);
    m_Rotation = vec3(json["Rotation"][0], json["Rotation"][1], json["Rotation"][2]);
    m_Scale = vec3(json["Scale"][0], json["Scale"][1], json["Scale"][2]);
}

void fw::Transform::UpdateTransform()
{
    m_TransformMatrix.CreateSRT(m_Scale, m_Rotation, m_Position);
}

void fw::Physics::AddtoInspector()
{
    float pos[2] = { m_pBody->GetPosition().x, m_pBody->GetPosition().y };
    ImGui::DragFloat2("Phy Pos", &pos[0]);
}

void fw::Physics::SaveToJSON(nlohmann::json& json)
{
}

void fw::Physics::LoadFromJSON(nlohmann::json& json)
{
}

b2Body* fw::Physics::CreateBody(b2World* pWorld, b2BodyType type, float density)
{
    Transform* transform = m_pGameObject->GetComponent<Transform>();
    if (transform == nullptr)
        return nullptr;

    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position.Set(transform->m_Position.x, transform->m_Position.z);
    float rot = transform->m_Rotation.y;
    rot = degreesToRads(rot);
    bodyDef.angle = rot;
    bodyDef.userData.pointer = (uintptr_t)m_pGameObject;

    b2Body* pBody = pWorld->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(transform->m_Scale.x / 2, transform->m_Scale.y / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = density;
    fixtureDef.friction = 0.3f;

    pBody->CreateFixture(&fixtureDef);

    

    return pBody;
}

void fw::Physics::SyncTransform()
{
    if (m_pBody->IsEnabled() == false)
        return;

    b2Vec2 pos = m_pBody->GetPosition();
    m_pGameObject->GetComponent<Transform>()->m_Position.x = pos.x;
    m_pGameObject->GetComponent<Transform>()->m_Position.z = pos.y;
    float rot = m_pBody->GetAngle();
    rot = radsToDegrees(rot);
    m_pGameObject->GetComponent<Transform>()->m_Rotation.y = -rot;
}

void fw::Render::AddtoInspector()
{
}

void fw::Render::SaveToJSON(nlohmann::json& json)
{
}

void fw::Render::LoadFromJSON(nlohmann::json& json)
{
}

void fw::Render::Draw()
{
    Uniforms* pUniforms = m_pGameObject->GetGameCore()->GetUniforms();
    mat4 worldMatrix = m_pGameObject->GetComponent<Transform>()->GetTransformMatrix();
    mat4 normalMatrix;
    normalMatrix.CreateRotation(m_pGameObject->GetComponent<Transform>()->m_Rotation);

    bgfx::setUniform(pUniforms->GetUniform("u_WorldMatrix"), &worldMatrix);
    bgfx::setUniform(pUniforms->GetUniform("u_NormalMatrix"), &normalMatrix);
    m_pMesh->Draw(0, pUniforms, m_pMaterial);
}

void fw::CameraComponent::AddtoInspector()
{
}

void fw::CameraComponent::SaveToJSON(nlohmann::json& json)
{
}

void fw::CameraComponent::LoadFromJSON(nlohmann::json& json)
{
}

void fw::CameraComponent::EnableCamera(int viewID)
{
    Uniforms* pUniforms = m_pGameObject->GetGameCore()->GetUniforms();

    vec2 scaleWithAspectRatio = m_ProjectionScale * vec2(1.0f / m_AspectRatio, 1);

    mat4 projMatrix;
    projMatrix.CreatePerspectiveVFoV(45, m_AspectRatio, 0.01f, 250.0f);

    bgfx::setUniform(pUniforms->GetUniform("u_ProjMatrix"), &projMatrix);
    bgfx::setUniform(pUniforms->GetUniform("u_ProjectionScale"), &scaleWithAspectRatio, 1);
}

void fw::FollowComponent::AddtoInspector()
{
}

void fw::FollowComponent::SaveToJSON(nlohmann::json& json)
{
}

void fw::FollowComponent::LoadFromJSON(nlohmann::json& json)
{
}

void fw::FollowComponent::SetViewMatrix()
{
    vec3 eye;
    vec3 forward = m_pGameObject->GetForwardVector();
    
    eye = m_pGameObject->GetPosition() - (forward * m_TargetArmLength);
    eye += vec3(0, m_yOffset, 0);
    vec3 up = forward + vec3(0, 1, 0);

    vec3 at = m_pGameObject->GetPosition();

    Uniforms* pUniforms = m_pGameObject->GetGameCore()->GetUniforms();

    bgfx::setUniform(pUniforms->GetUniform("u_CameraEyePosition"), &eye);

    mat4 viewMatrix;
    viewMatrix.CreateLookAtView(eye, up, at);
    bgfx::setUniform(pUniforms->GetUniform("u_ViewMatrix"), &viewMatrix);
}

void fw::MarioKartPhysicsComponent::AddtoInspector()
{
    ImGui::DragFloat("Speed", &m_Speed);
}

void fw::MarioKartPhysicsComponent::SaveToJSON(nlohmann::json& json)
{
}

void fw::MarioKartPhysicsComponent::LoadFromJSON(nlohmann::json& json)
{
}

void fw::MarioKartPhysicsComponent::UpdatePhysicsMovement(float deltaTime)
{
    b2Body* body = m_pGameObject->GetComponent<Physics>()->m_pBody;
    if (body == nullptr)
        return;

    float updir = 0.0f;

    float angleScalar = 0.0f;

    if (m_pController->IsHeld(Controller::Mask::Up))
        updir += 1;
    else if (m_pController->IsHeld(Controller::Mask::Down))
        updir -= 1;

    if (m_pController->IsHeld(Controller::Mask::Left))
        angleScalar -= 1;
    else if (m_pController->IsHeld(Controller::Mask::Right))
        angleScalar += 1;

    //FOR JAZ
    body->SetAngularDamping(10.0f);
    body->SetLinearDamping(2.0f);

    float targetVelocity = updir * m_Speed;
    float velocityDiff = targetVelocity;
    if (body->GetLinearVelocity().Length() <= targetVelocity)
        velocityDiff = targetVelocity - body->GetLinearVelocity().Length();

    float timeStep = 1 / 60.0f;

    vec3 fordir = m_pGameObject->GetForwardVector();

    vec2 forwardActual = vec2(fordir.x, fordir.z);

    float mass = body->GetMass();

    vec2 forceToApply = vec2(forwardActual.x, forwardActual.y).Normalize() * velocityDiff / timeStep;

    body->ApplyLinearImpulseToCenter(b2Vec2(forceToApply.x, forceToApply.y), true);

    body->ApplyAngularImpulse(m_AngularSpeed * deltaTime * angleScalar, true);
}

void fw::CoinComponent::AddtoInspector()
{
}

void fw::CoinComponent::SaveToJSON(nlohmann::json& json)
{
}

void fw::CoinComponent::LoadFromJSON(nlohmann::json& json)
{
}

void fw::CoinComponent::OnCoinPickup(float deltaTime)
{
    if (m_AnimationTimer > 0.0f)
    {
        Transform* transform = m_pGameObject->GetComponent<Transform>();
        Physics* physics = m_pGameObject->GetComponent<Physics>();
        if (transform != nullptr && physics != nullptr && m_pTarget!=nullptr)
        {
            m_AnimationTimer -= deltaTime;
            physics->m_pBody->SetEnabled(false);
            transform->m_Position = (m_pTarget->GetPosition() + vec3(0, 1.0f, 0));
            transform->m_Rotation.y += deltaTime * 360.0f;
        }
    }
    if (m_AnimationTimer < 0.0f)
        m_pGameObject->Disable();
}

void fw::CollisionReactionComponent::AddtoInspector()
{
}

void fw::CollisionReactionComponent::SaveToJSON(nlohmann::json& json)
{
}

void fw::CollisionReactionComponent::LoadFromJSON(nlohmann::json& json)
{
}

void fw::CollisionReactionComponent::BeginContact(GameObject* pObject)
{
    if (m_ObjectType == ObjectType::Coin)
        CoinReaction(pObject);
    else if (m_ObjectType == ObjectType::Oil)
        OilReaction(pObject);
    else if (m_ObjectType == ObjectType::SpeedBoost)
        SpeedBoostReaction(pObject);
}

void fw::CollisionReactionComponent::UpdateforCollision(float deltaTime)
{
    if(m_ObjectType != ObjectType::Coin)
    {
        b2Body* body = m_pGameObject->GetComponent<Physics>()->m_pBody;
        if(body!=nullptr)
        {
            if(body->IsEnabled() == false)
            {
                m_Elapsed += deltaTime;
                if (m_Elapsed > 2.0f)
                {
                    body->SetEnabled(true);
                    m_Elapsed = 0.0f;
                }
            }
        }
    }
}


void fw::CollisionReactionComponent::CoinReaction(GameObject* pObject)
{
    //if the coin has no coin component, return
    CoinComponent* coinComponent = m_pGameObject->GetComponent<CoinComponent>();
    if (coinComponent == nullptr)
        return;

    MarioKartPhysicsComponent* kartPhysics = pObject->GetComponent<MarioKartPhysicsComponent>();
    //return if its not a player
    if (kartPhysics == nullptr)
        return;

    //resets the animation timer to 1.0f
    coinComponent->PlayAnimationTimer();

    //set the player as a target. the coin will spin on top of the player for the duration of the animation timer ^
    coinComponent->SetTarget(pObject);

    CoinCountComponent* coinCount = pObject->GetComponent<CoinCountComponent>();
    if (coinCount != nullptr)
    {
        coinCount->IncrementCoinCount();
    }

    if (kartPhysics != nullptr)
    {
        float speed = kartPhysics->GetSpeed();
        kartPhysics->SetSpeed(speed + (speed * 0.01));
    }
}

void fw::CollisionReactionComponent::OilReaction(GameObject* pObject)
{
    //pObject is the GameObject that has collided with the oil. That means the player
    //if the player does not have a physics component, return. Safety check for future proofing
    if (pObject->GetComponent<Physics>() == nullptr)
        return;

    //gets the physics body of the player
    b2Body* PlayerBody = pObject->GetComponent<Physics>()->m_pBody;

    float AngularSpeed = 100.0f;
    //Use PlayerBody to apply angular impulse. Refer fw::MarioKartPhysicsComponent::UpdatePhysicsMovement(float deltaTime) written above on how to apply angular impulse
    //Before applying angular impulse, disable the speed ramp using: this->Disable(), so that the player doesnt collide
    PlayerBody->ApplyAngularImpulse(AngularSpeed, true);


    //BONUS: find a way to disable the PlayerBody for 1 second or more, to make the oil spill permenant(for the duration of the game)
    //HOW TO DISABLE PlayerBody: PlayerBody->SetEnabled(false); EZ
    //And then enable again after 1 second
    b2Body* OilBody = m_pGameObject->GetComponent<Physics>()->m_pBody;

    OilBody->SetEnabled(false);

    OilBody->SetLinearVelocity(b2Vec2(0, 0));

    OilBody->SetAngularVelocity(0.0f);


}

void fw::CollisionReactionComponent::SpeedBoostReaction(GameObject* pObject)
{
    //pObject is the GameObject that has collided with the oil. That means the player
    //if the player does not have a physics component, return. Safety check for future proofing
    if (pObject->GetComponent<Physics>() == nullptr)
        return;

    //gets the physics body of the player
    b2Body* PlayerBody = pObject->GetComponent<Physics>()->m_pBody;

    //We now have 2 objects to play with. One is this(speed boost) and the other one is pObject(player)
    //There is a function in GameObject.h/.cpp that gets the forward vector(the direction in which the object is facing) of the object
    //Remember Brad's slides in gng1? Remember Tech Math in sem 1? If no, then study the content LMAO
    //Store the forward vector of both the objects in 2 vec3 variables, say forwardV_1 and forwardV_2, awesome naming convention
    //find the dot product between the 2 vec3. float dotproduct = forwardV_1.Dot(forwardV_2), something like that. look it up
    //The forward vector ranges from 0 to 1. It is 0 if the 2 vec3 are perpendicular and 1 if they are parallel. Use the dot product to apply the amount of boost
    //It means that if you go straight through the speed boost ramp, you get the maximum speed boost. But if you go through it at an angle, the speed boost is decreased

    vec3 PlayerForwardVector = pObject->GetForwardVector().Normalize();
    
    vec3 BoostForwardVector = m_pGameObject->GetForwardVector().Normalize();

    float DotProduct = PlayerForwardVector.Dot(BoostForwardVector);

    if (DotProduct < 0)
    {
        DotProduct *= -1;
    }

    vec2 ForceToApply = PlayerBody->GetLinearVelocity().Length() * DotProduct * vec2(PlayerForwardVector.x, PlayerForwardVector.z);

    PlayerBody->ApplyLinearImpulseToCenter(b2Vec2(ForceToApply.x, ForceToApply.y), true);

    b2Body* BoostBody = m_pGameObject->GetComponent<Physics>()->m_pBody;

    BoostBody->SetEnabled(false);

    BoostBody->SetLinearVelocity(b2Vec2(0, 0));

    BoostBody->SetAngularVelocity(0.0f);

}

void fw::CoinCountComponent::AddtoInspector()
{
    ImGui::DragInt("Coin Count", &m_CoinCount);
}

void fw::CoinCountComponent::SaveToJSON(nlohmann::json& json)
{
}

void fw::CoinCountComponent::LoadFromJSON(nlohmann::json& json)
{
}

//void fw::HeighMapComponent::AddtoInspector()
//{
//}
//
//void fw::HeighMapComponent::SaveToJSON(nlohmann::json& json)
//{
//}
//
//void fw::HeighMapComponent::LoadFromJSON(nlohmann::json& json)
//{
//}
//
//void fw::HeighMapComponent::CreateHeightMap()
//{
//    Transform* transform = m_pGameObject->GetComponent<Transform>();
//
//    vec3 startPos = transform->m_Position;
//    m_Verts = new VertexFormat_Pos3dUV[m_VertScale.x * m_VertScale.y];
//    float xOffset = m_WorldScale.x / m_VertScale.x;
//    float yOffset = m_WorldScale.y / m_VertScale.y;
//    for (int j = 0; j < m_VertScale.y; j++)
//    {
//        for (int i = 0; i < m_VertScale.x; i++)
//        {
//            float xpos = startPos.x + (i * xOffset);
//            float ypos = startPos.y + (j * yOffset);
//            m_Verts[i + (j * m_VertScale.x)] = { vec3(xpos, 0, ypos), vec2(1.0f/m_VertScale.x * i,1.0f / m_VertScale.y * j) };
//        }
//    }
//
//    
//    for (int j = 0; j < m_VertScale.y - 1; j++)
//    {
//        for (int i = 0; i < m_VertScale.x - 1; i++)
//        {
//
//        }
//    }
//}
//
//void fw::HeighMapComponent::Draw()
//{
//}
