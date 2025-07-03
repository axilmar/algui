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
         * The constructor.
         * @param type event type.
         * @param target the target UINode.
         */
        UINodeEvent(const std::string_view& type, std::shared_ptr<UINode>&& target)
            : Event(type)
            , m_target(std::move(target))
        {
        }

        /**
         * Returns the target UINode.
         * @return the target UINode.
         */
        const std::shared_ptr<UINode>& getTarget() const {
            return m_target;
        }

    private:
        std::shared_ptr<UINode> m_target;
    };


} //namespace algui


#endif //ALGUI_UINODEEVENT_HPP
