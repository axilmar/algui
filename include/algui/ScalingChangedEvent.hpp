#ifndef ALGUI_SCALINGCHANGEDEVENT_HPP
#define ALGUI_SCALINGCHANGEDEVENT_HPP


#include "UINodeEvent.hpp"


namespace algui {


    /**
     * Scaling changed event.
     */
    class ScalingChangedEvent : public UINodeEvent {
    public:
        /**
         * Constructor.
         * @param target target node.
         */
        ScalingChangedEvent(std::shared_ptr<UINode>&& target)
            : UINodeEvent("scalingChanged", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_SCALINGCHANGEDEVENT_HPP
