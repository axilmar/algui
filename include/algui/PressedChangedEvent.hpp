#ifndef ALGUI_PRESSEDCHANGEDEVENT_HPP
#define ALGUI_PRESSEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Pressed changed event.
     */
    class PressedChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        PressedChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("pressedChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_PRESSEDCHANGEDEVENT_HPP
