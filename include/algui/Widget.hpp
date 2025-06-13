#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <list>


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

        bool contains(const Widget* wgt) const;

        bool add(Widget* child, Widget* nextSibling = nullptr);

        bool remove(Widget* child);

        void detach();

        void setX(float x);

        void setY(float y);

        void setWidth(float w);

        void setHeight(float h);

        void setVisible(bool v);

        void render();

    protected:
        virtual void paint() const = 0;

    private:
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
        bool m_visible;
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
