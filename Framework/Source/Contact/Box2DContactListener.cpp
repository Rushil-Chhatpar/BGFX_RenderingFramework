#include "Framework.h"
#include "Box2DContactListener.h"



namespace fw
{
    fw::Box2DContactListener::Box2DContactListener(EventManager* eventManager)
        : m_pEventManager(eventManager)
    {
    }
    Box2DContactListener::~Box2DContactListener()
    {
    }
    void Box2DContactListener::BeginContact(b2Contact* contact)
    {
        GameObject* obj1 = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        GameObject* obj2 = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        m_pEventManager->AddEvent(new ContactEvent(obj1, obj2, ContactEvent::EventType::BeginContact));
    }
    void Box2DContactListener::EndContact(b2Contact* contact)
    {
        GameObject* obj1 = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        GameObject* obj2 = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        m_pEventManager->AddEvent(new ContactEvent(obj1, obj2, ContactEvent::EventType::EndContact));

    }
}