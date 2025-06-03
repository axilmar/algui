#ifndef ALGUI_EVENTTARGET_HPP
#define ALGUI_EVENTTARGET_HPP


#include <map>
#include <functional>
#include <tuple>
#include <list>
#include <set>


namespace algui {


    /**
     * Base class for events.
     */
    class Event {
    public:
        virtual ~Event() {
        }
    };


    template <class EventType, class EventPhaseType>
    class EventHandlerOwner;


    /**
     * Base class for event targets.
     */
    template <class EventType, class EventPhaseType>
    class EventTarget {
    public:
        /**
         * Event handler.
         * @param type event type.
         * @param event event.
         * @param phase event phase.
         * @return true if the event was processed and event propagation needs to stop, false otherwise.
         */
        using EventHandler = std::function<bool(EventType type, const Event& event, EventPhaseType phase)>;

    private:
        //event key
        using EventKey = std::tuple<EventType, EventPhaseType>;

        //event handler entry
        struct EventHandlerEntry {
            std::list<EventHandlerEntry>* list;
            EventHandler handler;
        };

        //event handler list
        using EventHandlerList = std::list<EventHandlerEntry>;

    public:
        /**
         * Event handler id.
         */
        class EventHandlerId {
        public:
            /**
             * Removes the event handler from its owner event target.
             * @return true if the event handler was succesfully detached, false if it has already been detached.
             */
            bool detach() const {
                if (m_it->list) {
                    return this->m_eventTarget->removeEventHandler(*this);
                }
                return false;
            }

            /**
             * Required in order to put event handler ids in sets.
             * @param eventHandlerId event handler id to compare to this.
             * @return true if this comes before the given one, false otherwise.
             */
            bool operator < (const EventHandlerId& eventHandlerId) const {
                return &*m_it < &*eventHandlerId.m_it;
            }

        private:
            EventTarget* m_eventTarget;
            EventHandlerList* m_list;
            typename EventHandlerList::iterator m_it;
            EventHandlerId(EventTarget* target, EventHandlerList* list, const typename EventHandlerList::iterator& it) :
                m_eventTarget(target), m_list(list), m_it(it) {}
            friend class EventTarget;
        };

        EventTarget() {
        }

        EventTarget(const EventTarget&) = delete;
        EventTarget(EventTarget&&) = delete;

        /**
         * Nullifies all event handlers.
         */
        virtual ~EventTarget() {
            for (auto& listPair : m_eventHandlers) {
                for (auto& eventEntry : listPair.second) {
                    eventEntry.list = nullptr;
                }
            }
        }

        EventTarget& operator =(const EventTarget&) = delete;
        EventTarget& operator =(EventTarget&&) = delete;

        /**
         * Adds an event handler.
         * @param type event type.
         * @param handler event handler.
         * @param prioritized if true, then the handler will be executed before other existing handlers, otherwise after existing handlers.
         * @param phase event phase.
         * @param owner optional owner; if given, the event handler id is added to the given event handler owner.
         * @return id of event handler.
         */
        EventHandlerId addEventHandler(EventType type, const EventHandler& handler, EventPhaseType phase = EventPhaseType(), bool prioritized = false, EventHandlerOwner<EventType, EventPhaseType>* owner = nullptr) {
            const auto key = std::make_tuple(type, phase);
            EventHandlerList& eventHandlerList = m_eventHandlers[key];
            EventHandlerEntry entry{ &eventHandlerList, handler };
            auto it = eventHandlerList.insert(prioritized ? eventHandlerList.begin() : eventHandlerList.end(), entry);
            EventHandlerId eventHandlerId{ this, &eventHandlerList, it };
            if (owner) {
                owner->add(eventHandlerId);
            }
            return eventHandlerId;
        }

        /**
         * Removes an event handler.
         * @param eventHandlerId id of event handler.
         * @return true if the handler was successfully removed, false if the entry was already removed or not part of this event handler.
         */
        bool removeEventHandler(const EventHandlerId& eventHandlerId) {
            if (eventHandlerId.m_eventTarget == this && eventHandlerId.m_it->list) {
                eventHandlerId.m_it->list->erase(eventHandlerId.m_it);
                eventHandlerId.m_it->list = nullptr;
                return true;
            }
            return false;
        }

        /**
         * Dispatches an event.
         * @param type event type.
         * @param event event.
         * @param phase event phase.
         * @return true if the event was processed by an event handler, false otherwise.
         */
        bool dispatchEvent(EventType type, const Event& event, EventPhaseType phase = EventPhaseType()) const {
            const auto key = std::make_tuple(type, phase);
            auto it = m_eventHandlers.find(key);
            if (it != m_eventHandlers.end()) {
                for (const EventHandlerEntry& entry : it->second) {
                    if (entry.handler(type, event, phase)) {
                        return true;
                    }
                }
            }
            return false;
        }

    private:
        //typedefs
        using EventMap = std::map<EventKey, EventHandlerList>;

        //members
        EventMap m_eventHandlers;
    };


    /**
     * Base class for event handler owners.
     * It allows registration of event handler ids,
     * which will be automatically detached when this goes out of scope.
     */
    template <class EventType, class EventPhaseType>
    class EventHandlerOwner {
    public:
        /**
         * Removes all registered event handlers from their event targets.
         */
        virtual ~EventHandlerOwner() {
            detachAll();
        }

        /**
         * Registers and event handler with this event handler owner.
         * @param eventHandlerId id of event handler.
         * @return true if added, false if it existed.
         */
        bool add(const typename EventTarget<EventType, EventPhaseType>::EventHandlerId& eventHandlerId) {
            const auto [it, ok] = m_eventHandlers.insert(eventHandlerId);
            return ok;
        }

        /**
         * Removes an event handler.
         * @param eventHandlerId id of event handler to remove.
         * @return true if added succesfully, false if it did not exist.
         */
        bool remove(const typename EventTarget<EventType, EventPhaseType>::EventHandlerId& eventHandlerId) {
            return m_eventHandlers.erase(eventHandlerId) == 1;
        }

        /**
         * Detaches all event handlers.
         */
        void detachAll() {
            for (const auto& eventHandlerId : m_eventHandlers) {
                eventHandlerId.detach();
            }
            m_eventHandlers.clear();
        }

    private:
        std::set<typename EventTarget<EventType, EventPhaseType>::EventHandlerId> m_eventHandlers;
    };


} //namespace algui


#endif //ALGUI_EVENTTARGET_HPP
