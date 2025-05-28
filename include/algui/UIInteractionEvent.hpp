#ifndef ALGUI_UINTERACTIONEVENT_HPP
#define ALGUI_UINTERACTIONEVENT_HPP


#include <memory>
#include "UIEvent.hpp"


namespace algui {


    class UIInteractiveNode;


    /**
     * Base class for UI interaction events.
     */
    class UIInteractionEvent : public UIEvent {
    public:
        /**
         * Constructor.
         * @param target the target node.
         */
        UIInteractionEvent(const std::shared_ptr<UIInteractiveNode>& target);

        /**
         * Returns the target node.
         * @return the target node.
         */
        const std::shared_ptr<UIInteractiveNode>& getTarget() const {
            return m_target;
        }

    private:
        std::shared_ptr<UIInteractiveNode> m_target;
    };


} //namespace algui


#endif //ALGUI_UINTERACTIONEVENT_HPP
