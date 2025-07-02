#ifndef ALGUI_LOSTFOCUSEVENT_HPP
#define ALGUI_LOSTFOCUSEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * lost focus event.
     */
    class LostFocusEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        LostFocusEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("lostFocus", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_LOSTFOCUSEVENT_HPP
