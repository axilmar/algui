#include <algorithm>
#include <stdexcept>
#include "Widget.hpp"
#include "utility.hpp"


namespace algui {


    // INTERNAL ---------------------------------------------------------------


    static std::weak_ptr<Widget> focusedWidget;


    // PUBLIC -----------------------------------------------------------------


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
        , m_focused(false)
        , m_focusedTree(false)
        , m_focusedContainer(false)
    {
    }


    Widget::~Widget() {
        if (m_layout) {
            m_layout->removeWidget(shared_from_this());
        }
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


    bool Widget::inTree(const WidgetPtr& root) const {
        if (!root) {
            throw std::invalid_argument("Widget::inTree(root): null root.");
        }
        
        for (WidgetPtr wgt = const_cast<Widget*>(this)->shared_from_this(); wgt; wgt = wgt->parent()) {
            if (wgt == root) {
                return true;
            }
        }

        return false;
    }


    void Widget::add(const WidgetPtr& child, size_t index) {
        if (!child) {
            throw std::invalid_argument("Widget::add(child): null child.");
        }
        
        if (child->parent().get()) {
            throw std::invalid_argument("Widget::add(child): invalid child.");
        }
        
        if (inTree(child)) {
            throw std::invalid_argument("Widget::add(child): invalid tree.");
        }
        
        if (index > m_children.size()) {
            index = m_children.size();
        }

        child->m_parent = shared_from_this();
        m_children.insert(std::next(m_children.begin(), index), child);

        child->invalidateRect();
        invalidatePreferredSize();        
        invalidateLayout();
        
        child->updateTreeFlags(this);

        WidgetPtr currentFocused = algui::focusedWidget.lock();
        if (currentFocused && currentFocused->inTree(child)) {
            child->updateAncestorFocusedContainer(true);
        }
    }


    void Widget::remove(const WidgetPtr& child) {
        if (!child) {
            throw std::invalid_argument("Widget::remove(child): null child.");
        }
        
        if (child->parent().get() != this) {
            throw std::invalid_argument("Widget::remove(child): invalid child.");
        }

        WidgetPtr currentFocused = algui::focusedWidget.lock();
        if (currentFocused && currentFocused->inTree(child)) {
            currentFocused->setFocused(false);
        }
        
        child->m_parent.reset();
        m_children.erase(std::find(m_children.begin(), m_children.end(), child));

        invalidatePreferredSize();
        invalidateLayout();
        
        child->updateTreeFlags(nullptr);
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
            if (!enabled) {
                WidgetPtr currentFocused = algui::focusedWidget.lock();
                if (currentFocused && currentFocused->inTree(shared_from_this())) {
                    currentFocused->setFocused(false);
                }
            }
            m_enabled = enabled;
            updateEnabledTree(parent().get());
        }
    }


    WidgetPtr Widget::focusedWidget() {
        return algui::focusedWidget.lock();
    }


    bool Widget::focused() const {
        return m_focused;
    }


    bool Widget::focusedTree() const {
        return m_focusedTree;
    }


