#include <algorithm>
#include <stdexcept>
#include "Widget.hpp"


namespace algui {


    Widget::Widget()
        : m_x(0)
        , m_y(0)
        , m_width(0)
        , m_height(0)
        , m_left(0)
        , m_top(0)
        , m_right(0)
        , m_bottom(0)
        , m_validPreferredSize(false)
        , m_validPreferredSizeTree(false)
        , m_validLayout(false)
        , m_validLayoutTree(false)
        , m_validRect(false)
        , m_validRectTree(false)
        , m_visible(true)
        , m_visibleTree(true)
        , m_enabled(true)
        , m_enabledTree(true)
    {
    }


    WidgetPtr Widget::parent() const {
        return m_parent.lock();
    }


    const WidgetContainer& Widget::children() const {
        return m_children;
    }


    size_t Widget::indexOf(const WidgetPtr& child) const {
        if (!child) {
            throw std::invalid_argument("Widget::indexOf(child): null child.");
        }

        auto it = std::find(m_children.begin(), m_children.end(), child);
        
        if (it != m_children.end()) {
            return std::distance(m_children.begin(), it);
        }
        
        return -1;
    }


    void Widget::add(const WidgetPtr& child, size_t index) {
        if (!child) {
            throw std::invalid_argument("Widget::add(child): null child.");
        }
        
        if (child->parent().get()) {
            throw std::invalid_argument("Widget::add(child): invalid child.");
        }
        
        if (index > m_children.size()) {
            index = m_children.size();
        }

        child->m_parent = shared_from_this();
        m_children.insert(std::next(m_children.begin(), index), child);
        child->invalidateRect();
        invalidatePreferredSize();
        invalidateLayout();
        child->updateEnabledVisibleTree(this);
    }


    void Widget::remove(const WidgetPtr& child) {
        if (!child) {
            throw std::invalid_argument("Widget::remove(child): null child.");
        }
        
        if (child->parent().get() != this) {
            throw std::invalid_argument("Widget::remove(child): invalid child.");
        }
        
        child->m_parent.reset();
        m_children.erase(std::find(m_children.begin(), m_children.end(), child));
        invalidatePreferredSize();
        invalidateLayout();
        child->updateEnabledVisibleTree(nullptr);
    }


    void Widget::detach() {
        WidgetPtr parent = this->parent();
        if (parent) {
            parent->remove(shared_from_this());
        }
    }


    float Widget::x() const {
        return m_x;
    }


    float Widget::y() const {
        return m_y;
    }


    float Widget::width() const {
        return m_width;
    }


    float Widget::height() const {
        return m_height;
    }


    float Widget::left() const {
        return m_left;
    }


    float Widget::top() const {
        return m_top;
    }


    float Widget::right() const {
        return m_right;
    }


    float Widget::bottom() const {
        return m_bottom;
    }


    void Widget::setPositionAndSize(float x, float y, float width, float height) {
        if (width < 0) {
            width = 0;
        }

        if (height < 0) {
            height = 0;
        }

        const bool positionChanged = x != m_x || y != m_y;
        const bool sizeChanged = width != m_width || height != m_height;

        if (positionChanged || sizeChanged) {
            m_x = x;
            m_y = y;
            m_width = width;
            m_height = height;
            invalidateRect();

            if (sizeChanged) {
                invalidateLayout();
            }

            invalidateParent();
        }
    }


    void Widget::setPosition(float x, float y) {
        setPositionAndSize(x, y, m_width, m_height);
    }


    void Widget::setSize(float width, float height) {
        setPositionAndSize(m_x, m_y, width, height);
    }


    void Widget::setX(float x) {
        setPosition(x, m_y);
    }


    void Widget::setY(float y) {
        setPosition(m_x, y);
    }


    void Widget::setWidth(float width) {
        setSize(width, m_height);
    }


    void Widget::setHeight(float height) {
        setSize(m_width, height);
    }


    bool Widget::visible() const {
        return m_visible;
    }


    bool Widget::visibleTree() const {
        return m_visibleTree;
    }


    void Widget::setVisible(bool visible) {
        if (visible != m_visible) {
            m_visible = visible;
            updateVisibleTree(parent().get());
            invalidateParent();
        }
    }


