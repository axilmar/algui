#ifndef ALGUI_SELECTEDCHANGEDEVENT_HPP
#define ALGUI_SELECTEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Selected changed event.
     */
    class SelectedChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        SelectedChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("selectedChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_SELECTEDCHANGEDEVENT_HPP
