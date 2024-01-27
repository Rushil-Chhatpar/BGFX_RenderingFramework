//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Game.h"
#include "GameCore.h"
#include "Objects/Player.h"
#include "Objects/Controller.h"
#include "Components/Components.h"

Player::Player(fw::GameCore* pGameCore, Controller* pController, std::string name)
    : fw::GameObject( pGameCore, name )
    , m_pController( pController )
    , m_JumpAllowed(false)
{
    
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    GameObject::Update( deltaTime );
    
    fw::Transform* transform = GetComponent<fw::Transform>();
    if (transform == nullptr)
        return;

    float speed = 4.0f;

    vec2 dir;

    if( m_pController->IsHeld( Controller::Mask::Up ) )
    {
        dir.y += 1;
    }
    /*if( m_pController->IsHeld( Controller::Mask::Down ) )
    {
        dir.y += -1;
    }*/
    if( m_pController->IsHeld( Controller::Mask::Left ) )
    {
        dir.x += -1;
    }
    if( m_pController->IsHeld( Controller::Mask::Right ) )
    {
        dir.x += 1;
    }
    

    b2Body* body = GetComponent<fw::Physics>()->m_pBody;

    float targetVelocity = dir.x * speed;
    float velocityDiff = targetVelocity - body->GetLinearVelocity().x;
    float timeStep = 1 / 60.0f;
    body->ApplyForceToCenter(b2Vec2((body->GetMass() * velocityDiff / timeStep),0), true);

    if (m_JumpAllowed == true)
        body->ApplyLinearImpulse(b2Vec2(0, dir.y * speed), b2Vec2(transform->m_Position.x, transform->m_Position.y), true);
    
    if( m_pController->WasPressed( Controller::Mask::Action ) )
    {
        transform->m_Position.x = fw::Random::Float( 0.0f, 10.0f );
        transform->m_Position.y = fw::Random::Float( 0.0f, 10.0f );
    }

    dir.Normalize();
}

void Player::BeginContact(GameObject* contact)
{
    m_JumpAllowed = true;
}

void Player::EndContact(GameObject* contact)
{
    m_JumpAllowed = false;
}
