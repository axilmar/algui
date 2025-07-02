#ifndef ALGUI_ENABLEDCHANGEDEVENT_HPP
#define ALGUI_ENABLEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Enabled changed event.
     */
    class EnabledChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        EnabledChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("enabledChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_ENABLEDCHANGEDEVENT_HPP
