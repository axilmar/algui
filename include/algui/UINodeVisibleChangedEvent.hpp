#ifndef ALGUI_UINODEVISIBLECHANGEDEVENT_HPP
#define ALGUI_UINODEVISIBLECHANGEDEVENT_HPP


#include "UINodeEvent.hpp"


namespace algui {


    /**
     * Visible changed event.
     */
    class UINodeVisibleChangedEvent : public UINodeEvent {
    public:
        /**
         * Constructor.
         * @param target target node.
         */
        UINodeVisibleChangedEvent(std::shared_ptr<UINode>&& target)
            : UINodeEvent("visibleChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_UINODEVISIBLECHANGEDEVENT_HPP
