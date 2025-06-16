#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <list>
#include <any>
#include <string>
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

        const std::string& getId() const;

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

        bool getFlexible() const;

        size_t getDepth() const;

        bool contains(const Widget* wgt) const;

        Widget* get(float x, float y) const;

        bool add(Widget* child, Widget* nextSibling = nullptr);

        bool remove(Widget* child);

        void detach();

        void removeAll();

        void setId(const std::string& id);

        void setX(float x);

        void setY(float y);

        void setWidth(float w);

        void setHeight(float h);

        void setPosition(float x, float y);

        void setSize(float width, float height);

        void setGeometry(float x, float y, float width, float height);

        void setVisible(bool v);

        void setEnabled(bool v);

        void setHighlighted(bool v);

        void setPressed(bool v);

        void setSelected(bool v);

        void setFocused(bool v);

        void setError(bool v);

        void setManaged(bool v);

        void setFlexible(bool v);

        virtual void invalidateLayout();

        void invalidateParentLayout() const;

        void render();

        bool beginDragAndDrop(const std::any& data);

        static void setDragIcon(ALLEGRO_BITMAP* bitmap, int hotPointX = 0, int hotPointY = 0);

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
        std::string m_id;
        Widget* m_parent;
        std::list<Widget*>::iterator m_it;
        std::list<Widget*> m_children;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
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
        bool m_flexible:1;

        void _render();
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
