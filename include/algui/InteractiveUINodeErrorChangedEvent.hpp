#ifndef ALGUI_INTERACTIVEUINODEERRORCHANGEDEVENT_HPP
#define ALGUI_INTERACTIVEUINODEERRORCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Error changed event.
     */
    class InteractiveUINodeErrorChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodeErrorChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("errorChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODEERRORCHANGEDEVENT_HPP
