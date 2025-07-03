#ifndef ALGUI_CHILDEVENT_HPP
#define ALGUI_CHILDEVENT_HPP


#include "ObjectEvent.hpp"


namespace algui {


    /**
     * Base class for object child events.
     * @param T type of object.
     */
    template <class T> class ChildEvent : public ObjectEvent<T> {
    public:
        /**
         * The constructor.
         * @param type event type.
         * @param parent the parent.
         * @param child the child.
         */
        ChildEvent(const std::string_view& type, std::shared_ptr<T>&& parent, const std::shared_ptr<T>& child)
            : ObjectEvent<T>(type, std::move(parent))
            , m_child(child)
        {
        }

        /**
         * Returns the affected child.
         * @return the affected child.
         */
        const std::shared_ptr<T>& getChild() const {
            return m_child;
        }

    private:
        std::shared_ptr<T> m_child;
    };


} //namespace algui


#endif //ALGUI_CHILDEVENT_HPP
