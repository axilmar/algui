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

        void invalidatePreferredSize();
        void invalidateLayout();
        virtual void update();
        void draw(const PainterPtr& painter);

    protected:
        virtual void onSetPreferredSize() {
        }

        virtual void onSetLayout() {
        }

        virtual void onPaint(const PainterPtr& painter) const = 0;

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

        void updatePreferredSize();
        void updateLayout();
        void invalidateRect();
        void updateRect(Widget* parent);
        void invalidateParent();
        void updateVisibleTreeFlag(Widget* parent);
        void updateVisibleTree(Widget* parent);
        void updateEnabledTreeFlag(Widget* parent);
        void updateEnabledTree(Widget* parent);
        void updateEnabledVisibleTree(Widget* parent);
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
