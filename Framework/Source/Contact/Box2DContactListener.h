#pragma once
#include "CoreHeaders.h"

namespace fw
{
    class EventMangager;
    class Box2DContactListener : public b2ContactListener
    {
    public:
        Box2DContactListener(EventManager* eventManager);
        ~Box2DContactListener();

        virtual void BeginContact(b2Contact* contact) override;
        virtual void EndContact(b2Contact* contact) override;

    protected:
        EventManager* m_pEventManager;
    };
}