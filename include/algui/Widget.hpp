#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <list>
#include <any>
#include <allegro5/allegro.h>


namespace algui {


    class Widget {
    public:
        Widget();

        Widget(const Widget&) = delete;
        Widget(Widget&&) = delete;

        virtual ~Widget();

        Widget& operator =(const Widget&) = delete;
        Widget& operator =(Widget&&) = delete;

        Widget* getParent() const;

        Widget* getPrevSibling() const;

        Widget* getNextSibling() const;

        Widget* getFirstChild() const;

        Widget* getLastChild() const;

        Widget* getRoot() const;

        float getX() const;

        float getY() const;

        float getWidth() const;

        float getHeight() const;

        float getX1() const;

        float getY1() const;

        float getX2() const;

        float getY2() const;

        bool getVisible() const;

        bool getEnabled() const;

        bool getHighlighted() const;

        bool getPressed() const;

        bool getSelected() const;

        bool getFocused() const;

        bool getError() const;

        bool getTreeVisible() const;

        bool getTreeEnabled() const;
        
        bool getTreeHighlighed() const;

        bool getTreePressed() const;

        bool getTreeSelected() const;

        bool getTreeFocused() const;

        bool getTreeError() const;

        bool getManaged() const;

        float getMinWidth() const;

        float getMinHeight() const;

        float getMaxWidth() const;

        float getMaxHeight() const;

        bool contains(const Widget* wgt) const;

        Widget* get(float x, float y) const;

        bool add(Widget* child, Widget* nextSibling = nullptr);

        bool remove(Widget* child);

        void detach();

        void removeAll();

        void setX(float x);

        void setY(float y);

        void setWidth(float w);

        void setHeight(float h);

        void setPosition(float x, float y);

        void setSize(float width, float height);

        void setGeometry(float x, float y, float width, float height);

        void setMinWidth(float minW);

        void setMinHeight(float minH);

        void setMaxWidth(float maxW);

        void setMaxHeight(float maxH);

        void setMinSize(float minW, float minH);

        void setMaxSize(float maxW, float maxH);

        void setMinMaxSize(float minW, float minH, float maxW, float maxH);

        void setSize(float width, float height, float minW, float minH, float maxW, float maxH);

        void setGeometry(float x, float y, float width, float height, float minW, float minH, float maxW, float maxH);

        void setVisible(bool v);

        void setEnabled(bool v);

        void setHighlighted(bool v);

        void setPressed(bool v);

        void setSelected(bool v);

        void setFocused(bool v);

        void setError(bool v);

        void setManaged(bool v);

        virtual void invalidateLayout();

        void invalidateParentLayout() const;

        void render();

        bool beginDragAndDrop(const std::any& data);

        bool doEvent(const ALLEGRO_EVENT& event);

    protected:
        virtual void layout() const {}

        virtual void paint() const = 0;

        virtual void gotFocus() {}

        virtual void lostFocus() {}

        virtual bool intersects(float x, float y) const;

        virtual bool mouseButtonDown(const ALLEGRO_EVENT& event);

        virtual bool mouseButtonUp(const ALLEGRO_EVENT& event);

        virtual bool mouseEnter(const ALLEGRO_EVENT& event);

        virtual bool mouseMove(const ALLEGRO_EVENT& event);

        virtual bool mouseLeave(const ALLEGRO_EVENT& event);

        virtual bool mouseWheel(const ALLEGRO_EVENT& event);

        virtual bool keyDown(const ALLEGRO_EVENT& event);

        virtual bool keyUp(const ALLEGRO_EVENT& event);

        virtual bool keyChar(const ALLEGRO_EVENT& event);

        virtual bool unusedKeyDown(const ALLEGRO_EVENT& event);

        virtual bool unusedKeyUp(const ALLEGRO_EVENT& event);

        virtual bool unusedKeyChar(const ALLEGRO_EVENT& event);

        virtual bool drop(const ALLEGRO_EVENT& event);

        virtual bool dragEnter(const ALLEGRO_EVENT& event);

        virtual bool drag(const ALLEGRO_EVENT& event);

        virtual bool dragLeave(const ALLEGRO_EVENT& event);

        virtual bool dragWheel(const ALLEGRO_EVENT& event);

        virtual bool dragKeyDown(const ALLEGRO_EVENT& event);

        virtual bool dragKeyUp(const ALLEGRO_EVENT& event);

        virtual bool dragKeyChar(const ALLEGRO_EVENT& event);

    private:
        Widget* m_parent;
        std::list<Widget*>::iterator m_it;
        std::list<Widget*> m_children;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_minWidth;
        float m_minHeight;
        float m_maxWidth;
        float m_maxHeight;
        float m_x1;
        float m_y1;
        float m_x2;
        float m_y2;
        bool m_visible:1;
        bool m_enabled:1;
        bool m_highlighted:1;
        bool m_pressed:1;
        bool m_selected:1;
        bool m_focused:1;
        bool m_error:1;
        bool m_treeVisible:1;
        bool m_treeEnabled:1;
        bool m_treeHighlighted:1;
        bool m_treePressed:1;
        bool m_treeSelected:1;
        bool m_treeFocused:1;
        bool m_treeError:1;
        bool m_layout:1;
        bool m_doingLayout:1;
        bool m_managed:1;
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
