#include "Events.h"
#include "Framework.h"


namespace fw
{
    void fw::ContactEvent::BeginContact()
    {
        m_pObj1->BeginContact(m_pObj2);
        m_pObj2->BeginContact(m_pObj1);
    }
    void ContactEvent::EndContact()
    {
        m_pObj1->EndContact(m_pObj2);
        m_pObj2->EndContact(m_pObj1);
    }
}