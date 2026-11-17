#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include <memory>
#include <vector>
#include "Painter.hpp"


namespace algui {


    class Widget;


    using WidgetPtr = std::shared_ptr<Widget>;


    using WidgetContainer = std::vector<WidgetPtr>;


    class Widget : public std::enable_shared_from_this<Widget> {
    public:
        Widget();

        WidgetPtr parent() const;
        const WidgetContainer& children() const;
        size_t indexOf(const WidgetPtr& child) const;
        bool inTree(const WidgetPtr& root) const;
        virtual void add(const WidgetPtr& child, size_t index = -1);
        virtual void remove(const WidgetPtr& child);
        void detach();

        float x() const;
        float y() const;
        float width() const;
        float height() const;
        float left() const;
        float top() const;
        float right() const;
        float bottom() const;
        virtual void setPositionAndSize(float x, float y, float width, float height);
        void setPosition(float x, float y);
        void setSize(float width, float height);
        void setX(float x);
        void setY(float y);
        void setWidth(float width);
        void setHeight(float height);

        bool visible() const;
        bool visibleTree() const;
        void setVisible(bool visible);

        bool enabled() const;
        bool enabledTree() const;
        void setEnabled(bool enabled);

        static WidgetPtr focusedWidget();
        bool focused() const;
        bool focusedTree() const;
        virtual bool setFocused(bool focused);

        void invalidatePreferredSize();
        void invalidateLayout();
        virtual void update();
        virtual void draw(const PainterPtr& painter);
        virtual void draw(const PainterPtr& painter, float left, float top, float right, float bottom);

        WidgetPtr childFromPoint(float x, float y) const;
        WidgetPtr descentantFromPoint(float x, float y) const;
        WidgetPtr widgetFromPoint(float x, float y) const;

    protected:
        virtual void setPreferredSize() {}

        virtual void doLayout() {}

        virtual void paintBackground(const PainterPtr& painter) const = 0;

        virtual void drawChildren(const PainterPtr& painter) const;

        virtual void drawChildren(const PainterPtr& painter, float left, float top, float right, float bottom) const;

        virtual void paintForeground(const PainterPtr& painter) const {}

        virtual bool wantsFocus() const;

        virtual void lostFocus() {}

        virtual void gotFocus() {}

        virtual bool hitTest(float x, float y) const;

    private:
        std::weak_ptr<Widget> m_parent;
        WidgetContainer m_children;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_left;
        float m_top;
        float m_right;
        float m_bottom;
        bool m_validPreferredSize;
        bool m_validPreferredSizeTree;
        bool m_validLayout;
        bool m_validLayoutTree;
        bool m_validRect;
        bool m_validRectTree;
        bool m_visible;
        bool m_visibleTree;
        bool m_enabled;
        bool m_enabledTree;
        bool m_focused;
        bool m_focusedTree;
        bool m_focusedContainer;

        void invalidateRect();
        void invalidateParent();

        void updatePreferredSize();
        void updateLayout();
        void updateRect(Widget* parent);
        
        void updateVisibleTreeFlag(Widget* parent);
        void updateVisibleTree(Widget* parent);
        
        void updateEnabledTreeFlag(Widget* parent);
        void updateEnabledTree(Widget* parent);
        
        void updateFocusedTreeFlag(Widget* parent);
        void updateFocusedTree(Widget* parent);

        void updateTreeFlags(Widget* parent);

        void updateAncestorFocusedContainer(bool focusedContainer);
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
