#include <algorithm>
#include "algui/Widget.hpp"


namespace algui {


    Widget::Widget() 
        : m_parent(nullptr)
        , m_x(0)
        , m_y(0)
        , m_width(0)
        , m_height(0)
        , m_x1(0)
        , m_y1(0)
        , m_x2(0)
        , m_y2(0)
        , m_visible(true)
    {
    }


    Widget::~Widget() {
        detach();
    }


    Widget* Widget::getParent() const {
        return m_parent;
    }


    Widget* Widget::getPrevSibling() const {
        return m_parent && this != m_parent->m_children.front() ? *std::prev(m_it) : nullptr;
    }


    Widget* Widget::getNextSibling() const {
        return m_parent && this != m_parent->m_children.back() ? *std::next(m_it) : nullptr;
    }


    Widget* Widget::getFirstChild() const {
        return m_children.empty() ? nullptr : m_children.front();
    }


    Widget* Widget::getLastChild() const {
        return m_children.empty() ? nullptr : m_children.back();
    }


    Widget* Widget::getRoot() const {
        Widget* wgt = const_cast<Widget*>(this);
        for(; wgt->m_parent; wgt = wgt->m_parent) {}
        return wgt;
    }


    float Widget::getX() const {
        return m_x;
    }


    float Widget::getY() const {
        return m_y;
    }


    float Widget::getWidth() const {
        return m_width;
    }


    float Widget::getHeight() const {
        return m_height;
    }


    float Widget::getX1() const {
        return m_x1;
    }


    float Widget::getY1() const {
        return m_y1;
    }

    
    float Widget::getX2() const {
        return m_x2;
    }


    float Widget::getY2() const {
        return m_y2;
    }


    bool Widget::getVisible() const {
        return m_visible;
    }


    bool Widget::contains(const Widget* wgt) const {
        for (; wgt; wgt = wgt->m_parent) {
            if (wgt == this) {
                return true;
            }
        }
        return false;
    }


    bool Widget::add(Widget* child, Widget* nextSibling) {
        if (!child || child->m_parent || child->contains(this) || (nextSibling && nextSibling->m_parent != this)) {
            return false;
        }
        child->m_parent = this;
        child->m_it = m_children.insert(nextSibling ? nextSibling->m_it : m_children.end(), child);
        return true;
    }


    bool Widget::remove(Widget* child) {
        if (!child || child->m_parent != this) {
            return false;
        }
        child->m_parent = nullptr;
        m_children.erase(child->m_it);
        return true;
    }


    void Widget::detach() {
        if (m_parent) {
            m_parent->remove(this);
        }
    }


    void Widget::setX(float x) {
        m_x = x;
    }


    void Widget::setY(float y) {
        m_y = y;
    }


    void Widget::setWidth(float w) {
        m_width = w >= 0 ? w : 0;
    }


    void Widget::setHeight(float h) {
        m_height = h >= 0 ? h : 0;
    }


    void Widget::setVisible(bool v) {
        m_visible = v;
    }


    void Widget::render() {
        if (m_visible) {
            m_x1 = m_x;
            m_y1 = m_y;
            if (m_parent) {
                m_x1 += m_parent->m_x1;
                m_y1 += m_parent->m_y1;
            }
            m_x2 = m_x1 + m_width;
            m_y2 = m_y1 + m_height;
            paint();
            for (Widget* child : m_children) {
                child->render();
            }
        }
    }


} //namespace algui
