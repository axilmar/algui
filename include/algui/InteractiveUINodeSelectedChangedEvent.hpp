#ifndef ALGUI_INTERACTIVEUINODESELECTEDCHANGEDEVENT_HPP
#define ALGUI_INTERACTIVEUINODESELECTEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Selected changed event.
     */
    class InteractiveUINodeSelectedChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodeSelectedChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("selectedChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODESELECTEDCHANGEDEVENT_HPP
