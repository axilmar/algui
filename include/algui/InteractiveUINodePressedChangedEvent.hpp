#ifndef ALGUI_INTERACTIVEUINODEPRESSEDCHANGEDEVENT_HPP
#define ALGUI_INTERACTIVEUINODEPRESSEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Pressed changed event.
     */
    class InteractiveUINodePressedChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodePressedChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("pressedChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODEPRESSEDCHANGEDEVENT_HPP
