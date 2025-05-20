#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include "allegro5/allegro.h"


namespace algui {


    class Widget {
    public:
        const char* id;
        Widget* parent;
        Widget* prevSibling;
        Widget* nextSibling;
        Widget* firstChild;
        Widget* lastChild;
        float x;
        float y;
        float screenX;
        float screenY;
        float width;
        float height;
        bool focusable : 1;
        bool visible : 1;
        bool enabled : 1;
        bool highlighted : 1;
        bool pressed : 1;
        bool selected : 1;
        bool screenEnabled : 1;
        bool screenHighlighted : 1;
        bool screenPressed : 1;
        bool screenSelected : 1;
        bool hasMouse : 1;

        static Widget* focusWidget;
        static bool focusFollowsMouse;

        Widget();
        Widget(const Widget& src);
        Widget(Widget&& src) = delete;
        virtual ~Widget();
        Widget& operator = (const Widget& src);
        Widget& operator = (Widget&& src) = delete;

        Widget* getRoot() const;
        void addChild(Widget* child);
        void removeChild(Widget* child);
        
        void draw();

        bool hasFocus() const { return focusWidget == this; }
        void focus();
        void blur();
        
        virtual bool intersects(float screenX, float screenY) const {
            return screenX >= this->screenX && screenY >= this->screenY && screenX < this->screenX + this->width && screenY < this->screenY + this->height;
        }

        Widget* getChild(float screenX, float screenY) const;

        virtual bool dispatch(const ALLEGRO_EVENT& event);

        virtual void onPaint() const = 0;
        virtual void onPaintOverlay() const {}
        virtual void onGotFocus() = 0;
        virtual void onLostFocus() = 0;
        virtual void onDescentantGotFocus() = 0;
        virtual void onDescentantLostFocus() = 0;

        virtual bool onMouseEnter(const ALLEGRO_EVENT& event);
        virtual bool onMouseMove(const ALLEGRO_EVENT& event);
        virtual bool onMouseLeave(const ALLEGRO_EVENT& event);
        virtual bool onMouseButtonDown(const ALLEGRO_EVENT& event);
        virtual bool onMouseButtonUp(const ALLEGRO_EVENT& event);
        virtual bool onTimer(const ALLEGRO_EVENT& event);

        virtual bool onMouseWheel(const ALLEGRO_EVENT& event) { return false; }
        virtual bool onKeyDown(const ALLEGRO_EVENT& event) { return false; }
        virtual bool onKeyUp(const ALLEGRO_EVENT& event) { return false; }
        virtual bool onKeyChar(const ALLEGRO_EVENT& event) { return false; }
        virtual bool onJoystickAxis(const ALLEGRO_EVENT& event) { return false; }
        virtual bool onJoystickButtonDown(const ALLEGRO_EVENT& event) { return false; }
        virtual bool onJoystickButtonUp(const ALLEGRO_EVENT& event) { return false; }
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
