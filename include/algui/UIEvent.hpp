#ifndef ALGUI_UIEVENT_HPP
#define ALGUI_UIEVENT_HPP


#include <memory>
#include "Event.hpp"


namespace algui {


    class UINode;


    /**
     * Base class for UI events.
     */
    class UIEvent : public Event {
    public:
        /**
         * Constructor.
         * @param target the target node.
         */
        UIEvent(const std::shared_ptr<UINode>& target)
            : m_target(target)
        {
        }

        /**
         * Returns the target node.
         * @return the target node.
         */
        const std::shared_ptr<UINode>& getTarget() const {
            return m_target;
        }

    private:
        std::shared_ptr<UINode> m_target;
    };


} //namespace algui


#endif //ALGUI_UIEVENT_HPP
