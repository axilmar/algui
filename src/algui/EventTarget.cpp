#include <stdexcept>
#include "algui/EventTarget.hpp"


namespace algui {


    EventTarget::EventListenerId::EventListenerId(EventListenerId&& id)
        : m_eventTarget(id.m_eventTarget)
        , m_eventType(std::move(id.m_eventType))
        , m_it(std::move(id.m_it))
    {
        id.m_eventTarget = nullptr;
    }


    EventTarget::EventListenerId& EventTarget::EventListenerId::operator = (EventListenerId&& id) {
        EventTarget* eventTarget = id.m_eventTarget;
        m_eventType = std::move(id.m_eventType);
        m_it = std::move(id.m_it);
        id.m_eventTarget = nullptr;
        m_eventTarget = eventTarget;
        return *this;
    }


    EventTarget::EventListenerId::EventListenerId(EventTarget* eventTarget, std::string&& eventType, std::list<EventListenerFunction>::iterator&& it)
        : m_eventTarget(eventTarget)
        , m_eventType(std::move(eventType))
        , m_it(std::move(it))
    {
    }


    EventTarget::EventListenerId EventTarget::addEventListener(std::string&& eventType, EventListenerFunction&& func, bool prioritized) {
        std::list<EventListenerFunction>& list = m_eventListenerFunctions[eventType];
        auto it = list.insert(prioritized ? list.begin() : list.end(), std::move(func));
        return { this, std::move(eventType), std::move(it) };
    }


    void EventTarget::removeEventListener(const EventListenerId& id) {
        if (id.m_eventTarget != this) {
            throw std::invalid_argument("EventTarget: removeEventListener: invalid id.");
        }
        auto it = m_eventListenerFunctions.find(id.m_eventType);
        if (it != m_eventListenerFunctions.end()) {
            it->second.erase(id.m_it);
            id.m_eventTarget = nullptr;
        }
    }


    bool EventTarget::dispatchEvent(const Event& event) const {
        auto it = m_eventListenerFunctions.find(event.getType());
        if (it != m_eventListenerFunctions.end()) {
            for (const EventListenerFunction& func : it->second) {
                if (func(event)) {
                    return true;
                }
            }
        }
        return false;
    }


} //namespace algui
