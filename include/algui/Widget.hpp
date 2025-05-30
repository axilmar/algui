#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include "Tree.hpp"


namespace algui {


    class Widget : public Tree<Widget> {
    public:
        Widget() noexcept;

        virtual ~Widget() noexcept;

        float getLeft() const noexcept;

        void setLeft(float v) noexcept;

        float getTop() const noexcept;

        void setTop(float v) noexcept;

        float getRight() const noexcept;

        void setRight(float v) noexcept;

        float getBottom() const noexcept;

        void setBottom(float v) noexcept;

        float getWidth() const noexcept;

        void setWidth(float v) noexcept;

        float getHeight() const noexcept;

        void setHeight(float v) noexcept;

        float getMinWidth() const noexcept;

        void setMinWidth(float v) noexcept;

        float getMinHeight() const noexcept;

        void setMinHeight(float v) noexcept;

        float getMaxWidth() const noexcept;

        void setMaxWidth(float v) noexcept;

        float getMaxHeight() const noexcept;

        void setMaxHeight(float v) noexcept;

        float getScalingX() const noexcept;

        void setScalingX(float v) noexcept;

        float getScalingY() const noexcept;

        void setScalingY(float v) noexcept;

        float getX1() const noexcept;

        float getY1() const noexcept;

        float getX2() const noexcept;

        float getY2() const noexcept;

        bool isVisible() const noexcept;

        bool isVisibleTree() const noexcept;

        void setVisible(bool v) noexcept;

        bool isEnabled() const noexcept;

        bool isEnabledTree() const noexcept;

        void setEnabled(bool v) noexcept;

        bool isHighlighted() const noexcept;

        bool isHighlightedTree() const noexcept;

        void setHighlighted(bool v) noexcept;

        bool isPressed() const noexcept;

        bool isPressedTree() const noexcept;

        void setPressed(bool v) noexcept;

        bool isSelected() const noexcept;

        bool isSelectedTree() const noexcept;

        void setSelected(bool v) noexcept;

        bool isFocused() const noexcept;

        bool isFocusedTree() const noexcept;

        bool setFocused(bool v) noexcept;

        static Widget* getFocused() noexcept;

        bool isFocusable() const noexcept;

        void setFocusable(bool v) noexcept;

        bool isValid() const noexcept;

        bool isValidTree() const noexcept;

        void setValid(bool v) noexcept;

        void invalidateGeometry() noexcept;

        void invalidateLayout() noexcept;

        void render() noexcept;

    protected:
        virtual void onGeometry() noexcept {}

        virtual void onLayout() const noexcept {}

        virtual void onPaint() const noexcept {}

    private:
        float m_left;
        float m_top;
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
        float m_scalingX;
        float m_scalingY;
        float m_screenScalingX;
        float m_screenScalingY;
        bool m_geometryDirty:1;
        bool m_geometryTreeDirty:1;
        bool m_layoutDirty:1;
        bool m_layoutTreeDirty:1;
        bool m_screenGeometryDirty:1;
        bool m_screenGeometryTreeDirty:1;
        bool m_visible:1;
        bool m_enabled:1;
        bool m_highlighted:1;
        bool m_pressed:1;
        bool m_selected:1;
        bool m_focused:1;
        bool m_focusable:1;
        bool m_valid:1;
        bool m_visibleTree:1;
        bool m_enabledTree:1;
        bool m_highlightedTree:1;
        bool m_pressedTree:1;
        bool m_selectedTree:1;
        bool m_focusedTree:1;
        bool m_validTree:1;

        void updateGeometry() noexcept;
        void updateLayout() noexcept;
        void invalidateScreenGeometry() noexcept;
        void updateScreenGeometry(bool force) noexcept;
        void paint() const noexcept;
        void updateVisibleTree() noexcept;
        void updateEnabledTree() noexcept;
        void updateHighlightedTree() noexcept;
        void updatePressedTree() noexcept;
        void updateSelectedTree() noexcept;
        void updateFocusedTree() noexcept;
        void updateValidTree() noexcept;
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
