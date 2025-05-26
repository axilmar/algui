#include "algui/EventTarget.hpp"


namespace algui {


    EventTarget::EventHandlerId EventTarget::addEventHandler(const std::string& eventName, const EventHandler& handler) {
        EventHandlerList& eventList = m_eventHandlers[eventName];
        auto it = eventList.insert(eventList.begin(), handler);
        return std::make_tuple(eventName, it);
    }


    void EventTarget::removeEventHandler(const EventHandlerId& eventHandlerId) {
        const auto& it = m_eventHandlers.find(std::get<0>(eventHandlerId));
        if (it != m_eventHandlers.end()) {
            it->second.erase(std::get<1>(eventHandlerId));
        }
    }


    bool EventTarget::dispatchEvent(const std::string& eventName, void* event) const {
        const auto& it = m_eventHandlers.find(eventName);
        if (it != m_eventHandlers.end()) {
            for (const EventHandler& handler : it->second) {
                if (handler(event)) {
                    return true;
                }
            }
        }
        return false;
    }


} //namespace algui
