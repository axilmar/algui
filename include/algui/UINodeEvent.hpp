#ifndef ALGUI_UINODEEVENT_HPP
#define ALGUI_UINODEEVENT_HPP


#include <memory>
#include <utility>
#include "Event.hpp"


namespace algui {


    class UINode;


    /**
     * Base class for UINode events.
     */
    class UINodeEvent : public Event {
    public:
        /**
         * Returns the target UINode.
         * @return the target UINode.
         */
        const std::shared_ptr<UINode>& getTarget() const {
            return m_target;
        }

    protected:
        /**
         * The constructor.
         * @param type event type.
         * @param target the target UINode.
         */
        UINodeEvent(std::string&& type, std::shared_ptr<UINode>&& target)
            : Event(std::move(type))
            , m_target(std::move(target))
        {
        }

    private:
        std::shared_ptr<UINode> m_target;
    };


} //namespace algui


#endif //ALGUI_UINODEEVENT_HPP
