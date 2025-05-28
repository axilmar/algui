#pragma warning (disable: 4309)
#include "algui/UIInteractionEvent.hpp"
#include "algui/UIInteractiveNode.hpp"


namespace algui {


    UIInteractionEvent::UIInteractionEvent(const std::shared_ptr<UIInteractiveNode>& target)
        : UIEvent(target)
        , m_target(target)
    {
    }


} //namespace algui
