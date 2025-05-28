#include "algui/EventType.hpp"


namespace algui {


    const std::string EventType::childAdded = "childadded";
    const std::string EventType::childRemoved = "childremoved";

    const std::string EventType::focus = "focus";
    const std::string EventType::focusIn = "focusin";
    const std::string EventType::blur = "blur";
    const std::string EventType::focusOut = "focusout";

    static const std::string mouseEnter = "mouseenter";
    static const std::string mouseMove = "mousemove";
    static const std::string mouseLeave = "mouseleave";
    static const std::string mouseButtonDown = "mousebuttondown";
    static const std::string mouseButtonHeldDown = "mousebuttonhelddown";
    static const std::string mouseButtonUp = "mousebuttonup";

    static const std::string mousewheel = "mousewheel";
    static const std::string click = "click";
    static const std::string doubleClick = "doubleclick";

    static const std::string dragEnter = "dragenter";
    static const std::string drag = "drag";
    static const std::string dragLeave = "dragleave";
    static const std::string drop = "drop";

    static const std::string keyDown = "keydown";
    static const std::string keyUp = "keyup";
    static const std::string keyChar = "keychar";
    static const std::string unusedKeyDown = "unusedkeydown";
    static const std::string unusedKeyUp = "unusedkeyup";
    static const std::string unusedKeyChar = "unusedkeychar";

    static const std::string timer = "timer";


} //namespace algui
