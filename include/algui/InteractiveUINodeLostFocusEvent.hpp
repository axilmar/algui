#ifndef ALGUI_INTERACTIVEUINODELOSTFOCUSEVENT_HPP
#define ALGUI_INTERACTIVEUINODELOSTFOCUSEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * lost focus event.
     */
    class InteractiveUINodeLostFocusEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodeLostFocusEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("lostFocus", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODELOSTFOCUSEVENT_HPP
