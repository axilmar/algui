#pragma warning (disable: 4309)
#include "algui/Widget.hpp"

namespace algui {


    static bool acceptsEvents(Widget* wgt) {
        if (!wgt) {
            return false;
        }
        for (; wgt; wgt = wgt->parent) {
            if (!wgt->visible || !wgt->enabled) {
                return false;
            }
        }
        return true;
    }


    static Widget* getChildWithMouse(Widget* wgt) {
        for (Widget* child = wgt->lastChild; child; child = child->prevSibling) {
            if (child->hasMouse) {
                return child;
            }
        }
        return nullptr;
    }


    Widget* Widget::focusWidget = nullptr;
    bool Widget::focusFollowsMouse = false;
    bool Widget::dragAndDrop = false;
    std::any Widget::draggedContent;


    Widget::Widget() :
        id(nullptr),
        parent(nullptr),
        prevSibling(nullptr),
        nextSibling(nullptr),
        firstChild(nullptr),
        lastChild(nullptr),
        x(0),
        y(0),
        width(0),
        height(0),
        xScaling(1),
        yScaling(1),
        focusable(true),
        visible(true),
        enabled(true),
        highlighted(false),
        pressed(false),
        selected(false),
        screenX1(0),
        screenY1(0),
        screenX2(0),
        screenY2(0),
        screenWidth(0),
        screenHeight(0),
        screenXScaling(0),
        screenYScaling(0),
        screenEnabled(true),
        screenHighlighted(false),
        screenPressed(false),
        screenSelected(false),
        hasMouse(false)
    {
    }
    
    Widget::Widget(const Widget& src) :
        id(src.id),
        parent(nullptr),
        prevSibling(nullptr),
        nextSibling(nullptr),
        firstChild(nullptr),
        lastChild(nullptr),
        x(src.x),
        y(src.y),
        width(src.width),
        height(src.height),
        xScaling(src.xScaling),
        yScaling(src.yScaling),
        focusable(src.focusable),
        visible(src.visible),
        enabled(src.enabled),
        highlighted(false),
        pressed(false),
        selected(false),
        screenX1(0),
        screenY1(0),
        screenX2(0),
        screenY2(0),
        screenWidth(0),
        screenHeight(0),
        screenXScaling(0),
        screenYScaling(0),
        screenEnabled(true),
        screenHighlighted(false),
        screenPressed(false),
        screenSelected(false),
        hasMouse(false)
    {
    }


    Widget::~Widget() {
        if (focusWidget == this) {
            focusWidget = nullptr;
        }
        for (Widget* child = firstChild; child; ) {
            Widget* nextChild = child->nextSibling;
            delete child;
            child = nextChild;
        }
    }
    

    Widget& Widget::operator = (const Widget& src) {
        x = src.x;
        y = src.y;
        width = src.width;
        height = src.height;
        focusable = src.focusable;
        visible = src.visible;
        enabled = src.enabled;
        return *this;
    }


    Widget* Widget::getRoot() const {
        Widget* result = const_cast<Widget*>(this);
        for (; result->parent; result = result->parent) {}
        return result;
    }


    void Widget::addChild(Widget* child) {
        if (lastChild) {
            child->prevSibling = lastChild;
            lastChild->nextSibling = child;
            lastChild = child;
        }
        else {
            firstChild = child;
            lastChild = child;
        }
        child->parent = this;
    }
    
    
    void Widget::removeChild(Widget* child) {
        if (child->prevSibling) {
            child->prevSibling->nextSibling = child->nextSibling;
        }
        else {
            firstChild = child->nextSibling;
        }
        if (child->nextSibling) {
            child->nextSibling->prevSibling = child->prevSibling;
        }
        else {
            lastChild = child->prevSibling;
        }
        child->parent = nullptr;
    }


