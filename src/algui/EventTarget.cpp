#include "algui/EventTarget.hpp"


namespace algui {


    EventTarget::EventHandlerId EventTarget::addEventHandler(const std::string& eventName, const EventHandler& handler, bool prioritized, int phase) {
        const auto key = std::make_tuple(eventName, phase);
        EventHandlerList& eventList = m_eventHandlers[key];
        auto it = eventList.insert(prioritized ? eventList.begin() : eventList.end(), handler);
        return std::make_tuple(eventName, it, phase);
    }


    bool EventTarget::removeEventHandler(const EventHandlerId& eventHandlerId) {
        const auto key = std::make_tuple(std::get<0>(eventHandlerId), std::get<2>(eventHandlerId));
        const auto& it = m_eventHandlers.find(key);
        if (it != m_eventHandlers.end()) {
            it->second.erase(std::get<1>(eventHandlerId));
            return true;
        }
        return false;
    }


    bool EventTarget::dispatchEvent(const std::string& eventName, const Event& event, int phase) const {
        const auto key = std::make_tuple(eventName, phase);
        const auto& it = m_eventHandlers.find(key);
        if (it != m_eventHandlers.end()) {
            for (const EventHandler& handler : it->second) {
                if (handler(eventName, event, phase)) {
                    return true;
                }
            }
        }
        return false;
    }


} //namespace algui
