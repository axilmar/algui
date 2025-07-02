#ifndef ALGUI_INTERACTIVEUINODEHIGHLIGHTEDCHANGEDEVENT_HPP
#define ALGUI_INTERACTIVEUINODEHIGHLIGHTEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Highlighted changed event.
     */
    class InteractiveUINodeHighlightedChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        InteractiveUINodeHighlightedChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("highlightedChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODEHIGHLIGHTEDCHANGEDEVENT_HPP
