#ifndef ALGUI_ERRORCHANGEDEVENT_HPP
#define ALGUI_ERRORCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Error changed event.
     */
    class ErrorChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        ErrorChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("errorChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_ERRORCHANGEDEVENT_HPP
