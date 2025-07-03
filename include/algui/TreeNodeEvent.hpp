#ifndef ALGUI_TREENODEEVENT_HPP
#define ALGUI_TREENODEEVENT_HPP


#include "Event.hpp"


namespace algui {


    /**
     * Base class for tree node events.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class TreeNodeEvent : public Event {
    public:
        /**
         * The constructor.
         * @param type event type.
         * @param target event target.
         */
        TreeNodeEvent(std::string&& type, std::shared_ptr<T>&& target)
            : Event(std::move(type))
            , m_target(std::move(target))
        {
        }

        /**
         * Returns the event target.
         * @return the event target.
         */
        const std::shared_ptr<T>& getTarget() const {
            return m_target;
        }

    private:
        std::shared_ptr<T> m_target;
    };


} //namespace algui


#endif //ALGUI_TREENODEEVENT_HPP
