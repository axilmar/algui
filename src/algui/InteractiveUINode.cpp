#include "algui/InteractiveUINode.hpp"
#include "algui/ObjectEvent.hpp"
#include "algui/KeyboardEvent.hpp"


#ifdef ERROR
#undef ERROR
#endif


namespace algui {


    enum FLAGS {
        ENABLED     = 1 << 0,
        HIGHLIGHTED = 1 << 1, 
        PRESSED     = 1 << 2,
        SELECTED    = 1 << 3,
        ERROR       = 1 << 4
    };


    static InteractiveUINode* _focusedNode = nullptr;
    static ALLEGRO_EVENT _prevMouseEvent = { 0 };
    static ALLEGRO_EVENT _buttonDownEvent = { 0 };
    static float _dragAndDropDistance = 4;
    static bool _dragAndDrop = false;
    static int _dragAndDropButton = 0;
    static std::any _draggedData;
    static bool _resetPrevMousePosition = false;
    static std::vector<DraggedImage>* _draggedImages = nullptr;


    static float _distance(float x1, float y1, float x2, float y2) {
        const float dx = abs(x1 - x2);
        const float dy = abs(y1 - y2);
        return sqrt(dx*dx+dy*dy);
    }


    template <class T, class E>
    static bool _dispatchEvent(T* object, const E& event) {
        return object ? object->dispatchEvent(event) : false;
    }


    static void _renderDraggedImages() {
        if (_draggedImages) {
            ALLEGRO_MOUSE_STATE state;
            al_get_mouse_state(&state);
            for (const DraggedImage& di : *_draggedImages) {
                if (di.enabled) {
                    al_draw_bitmap(di.bitmap, state.x - di.xFocus, state.y - di.yFocus, 0);
                }
            }
        }
    }


    InteractiveUINode::~InteractiveUINode() {
        if (this == _focusedNode) {
            _focusedNode = nullptr;
        }
    }


    void InteractiveUINode::render() {
        UINode::render();
        _renderDraggedImages();
    }


