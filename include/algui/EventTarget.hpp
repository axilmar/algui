#ifndef ALGUI_EVENTTARGET_HPP
#define ALGUI_EVENTTARGET_HPP


#include <string>
#include <map>
#include <list>
#include <functional>
#include <tuple>


namespace algui {


    /**
     * An event target.
     * It is modelled after the Javascript EventTarget class.
     * It allows registration/unregistration of event handlers.
     * Event handlers are executed in the reverse order they are registered.
     */
    class EventTarget {
    public:
        /**
         * Type of event handler function.
         * @param event pointer to event structure; it may be null if there are no event data for the event.
         * @return true if event should not be propagated, false otherwise.
         */
        using EventHandler = std::function<bool(void* event)>;

        /** Type of event handler list. */
        using EventHandlerList = std::list<EventHandler>;

        /**
         * Type of event handler id.
         */
        using EventHandlerId = std::tuple<std::string, typename EventHandlerList::iterator>;

        /**
         * Adds an event handler.
         * @param eventName name of event.
         * @param handler the event handler to add.
         * @return the event handler id to use for unregistration.
         */
        EventHandlerId addEventHandler(const std::string& eventName, const EventHandler& handler);

        /**
         * Removes an event handler.
         * If the handler is already removed, the effects are undefined.
         * @param eventHandlerId id of event handler to remove.
         */
        void removeEventHandler(const EventHandlerId& eventHandlerId);

        /**
         * Dispatches an event.
         * @param eventName name of event to dispatch.
         * @param event event data.
         * @return true if the event was processed, false otherwise.
         */
        bool dispatchEvent(const std::string& eventName, void* event = nullptr) const;

    private:
        std::map<std::string, EventHandlerList> m_eventHandlers;
    };


} //namespace algui


#endif //ALGUI_EVENTTARGET_HPP
