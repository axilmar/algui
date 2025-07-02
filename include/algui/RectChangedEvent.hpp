#ifndef ALGUI_RECTCHANGEDEVENT_HPP
#define ALGUI_RECTCHANGEDEVENT_HPP


#include "UINodeEvent.hpp"


namespace algui {


    /**
     * Rect changed event.
     */
    class RectChangedEvent : public UINodeEvent {
    public:
        /**
         * Constructor.
         * @param target target node.
         */
        RectChangedEvent(std::shared_ptr<UINode>&& target)
            : UINodeEvent("rectChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_RECTCHANGEDEVENT_HPP