    void Widget::draw() {
        if (visible) {
            if (parent) {
                screenX1 = x * parent->screenXScaling + parent->screenX1;
                screenY1 = y * parent->screenYScaling + parent->screenY1;
                screenWidth = width * parent->screenXScaling;
                screenHeight = height * parent->screenYScaling;
                screenX2 = screenX1 + screenWidth;
                screenY2 = screenY1 + screenHeight;
                screenXScaling = xScaling * parent->screenXScaling;
                screenYScaling = yScaling * parent->screenYScaling;
                screenEnabled = enabled && parent->screenEnabled;
                screenHighlighted = highlighted || parent->screenHighlighted;
                screenPressed = pressed || parent->screenPressed;
                screenSelected = selected || parent->screenSelected;
            }
            else {
                screenX1 = x;
                screenY1 = y;
                screenWidth = width;
                screenHeight = height;
                screenX2 = screenX1 + screenWidth;
                screenY2 = screenY1 + screenHeight;
                screenXScaling = xScaling;
                screenYScaling = yScaling;
                screenEnabled = enabled;
                screenHighlighted = highlighted;
                screenPressed = pressed;
                screenSelected = selected;
            }
            onPaint();
            onLayout();
            for (Widget* child = firstChild; child; child = child->nextSibling) {
                child->draw();
            }
            onPaintOverlay();
        }
    }


    void Widget::focus() {
        if (focusable && focusWidget != this) {
            if (focusWidget) {
                focusWidget->blur();
            }
            focusWidget = this;
            onGotFocus();
            for (Widget* ancestor = parent; ancestor; ancestor = ancestor->parent) {
                ancestor->onDescentantGotFocus();
            }
        }
    }
    
    
    void Widget::blur() {
        if (focusWidget == this) {
            focusWidget = nullptr;
            onLostFocus();
            for (Widget* ancestor = parent; ancestor; ancestor = ancestor->parent) {
                ancestor->onDescentantLostFocus();
            }
        }
    }


    Widget* Widget::getChild(float screenX, float screenY) const {
        for (Widget* child = lastChild; child; child = child->prevSibling) {
            if (child->visible && child->intersects(screenX, screenY)) {
                return child;
            }
        }
        return nullptr;
    }


