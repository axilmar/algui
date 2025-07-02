#ifndef ALGUI_HIGHLIGHTEDCHANGEDEVENT_HPP
#define ALGUI_HIGHLIGHTEDCHANGEDEVENT_HPP


#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Highlighted changed event.
     */
    class HighlightedChangedEvent : public InteractiveUINodeEvent {
    public:
        /**
         * The constructor.
         * @param target the target node.
         */
        HighlightedChangedEvent(std::shared_ptr<InteractiveUINode>&& target)
            : InteractiveUINodeEvent("highlightedChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_HIGHLIGHTEDCHANGEDEVENT_HPP
