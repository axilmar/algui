#ifndef ALGUI_INTERACTIVEUINODEEVENT_HPP
#define ALGUI_INTERACTIVEUINODEEVENT_HPP


#include <memory>
#include <utility>
#include "Event.hpp"


namespace algui {


    class InteractiveUINode;


    /**
     * Base class for events of class `InteractiveUINode`.
     */
    class InteractiveUINodeEvent : public Event {
    public:
        /**
         * Returns the event target.
         * @return the event target.
         */
        const std::shared_ptr<InteractiveUINode>& getTarget() const {
            return m_target;
        }

    protected:
        /**
         * The constructor.
         * @param type event type.
         * @param target event target.
         */
        InteractiveUINodeEvent(std::string&& type, std::shared_ptr<InteractiveUINode>&& target)
            : Event(std::move(type))
            , m_target(std::move(target))
        {
        }

    private:
        std::shared_ptr<InteractiveUINode> m_target;
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODEEVENT_HPP