    bool Widget::setFocused(bool focused) {
        bool result = false;

        if (focused != m_focused) {
            if (focused) {
                if (m_enabledTree && wantsFocus()) {
                    WidgetPtr currentFocused = algui::focusedWidget.lock();
                    if (currentFocused) {
                        currentFocused->setFocused(false);
                    }

                    algui::focusedWidget = shared_from_this();
                    m_focused = true;
                    updateFocusedTree(parent().get());
                    updateAncestorFocusedContainer(true);
                    gotFocus();
                    result = true;
                }
            }

            else {
                algui::focusedWidget.reset();
                m_focused = false;
                updateFocusedTree(parent().get());
                updateAncestorFocusedContainer(false);
                lostFocus();
                result = true;
            }
        }
        
        return result;
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
            paintBackground(painter);
            drawChildren(painter);
            paintForeground(painter);
        }
    }


    void Widget::draw(const PainterPtr& painter, float left, float top, float right, float bottom) {
        if (m_visibleTree && checkRectIntersection(left, top, right, bottom, m_left, m_top, m_right, m_bottom)) {
            paintBackground(painter);
            drawChildren(painter, left, top, right, bottom);
            paintForeground(painter);
        }
    }


    WidgetPtr Widget::childFromPoint(float x, float y) const {
        WidgetPtr result;

        for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
            const WidgetPtr& child = *it;
            if (child->hitTest(x, y)) {
                result = child;
                break;
            }
        }

        return result;
    }
        
        
    WidgetPtr Widget::descentantFromPoint(float x, float y) const {
        WidgetPtr result;

        WidgetPtr child = childFromPoint(x, y);
        if (child) {
            result = child->widgetFromPoint(x, y);
            if (!result) {
                result = child;
            }
        }

        return result;
    }


    WidgetPtr Widget::widgetFromPoint(float x, float y) const {
        WidgetPtr result = descentantFromPoint(x, y);
        if (!result && hitTest(x, y)) {
            result = const_cast<Widget*>(this)->shared_from_this();
        }
        return result;
    }


    const LayoutPtr& Widget::layout() const {
        return m_layout;
    }
        
        
    void Widget::setLayout(const LayoutPtr& layout) {
        if (layout != m_layout) {
            if (m_layout) {
                m_layout->removeWidget(shared_from_this());
            }
            m_layout = layout;
            if (m_layout) {
                m_layout->addWidget(shared_from_this());
                invalidatePreferredSize();
                invalidateLayout();
            }
        }
    }


    // PROTECTED --------------------------------------------------------------


    void Widget::doLayout() {
        if (m_layout) {
            m_layout->apply(shared_from_this());
        }
    }


    void Widget::drawChildren(const PainterPtr& painter) const {
        for (const WidgetPtr& child : m_children) {
            child->draw(painter);
        }
    }


    void Widget::drawChildren(const PainterPtr& painter, float left, float top, float right, float bottom) const {
        for (const WidgetPtr& child : m_children) {
            child->draw(painter, left, top, right, bottom);
        }
    }


    bool Widget::wantsFocus() const {
        return false;
    }


    bool Widget::hitTest(float x, float y) const {
        return m_visibleTree && x >= m_left && y >= m_top && x < m_right && y < m_bottom;
    }


    // PRIVATE ----------------------------------------------------------------


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
        
        
    void Widget::invalidateParent() {
        WidgetPtr parent = this->parent();
        if (parent) {
            parent->invalidatePreferredSize();
            parent->invalidateLayout();
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
            setPreferredSize();
            m_validPreferredSize = true;
        }
    }


    void Widget::updateLayout() {
        if (!m_validLayout) {
            doLayout();
            m_validLayout = true;
        }
        if (!m_validLayoutTree) {
            for (const WidgetPtr& child : m_children) {
                child->updateLayout();
            }
            m_validLayoutTree = true;
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


    void Widget::updateFocusedTreeFlag(Widget* parent) {
        m_focusedTree = m_focused && (!parent || parent->m_focusedTree);
    }


    void Widget::updateFocusedTree(Widget* parent) {
        updateFocusedTreeFlag(parent);
        for (const WidgetPtr& child : m_children) {
            child->updateFocusedTree(this);
        }
    }


    void Widget::updateTreeFlags(Widget* parent) {
        updateVisibleTreeFlag(parent);
        updateEnabledTreeFlag(parent);
        updateFocusedTreeFlag(parent);
        for (const WidgetPtr& child : m_children) {
            child->updateTreeFlags(this);
        }
    }


    void Widget::updateAncestorFocusedContainer(bool focusedContainer) {
        for (WidgetPtr ancestor = parent(); ancestor; ancestor = ancestor->parent()) {
            if (focusedContainer != ancestor->m_focusedContainer) {
                ancestor->m_focusedContainer = focusedContainer;
            }
            else {
                break;
            }
        }
    }


} //namespace algui
