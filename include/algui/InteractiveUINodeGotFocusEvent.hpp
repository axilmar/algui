#ifndef ALGUI_INTERACTIVEUINODEGOTFOCUSEVENT_HPP
#define ALGUI_INTERACTIVEUINODEGOTFOCUSEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * got focus event.
     */
    class InteractiveUINodeGotFocusEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodeGotFocusEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("gotFocus", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODEGOTFOCUSEVENT_HPP
