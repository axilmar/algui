#ifndef ALGUI_EVENTTARGET_HPP
#define ALGUI_EVENTTARGET_HPP


#include <map>
#include <functional>
#include <list>
#include "SharedObject.hpp"
#include "Event.hpp"
#include "FirstArgumentType.hpp"


namespace algui {


    /**
     * Base class for event targets.
     */
    class EventTarget : public SharedObject {
    public:
        /**
         * Event listener function.
         * @param event event to process.
         * @return true if event propagation should stop, false otherwise.
         */
        using EventListenerFunction = std::function<bool(const Event& event)>;

        /**
         * Id for event listeners.
         */
        class EventListenerId {
        public:
            /**
             * The default constructor.
             */
            EventListenerId() {}

            /**
             * The copy constructor.
             * Deleted because there shall be only one owner of an event listener.
             */
            EventListenerId(const EventListenerId&) = delete;

            /**
             * The move constructor.
             * @param id source object.
             */
            EventListenerId(EventListenerId&& id);

            /**
             * The copy assignment operator.
             * Deleted because there shall be only one owner of an event listener.
             */
            EventListenerId& operator = (const EventListenerId&) = delete;

            /**
             * The move assignment operator.
             * @param id source object.
             * @return reference to this.
             */
            EventListenerId& operator = (EventListenerId&& id) noexcept;

        private:
            mutable EventTarget* m_eventTarget{nullptr};
            std::string m_eventType;
            std::list<EventListenerFunction>::iterator m_it;
            EventListenerId(EventTarget* eventTarget, std::string&& eventType, std::list<EventListenerFunction>::iterator&& it);
            friend class EventTarget;
        };

        /**
         * Removes all event listener ids registered to this.
         */
        virtual ~EventTarget();

        /**
         * Adds an event listener.
         * @param eventType type of event.
         * @param func function to add.
         * @param prioritized if true, the function will be executed before previously added functions, otherwise it will be executed after them.
         * @return an id for the event listener.
         */
        EventListenerId addEventListener(std::string&& eventType, EventListenerFunction&& func, bool prioritized = false);

        /**
         * Adds an event listener.
         * @param eventType type of event.
         * @param func function to add; the event parameter can be any type derived from `class Event`.
         * @param prioritized if true, the function will be executed before previously added functions, otherwise it will be executed after them.
         * @return an id for the event listener.
         */
        template <class L>
        EventListenerId addEventListener(std::string&& eventType, L&& func, bool prioritized = false) {
            auto f = EventListenerFunction([func](const Event& e) {
                using T = FirstArgumentType<L>;
                return func(dynamic_cast<const T&>(e));
            });
            return addEventListener(std::move(eventType), std::move(f), prioritized);
        }

        /**
         * Removes an event listener.
         * @param id id of event listener to remove.
         * @exception std::invalid_argument thrown if the id is already removed or has not been created by this event target.
         */
        void removeEventListener(const EventListenerId& id);

        /**
         * Dispatches the given event.
         * It executes the functions registered for the given event, until a function returns true.
         * @param event the event to dispatch.
         * @return true if propagation stopped, false otherwise.
         * @exception std::bad_cast thrown if an event handler's first argument type does not match the underlying type of the given event.
         */
        bool dispatchEvent(const Event& event) const;

        /**
         * Adds an event listener id to this object.
         * The event listener will automatically be removed from its event target when this event target goes out of scope.
         * @param id the event listener id to add to this object.
         */
        void addEventListenerId(EventListenerId&& id);

    private:
        std::map<std::string, std::list<EventListenerFunction>, std::less<>> m_eventListenerFunctions;
        std::vector<EventListenerId> m_eventListenerIds;
    };


} //namespace algui


#endif //ALGUI_EVENTTARGET_HPP
