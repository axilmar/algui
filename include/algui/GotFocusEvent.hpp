#ifndef ALGUI_GOTFOCUSEVENT_HPP
#define ALGUI_GOTFOCUSEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * got focus event.
     */
    class GotFocusEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        GotFocusEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("gotFocus", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_GOTFOCUSEVENT_HPP
