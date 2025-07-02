#ifndef ALGUI_VISIBLECHANGEDEVENT_HPP
#define ALGUI_VISIBLECHANGEDEVENT_HPP


#include "UINodeEvent.hpp"


namespace algui {


    /**
     * Visible changed event.
     */
    class VisibleChangedEvent : public UINodeEvent {
    public:
        /**
         * Constructor.
         * @param target target node.
         */
        VisibleChangedEvent(std::shared_ptr<UINode>&& target)
            : UINodeEvent("visibleChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_VISIBLECHANGEDEVENT_HPP