    bool Widget::enabled() const {
        return m_enabled;
    }


    bool Widget::enabledTree() const {
        return m_enabledTree;
    }


    void Widget::setEnabled(bool enabled) {
        if (enabled != m_enabled) {
            m_enabled = enabled;
            updateEnabledTree(parent().get());
        }
    }


    void Widget::invalidatePreferredSize() {
        if (m_validPreferredSize) {
            m_validPreferredSize = false;
            for (WidgetPtr ancestor = parent(); ancestor; ancestor->parent()) {
                if (ancestor->m_validPreferredSizeTree) {
                    ancestor->m_validPreferredSizeTree = false;
                }
                else {
                    break;
                }
            }
        }
    }


    void Widget::invalidateLayout() {
        if (m_validLayout) {
            m_validLayout = false;
            for (WidgetPtr ancestor = parent(); ancestor; ancestor->parent()) {
                if (ancestor->m_validLayoutTree) {
                    ancestor->m_validLayoutTree = false;
                }
                else {
                    break;
                }
            }
        }
    }


    void Widget::update() {
        updatePreferredSize();
        updateLayout();
        updateRect(parent().get());
    }


    void Widget::draw(const PainterPtr& painter) {
        if (m_visibleTree) {
            onPaint(painter);
            for (const WidgetPtr& child : m_children) {
                child->draw(painter);
            }
        }
    }


    void Widget::updatePreferredSize() {
        if (!m_validPreferredSizeTree) {
            for (const WidgetPtr& child : m_children) {
                child->updatePreferredSize();
            }
            m_validPreferredSizeTree = true;
        }
        if (!m_validPreferredSize) {
            onSetPreferredSize();
            m_validPreferredSize = true;
        }
    }


    void Widget::updateLayout() {
        if (!m_validLayout) {
            onSetLayout();
            m_validLayout = true;
        }
        if (!m_validLayoutTree) {
            for (const WidgetPtr& child : m_children) {
                child->updateLayout();
            }
            m_validLayoutTree = true;
        }
    }


    void Widget::invalidateRect() {
        if (m_validRect) {
            m_validRect = false;
            for (WidgetPtr ancestor = parent(); ancestor; ancestor->parent()) {
                if (ancestor->m_validRectTree) {
                    ancestor->m_validRectTree = false;
                }
                else {
                    break;
                }
            }
        }
    }
        
        
    void Widget::updateRect(Widget* parent) {
        if (!m_validRect) {
            if (parent) {
                m_left = parent->m_left + m_x;
                m_top = parent->m_top + m_y;
            }
            else {
                m_left = m_x;
                m_top = m_y;
            }
            m_right = m_left + m_width;
            m_bottom = m_top + m_height;
            m_validRect = true;
            m_validRectTree = false;
        }
        if (!m_validRectTree) {
            for (const WidgetPtr& child : m_children) {
                child->updateRect(this);
            }
            m_validRectTree = true;
        }
    }


    void Widget::invalidateParent() {
        WidgetPtr parent = this->parent();
        if (parent) {
            parent->invalidatePreferredSize();
            parent->invalidateLayout();
        }
    }


    void Widget::updateVisibleTreeFlag(Widget* parent) {
        m_visibleTree = m_visible && (!parent || parent->m_visibleTree);
    }


    void Widget::updateVisibleTree(Widget* parent) {
        updateVisibleTreeFlag(parent);
        for (const WidgetPtr& child : m_children) {
            child->updateVisibleTree(this);
        }
    }


    void Widget::updateEnabledTreeFlag(Widget* parent) {
        m_enabledTree = m_enabled && (!parent || parent->m_enabledTree);
    }


    void Widget::updateEnabledTree(Widget* parent) {
        updateEnabledTreeFlag(parent);
        for (const WidgetPtr& child : m_children) {
            child->updateEnabledTree(this);
        }
    }


    void Widget::updateEnabledVisibleTree(Widget* parent) {
        updateEnabledTreeFlag(parent);
        updateVisibleTreeFlag(parent);
        for (const WidgetPtr& child : m_children) {
            child->updateEnabledVisibleTree(this);
        }
    }


} //namespace algui