    void InteractiveUINode::render(const Rect& clipping) {
        UINode::render(clipping);
        _renderDraggedImages();
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getParent() const {
        InteractiveUINode* inode = getParentPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getParentPtr() const {
        for (UINode* node = UINode::getParentPtr(); node; node = node->getParentPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getFirstChild() const {
        InteractiveUINode* inode = getFirstChildPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getFirstChildPtr() const {
        for (UINode* node = UINode::getFirstChildPtr(); node; node = node->getNextSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getLastChild() const {
        InteractiveUINode* inode = getLastChildPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getLastChildPtr() const {
        for (UINode* node = UINode::getLastChildPtr(); node; node = node->getPrevSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getPrevSibling() const {
        InteractiveUINode* inode = getPrevSiblingPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getPrevSiblingPtr() const {
        for (UINode* node = UINode::getPrevSiblingPtr(); node; node = node->getPrevSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getNextSibling() const {
        InteractiveUINode* inode = getNextSiblingPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getNextSiblingPtr() const {
        for (UINode* node = UINode::getNextSiblingPtr(); node; node = node->getNextSiblingPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                return inode;
            }
        }
        return nullptr;
    }


    std::shared_ptr<InteractiveUINode> InteractiveUINode::getRoot() const {
        InteractiveUINode* inode = getRootPtr();
        return inode ? inode->sharedFromThis<InteractiveUINode>() : nullptr;
    }


    InteractiveUINode* InteractiveUINode::getRootPtr() const {
        InteractiveUINode* result = const_cast<InteractiveUINode*>(this);
        for (UINode* node = UINode::getParentPtr(); node; node = node->getParentPtr()) {
            InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
            if (inode) {
                result = inode;
            }
        }
        return result;
    }


    bool InteractiveUINode::isEnabled() const {
        return (m_flags & ENABLED) == ENABLED;
    }


    void InteractiveUINode::setEnabled(bool v) {
        if (v != isEnabled()) {
            if (!v && contains(_focusedNode)) {
                _focusedNode->blur();
            }
            m_flags = v ? m_flags | ENABLED : m_flags & ~ENABLED;
            _setEnabledTree(this, !UINode::getParentPtr() || UINode::getParentPtr()->isEnabledTree());
            dispatchEvent(ObjectEvent<InteractiveUINode>("enabledChanged", sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isFocused() const {
        return _focusedNode == this;
    }


    InteractiveUINode* InteractiveUINode::getFocusedNode() {
        return _focusedNode;
    }


    bool InteractiveUINode::setFocused(bool v) {
        if (v == isFocused()) {
            return true;
        }

        if (v) {
            if (!isEnabledTree()) {
                return false;
            }
            if (_focusedNode) {
                _focusedNode->blur();
            }
            _focusedNode = this;
            _setFocusedTree(this);
            ObjectEvent<InteractiveUINode> event("gotFocus", sharedFromThis<InteractiveUINode>());
            for (InteractiveUINode* inode = this; inode; inode = inode->getParentPtr()) {
                inode->dispatchEvent(event);
            }
        }

        else {
            _focusedNode = nullptr;
            _setFocusedTree(this);
            ObjectEvent<InteractiveUINode> event("lostFocus", sharedFromThis<InteractiveUINode>());
            for (InteractiveUINode* inode = this; inode; inode = inode->getParentPtr()) {
                inode->dispatchEvent(event);
            }
        }

        return true;
    }


    bool InteractiveUINode::isHighlighted() const {
        return (m_flags & HIGHLIGHTED) == HIGHLIGHTED;
    }


    void InteractiveUINode::setHighlighted(bool v) {
        if (v != isHighlighted()) {
            m_flags = v ? m_flags | HIGHLIGHTED : m_flags & ~HIGHLIGHTED;
            _setHighlightedTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isHighlightedTree());
            dispatchEvent(ObjectEvent<InteractiveUINode>("highlightedChanged", sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isPressed() const {
        return (m_flags & PRESSED) == PRESSED;
    }


    void InteractiveUINode::setPressed(bool v) {
        if (v != isPressed()) {
            m_flags = v ? m_flags | PRESSED : m_flags & ~PRESSED;
            _setPressedTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isPressedTree());
            dispatchEvent(ObjectEvent<InteractiveUINode>("pressedChanged", sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isSelected() const {
        return (m_flags & SELECTED) == SELECTED;
    }


    void InteractiveUINode::setSelected(bool v) {
        if (v != isSelected()) {
            m_flags = v ? m_flags | SELECTED : m_flags & ~SELECTED;
            _setSelectedTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isSelectedTree());
            dispatchEvent(ObjectEvent<InteractiveUINode>("selectedChanged", sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::isError() const {
        return (m_flags & ERROR) == ERROR;
    }


    void InteractiveUINode::setError(bool v) {
        if (v != isError()) {
            m_flags = v ? m_flags | ERROR : m_flags & ~ERROR;
             _setErrorTree(this, UINode::getParentPtr() && UINode::getParentPtr()->isErrorTree());
             dispatchEvent(ObjectEvent<InteractiveUINode>("errorChanged", sharedFromThis<InteractiveUINode>()));
        }
    }


    bool InteractiveUINode::beginDragAndDrop(const MouseEvent& event, const std::any& data) {
        if (_dragAndDrop) {
            return false;
        }
        if (!_buttonDownEvent.mouse.button) {
            return false;
        }
        if (!data.has_value()) {
            return false;
        }
        if (_distance(event.getX(), event.getY(), _buttonDownEvent.mouse.x, _buttonDownEvent.mouse.y) < _dragAndDropDistance) {
            return false;
        }
        _dragAndDrop = true;
        _dragAndDropButton = _buttonDownEvent.mouse.button;
        _draggedData = data;
        _resetPrevMousePosition = true;
        return true;
    }


    void InteractiveUINode::endDragAndDrop() {
        if (_dragAndDrop) {
            _dragAndDrop = false;
            _dragAndDropButton = 0;
            _draggedData.reset();
            _buttonDownEvent.mouse.button = 0;
            _resetPrevMousePosition = false;
            _draggedImages = nullptr;
        }
    }


    const std::any& InteractiveUINode::getDraggedData() {
        return _draggedData;
    }


    bool InteractiveUINode::setDraggedImages(std::vector<DraggedImage>* images) {
        if (_dragAndDrop) {
            _draggedImages = images;
            return true;
        }
        return false;
    }


    bool InteractiveUINode::doEvent(const ALLEGRO_EVENT& event) {
        if (_resetPrevMousePosition) {
            _resetPrevMousePosition = false;
            _prevMouseEvent.mouse.x = -1;
            _prevMouseEvent.mouse.y = -1;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_WARPED) {
            bool result = false;

            if (event.mouse.dx || event.mouse.dy) {
                result = _dragAndDrop ? 
                         _doRootMouseMoveEvent("drag", "dragEnter", "dragLeave", this, event) : 
                         _doRootMouseMoveEvent("mouseMove", "mouseEnter", "mouseLeave", this, event);
            }

            if (event.mouse.dz || event.mouse.dw) {
                result = _dragAndDrop ? 
                         _doMouseButtonEvent("dragWheel", this, event) : 
                         _doMouseButtonEvent("mouseWheel", this, event);
            }

            _prevMouseEvent = event;
            return result;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (_dragAndDrop) {
                return false;
            }
            if (_buttonDownEvent.mouse.button == 0) {
                _buttonDownEvent = event;
            }
            bool result = _doMouseButtonEvent("mouseButtonDown", this, event);
            _prevMouseEvent = event;
            return result;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (_dragAndDrop) {
                if (event.mouse.button == _dragAndDropButton) {
                    bool result = _doMouseButtonEvent("drop", this, event);
                    _prevMouseEvent = event;
                    endDragAndDrop();
                    return result;
                }
                return false;
            }
            else {
                if (event.mouse.button == _buttonDownEvent.mouse.button) {
                    _buttonDownEvent.mouse.button = 0;
                }
                bool result = _doMouseButtonEvent("mouseButtonUp", this, event);
                _prevMouseEvent = event;
                return result;
            }
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            return _dragAndDrop ? 
                   _doDragKeyEvent("dragKeyDown", this, event, _prevMouseEvent) : 
                   _doRootKeyboardEvent("keyDown", this, event);
        }

        if (event.type == ALLEGRO_EVENT_KEY_UP) {
            return _dragAndDrop ? 
                    _doDragKeyEvent("dragKeyUp", this, event, _prevMouseEvent) : 
                    _doRootKeyboardEvent("keyUp", this, event);
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            return _dragAndDrop ? 
                   _doDragKeyEvent("dragKeyChar", this, event, _prevMouseEvent) :
                   _doRootKeyboardEvent("keyChar", this, event);
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            Event event("timer");
            return _doTimerEvent(this, event);
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_EXPOSE) {
            render(Rect::rect(event.display.x, event.display.y, event.display.width, event.display.height));
            return true;
        }

        return false;
    }


    void InteractiveUINode::setNewChildState(const std::shared_ptr<UINode>& child) {
        UINode::setNewChildState(child);
        _setEnabledTree(child.get(), isEnabledTree());
        _setFocusedTree(child.get(), isFocusedTree());
        _setHighlightedTree(child.get(), isHighlightedTree());
        _setPressedTree(child.get(), isPressedTree());
        _setSelectedTree(child.get(), isSelectedTree());
        _setErrorTree(child.get(), isErrorTree());
    }


    void InteractiveUINode::_setEnabledTree(UINode* node, bool parentEnabledTree) {
        bool enabledTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            enabledTree = ((inode->m_flags & ENABLED) == ENABLED) && parentEnabledTree;
        }
        else {
            enabledTree = parentEnabledTree;
        }
        if (node && enabledTree != node->isEnabledTree()) {
            node->UINode::_setEnabledTree(enabledTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setEnabledTree(child, enabledTree);
            }
        }
    }


    void InteractiveUINode::_setFocusedTree(UINode* node, bool parentFocusedTree) {
        bool focusedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            focusedTree = inode->isFocused() || parentFocusedTree;
        }
        else {
            focusedTree = parentFocusedTree;
        }
        if (node && focusedTree != node->isFocusedTree()) {
            node->UINode::_setFocusedTree(focusedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setFocusedTree(child, focusedTree);
            }
        }
    }


    void InteractiveUINode::_setHighlightedTree(UINode* node, bool parentHighlightedTree) {
        bool highlightedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            highlightedTree = inode->isHighlighted() || parentHighlightedTree;
        }
        else {
            highlightedTree = parentHighlightedTree;
        }
        if (node && highlightedTree != node->isHighlightedTree()) {
            node->UINode::_setHighlightedTree(highlightedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setHighlightedTree(child, highlightedTree);
            }
        }
    }


    void InteractiveUINode::_setPressedTree(UINode* node, bool parentPressedTree) {
        bool pressedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            pressedTree = inode->isPressed() || parentPressedTree;
        }
        else {
            pressedTree = parentPressedTree;
        }
        if (node && pressedTree != node->isPressedTree()) {
            node->UINode::_setPressedTree(pressedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setPressedTree(child, pressedTree);
            }
        }
    }


    void InteractiveUINode::_setSelectedTree(UINode* node, bool parentSelectedTree) {
        bool selectedTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            selectedTree = inode->isSelected() || parentSelectedTree;
        }
        else {
            selectedTree = parentSelectedTree;
        }
        if (node && selectedTree != node->isSelectedTree()) {
            node->UINode::_setSelectedTree(selectedTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setSelectedTree(child, selectedTree);
            }
        }
    }


    void InteractiveUINode::_setErrorTree(UINode* node, bool parentErrorTree) {
        bool errorTree;
        InteractiveUINode* inode = dynamic_cast<InteractiveUINode*>(node);
        if (inode) {
            errorTree = inode->isError() || parentErrorTree;
        }
        else {
            errorTree = parentErrorTree;
        }
        if (node && errorTree != node->isErrorTree()) {
            node->UINode::_setErrorTree(errorTree);
            for (UINode* child = node->getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
                _setErrorTree(child, errorTree);
            }
        }
    }


    bool InteractiveUINode::_doRootMouseMoveEvent(const std::string_view& type, const std::string_view& enterType, const std::string_view& leaveType, UINode* node, const ALLEGRO_EVENT& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }
        const bool hadMouse = node->intersects(_prevMouseEvent.mouse.x, _prevMouseEvent.mouse.y);
        const bool hasMouse = node->intersects(event.mouse.x, event.mouse.y);
        if (hadMouse && hasMouse) {
            return _doMouseMoveEvent(type, enterType, leaveType, node, event);
        }
        if (hadMouse) {
            return _doMouseLeaveEvent(leaveType, node, event);
        }
        if (hasMouse) {
            return _doMouseEnterEvent(enterType, node, event);
        }
        return false;
    }


    bool InteractiveUINode::_doMouseEnterEvent(const std::string_view& type, UINode* node, const ALLEGRO_EVENT& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        InteractiveUINode* inode = node->as<InteractiveUINode>();

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, true))) {
            return true;
        }

        UINode* child = node->getChildAt(event.mouse.x, event.mouse.y);
        if (_doMouseEnterEvent(type, child, event)) {
            return true;
        }
        
        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, false))) {
            return true;
        }

        return false;
    }


    bool InteractiveUINode::_doMouseMoveEvent(const std::string_view& type, const std::string_view& enterType, const std::string_view& leaveType, UINode* node, const ALLEGRO_EVENT& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        InteractiveUINode* inode = node->as<InteractiveUINode>();

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, true))) {
            return true;
        }

        UINode* oldChild = node->getChildAt(_prevMouseEvent.mouse.x, _prevMouseEvent.mouse.y);
        UINode* newChild = node->getChildAt(event.mouse.x, event.mouse.y);

        if (oldChild == newChild) {
            if (_doMouseMoveEvent(type, enterType, leaveType, newChild, event)) {
                return true;
            }
        }
        else {
            const bool result1 = _doMouseLeaveEvent(leaveType, oldChild, _prevMouseEvent);
            const bool result2 = _doMouseEnterEvent(enterType, newChild, event);
            if (result1 || result2) {
                return true;
            }
        }

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, false))) {
            return true;
        }

        return false;
    }


    bool InteractiveUINode::_doMouseLeaveEvent(const std::string_view& type, UINode* node, const ALLEGRO_EVENT& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        InteractiveUINode* inode = node->as<InteractiveUINode>();

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, true))) {
            return true;
        }

        UINode* child = node->getChildAt(_prevMouseEvent.mouse.x, _prevMouseEvent.mouse.y);
        if (_doMouseLeaveEvent(type, child, _prevMouseEvent)) {
            return true;
        }

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, false))) {
            return true;
        }

        return false;
    }


    bool InteractiveUINode::_doMouseButtonEvent(const std::string_view& type, UINode* node, const ALLEGRO_EVENT& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        InteractiveUINode* inode = node->as<InteractiveUINode>();

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, true))) {
            return true;
        }

