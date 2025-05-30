#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP


#include "Tree.hpp"


namespace algui {


    class Widget : public Tree<Widget> {
    public:
        Widget();

        float getLeft() const;

        void setLeft(float v);

        float getTop() const;

        void setTop(float v);

        float getRight() const;

        void setRight(float v);

        float getBottom() const;

        void setBottom(float v);

        float getWidth() const;

        void setWidth(float v);

        float getHeight() const;

        void setHeight(float v);

        float getMinWidth() const;

        void setMinWidth(float v);

        float getMinHeight() const;

        void setMinHeight(float v);

        float getMaxWidth() const;

        void setMaxWidth(float v);

        float getMaxHeight() const;

        void setMaxHeight(float v);

        float getScalingX() const;

        void setScalingX(float v);

        float getScalingY() const;

        void setScalingY(float v);

        bool isVisible() const;

        void setVisible(bool v);

        void invalidateGeometry();

        void invalidateLayout();

        void render();

    protected:
        virtual void onGeometry() {}

        virtual void onLayout() const {}

        virtual void onPaint() const {}

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

        void updateGeometry();
        void updateLayout();
        void invalidateScreenGeometry();
        void updateScreenGeometry(bool force);
        void paint() const;
    };


} //namespace algui


#endif //ALGUI_WIDGET_HPP
