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
     * Event handlers are executed in the order they are registered.
     */
    class EventTarget {
    public:
        /**
         * Type of event handler function.
         * @param event pointer to event structure; it may be null if there are no event data for the event.
         * @return true if event should not be propagated, false otherwise.
         */
        using EventHandler = std::function<bool(const void* event)>;

        /** Type of event handler list. */
        using EventHandlerList = std::list<EventHandler>;

        /**
         * Type of event handler id.
         */
        using EventHandlerId = std::tuple<std::string, typename EventHandlerList::iterator, int>;

        /**
         * Adds an event handler.
         * @param eventName name of event.
         * @param handler the event handler to add.
         * @param prioritized if true, the handler is added at the beginning of the list, otherwise at the end of the list.
         * @param phase event phase.
         * @return the event handler id to use for unregistration.
         */
        virtual EventHandlerId addEventHandler(const std::string& eventName, const EventHandler& handler, bool prioritized = false, int phase = 0);

        /**
         * Removes an event handler.
         * If the handler is already removed, the effects are undefined.
         * @param eventHandlerId id of event handler to remove.
         * @return true if the event handler found, false otherwise.
         */
        virtual bool removeEventHandler(const EventHandlerId& eventHandlerId);

        /**
         * Dispatches an event.
         * @param eventName name of event to dispatch.
         * @param event event data.
         * @param phase event phase.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dispatchEvent(const std::string& eventName, const void* event = nullptr, int phase = 0) const;

    private:
        std::map<std::tuple<std::string, int>, EventHandlerList> m_eventHandlers;
    };


} //namespace algui


#endif //ALGUI_EVENTTARGET_HPP