    bool Widget::dispatch(const ALLEGRO_EVENT& event) {
        switch (event.type) {
            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                if (acceptsEvents(focusWidget)) {
                    return focusWidget->onJoystickAxis(event);
                }
                break;

            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                if (acceptsEvents(focusWidget)) {
                    return focusWidget->onJoystickButtonDown(event);
                }
                break;

            case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                if (acceptsEvents(focusWidget)) {
                    return focusWidget->onJoystickButtonUp(event);
                }
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (acceptsEvents(focusWidget)) {
                    return focusWidget->onKeyDown(event);
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                if (acceptsEvents(focusWidget)) {
                    return focusWidget->onKeyUp(event);
                }
                break;

            case ALLEGRO_EVENT_KEY_CHAR:
                if (acceptsEvents(focusWidget)) {
                    return focusWidget->onKeyChar(event);
                }
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_WARPED:
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                {
                    bool result = false;
                    if (acceptsEvents(this)) {
                        if (event.mouse.dx || event.mouse.dy) {
                            const bool oldHasMouse = hasMouse;
                            hasMouse = intersects(event.mouse.x, event.mouse.y);
                            if (hasMouse && oldHasMouse) {
                                if (!dragAndDrop) {
                                    result = result || onMouseMove(event);
                                }
                                else {
                                    result = result || onDrag(event);
                                }
                            }
                            else if (hasMouse) {
                                if (!dragAndDrop) {
                                    result = result || onMouseEnter(event);
                                }
                                else {
                                    result = result || onDragEnter(event);
                                }
                            }
                            else if (oldHasMouse) {
                                if (!dragAndDrop) {
                                    result = result || onMouseLeave(event);
                                }
                                else {
                                    result = result || onDragLeave(event);
                                }
                            }
                        }
                    }
                    if (acceptsEvents(focusWidget)) {
                        if (event.mouse.dz || event.mouse.dw) {
                            if (focusWidget) {
                                result = result || focusWidget->onMouseWheel(event);
                            }
                        }
                    }
                    return result;
                }

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (!dragAndDrop) {
                    if (acceptsEvents(this)) {
                        hasMouse = intersects(event.mouse.x, event.mouse.y);
                        return onMouseButtonDown(event);
                    }
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (acceptsEvents(this)) {
                    hasMouse = intersects(event.mouse.x, event.mouse.y);
                    if (!dragAndDrop) {
                        return onMouseButtonUp(event);
                    }
                    else {
                        const bool result = onDrop(event);
                        dragAndDrop = false;
                        draggedContent = std::any();
                        return result;
                    }
                }
                break;

            case ALLEGRO_EVENT_TIMER:
                return onTimer(event);
        }

        return false;
    }


    bool Widget::onMouseEnter(const ALLEGRO_EVENT& event) {
        Widget* child = getChild(event.mouse.x, event.mouse.y);
        if (child) {
            child->hasMouse = true;
            if (child->enabled) {
                return child->onMouseEnter(event);
            }
        }
        if (focusFollowsMouse && focusable && enabled) {
            focus();
        }
        return false;
    }


    bool Widget::onMouseMove(const ALLEGRO_EVENT& event) {
        Widget* oldMouseChild = getChildWithMouse(this);
        Widget* newMouseChild = getChild(event.mouse.x, event.mouse.y);

        if (newMouseChild == oldMouseChild) {
            if (newMouseChild && newMouseChild->enabled) {
                return newMouseChild->onMouseMove(event);
            }
        }

        bool result = false;

        if (oldMouseChild) {
            oldMouseChild->hasMouse = false;
            if (oldMouseChild->enabled) {
                result = result || oldMouseChild->onMouseLeave(event);
            }
        }

        if (newMouseChild) {
            newMouseChild->hasMouse = true;
            if (newMouseChild->enabled) {
                result = result || newMouseChild->onMouseEnter(event);
            }
        }

        return result;
    }


    bool Widget::onMouseLeave(const ALLEGRO_EVENT& event) {
        if (focusFollowsMouse && hasFocus()) {
            blur();
        }
        Widget* child = getChildWithMouse(this);
        if (child) {
            child->hasMouse = false;
            if (child->enabled) {
                return child->onMouseLeave(event);
            }
        }
        return false;
    }



    bool Widget::onMouseButtonDown(const ALLEGRO_EVENT& event) {
        Widget* child = getChild(event.mouse.x, event.mouse.y);
        if (child && child->enabled) {
            return child->onMouseButtonDown(event);
        }
        return false;
    }



    bool Widget::onMouseButtonUp(const ALLEGRO_EVENT& event) {
        Widget* child = getChild(event.mouse.x, event.mouse.y);
        if (child && child->enabled) {
            return child->onMouseButtonUp(event);
        }
        return false;
    }


    bool Widget::onTimer(const ALLEGRO_EVENT& event) {
        bool result = false;
        for (Widget* child = firstChild; child; child = child->nextSibling) {
            if (child->enabled) {
                result = result || child->onTimer(event);
            }
        }
        return result;
    }


    bool Widget::onDragEnter(const ALLEGRO_EVENT& event) {
        Widget* child = getChild(event.mouse.x, event.mouse.y);
        if (child) {
            child->hasMouse = true;
            if (child->enabled) {
                return child->onDragEnter(event);
            }
        }
        return false;
    }


    bool Widget::onDrag(const ALLEGRO_EVENT& event) {
        Widget* oldMouseChild = getChildWithMouse(this);
        Widget* newMouseChild = getChild(event.mouse.x, event.mouse.y);

        if (newMouseChild == oldMouseChild) {
            if (newMouseChild && newMouseChild->enabled) {
                return newMouseChild->onDrag(event);
            }
        }

        bool result = false;

        if (oldMouseChild) {
            oldMouseChild->hasMouse = false;
            if (oldMouseChild->enabled) {
                result = result || oldMouseChild->onDragLeave(event);
            }
        }

        if (newMouseChild) {
            newMouseChild->hasMouse = true;
            if (newMouseChild->enabled) {
                result = result || newMouseChild->onDragEnter(event);
            }
        }

        return result;
    }


    bool Widget::onDragLeave(const ALLEGRO_EVENT& event) {
        Widget* child = getChildWithMouse(this);
        if (child) {
            child->hasMouse = false;
            if (child->enabled) {
                return child->onDragLeave(event);
            }
        }
        return false;
    }



    bool Widget::onDrop(const ALLEGRO_EVENT& event) {
        Widget* child = getChild(event.mouse.x, event.mouse.y);
        if (child && child->enabled) {
            return child->onDrop(event);
        }
        return false;
    }


} //namespace algui