        UINode* child = node->getChildAt(event.mouse.x, event.mouse.y);
        if (_doMouseButtonEvent(type, child, event)) {
            return true;
        }

        if (_dispatchEvent(inode, MouseEvent(type, event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w, event.mouse.button, false))) {
            return true;
        }

        return false;
    }


    bool InteractiveUINode::_doRootKeyboardEvent(const std::string_view& type, InteractiveUINode* node, const ALLEGRO_EVENT& event) {
        if (!node->isEnabledTree()) {
            return false;
        }

        KeyboardEvent keyEvent(type, event.keyboard.keycode, event.keyboard.unichar, event.keyboard.modifiers, event.keyboard.repeat);

        if (_focusedNode) {
            if (_focusedNode->dispatchEvent(keyEvent)) {
                return true;
            }
        }

        return _doKeyboardEvent(type, node, keyEvent);
    }


    bool InteractiveUINode::_doKeyboardEvent(const std::string_view& type, UINode* node, const KeyboardEvent& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        InteractiveUINode* inode = node->as<InteractiveUINode>();

        if (_dispatchEvent(inode, event)) {
            return true;
        }

        for (UINode* child = node->UINode::getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
            if (_doKeyboardEvent(type, child, event)) {
                return true;
            }
        }

        return false;
    }


    bool InteractiveUINode::_doTimerEvent(UINode* node, const Event& event) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        if (node->dispatchEvent(event)) {
            return true;
        }

        for (UINode* child = node->UINode::getFirstChildPtr(); child; child = child->getNextSiblingPtr()) {
            if (_doTimerEvent(child, event)) {
                return true;
            }
        }

        return false;
    }


    bool InteractiveUINode::_doDragKeyEvent(const std::string_view& type, UINode* node, const ALLEGRO_EVENT& event, const ALLEGRO_EVENT& mouseEvent) {
        if (!node || !node->isEnabledTree()) {
            return false;
        }

        InteractiveUINode* inode = node->as<InteractiveUINode>();

        KeyboardEvent keyEvent(type, event.keyboard.keycode, event.keyboard.unichar, event.keyboard.modifiers, event.keyboard.repeat);

        if (_dispatchEvent(inode, keyEvent)) {
            return true;
        }

        UINode* child = node->getChildAt(mouseEvent.mouse.x, mouseEvent.mouse.y);
        if (_doDragKeyEvent(type, child, event, mouseEvent)) {
            return true;
        }

        if (_dispatchEvent(inode, keyEvent)) {
            return true;
        }

        return false;
    }


} //namespace algui
