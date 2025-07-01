#ifndef ALGUI_UINODERECTCHANGEDEVENT_HPP
#define ALGUI_UINODERECTCHANGEDEVENT_HPP


#include "UINodeEvent.hpp"


namespace algui {


    /**
     * Rect changed event.
     */
    class UINodeRectChangedEvent : public UINodeEvent {
    public:
        /**
         * Constructor.
         * @param target target node.
         */
        UINodeRectChangedEvent(std::shared_ptr<UINode>&& target)
            : UINodeEvent("rectChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_UINODERECTCHANGEDEVENT_HPP
