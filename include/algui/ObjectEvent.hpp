#ifndef ALGUI_OBJECTEVENT_HPP
#define ALGUI_OBJECTEVENT_HPP


#include <memory>
#include <utility>
#include "Event.hpp"


namespace algui {


    /**
     * Base class for object events.
     * @param T type of object.
     */
    template <class T> class ObjectEvent : public Event {
    public:
        /**
         * The constructor.
         * @param type event type.
         * @param object the object.
         */
        ObjectEvent(const std::string_view& type, std::shared_ptr<T>&& object)
            : Event(type)
            , m_object(std::move(object))
        {
        }

        /**
         * Returns the object T.
         * @return the object T.
         */
        const std::shared_ptr<T>& getObject() const {
            return m_object;
        }

    private:
        std::shared_ptr<T> m_object;
    };


} //namespace algui


#endif //ALGUI_TEVENT_HPP
