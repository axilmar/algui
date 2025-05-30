#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include "Tree.hpp"


namespace algui {


    class Widget : public Tree<Widget> {
    public:
        Widget() noexcept;

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

        void setVisible(bool v) noexcept;

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
        bool m_visible:1;
        bool m_geometryDirty:1;
        bool m_geometryTreeDirty:1;
        bool m_layoutDirty:1;
        bool m_layoutTreeDirty:1;
        bool m_screenGeometryDirty:1;
        bool m_screenGeometryTreeDirty:1;

        void updateGeometry() noexcept;
        void updateLayout() noexcept;
        void invalidateScreenGeometry() noexcept;
        void updateScreenGeometry(bool force) noexcept;
        void paint() const noexcept;
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
