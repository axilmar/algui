#ifndef ALGUI_INTERACTIVEUINODEENABLEDCHANGEDEVENT_HPP
#define ALGUI_INTERACTIVEUINODEENABLEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Enabled changed event.
     */
    class InteractiveUINodeEnabledChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodeEnabledChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("enabledChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODEENABLEDCHANGEDEVENT_HPP
