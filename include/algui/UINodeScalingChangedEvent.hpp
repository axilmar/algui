#ifndef ALGUI_UINODESCALINGCHANGEDEVENT_HPP
#define ALGUI_UINODESCALINGCHANGEDEVENT_HPP


#include "UINodeEvent.hpp"


namespace algui {


    /**
     * Scaling changed event.
     */
    class UINodeScalingChangedEvent : public UINodeEvent {
    public:
        /**
         * Constructor.
         * @param target target node.
         */
        UINodeScalingChangedEvent(std::shared_ptr<UINode>&& target)
            : UINodeEvent("scalingChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_UINODESCALINGCHANGEDEVENT_HPP
