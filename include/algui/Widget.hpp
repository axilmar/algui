#ifndef ALGUI_WIDGET_HPP
#define ALGUI_WIDGET_HPP

#include "Tree.hpp"

namespace algui {

    struct WidgetInternals;

    class Widget : public Tree<Widget> {
    public:
        Widget();
        float getLeft() const;
        float getTop() const;
        float getRight() const;
        float getBottom() const;
        float getWidth() const;
        float getHeight() const;
        float getScalingX() const;
        float getScalingY() const;
        float getScreenLeft() const;
        float getScreenTop() const;
        float getScreenRight() const;
        float getScreenBottom() const;
        bool isVisible() const;
        void setLeft(float v);
        void setTop(float v);
        void setRight(float v);
        void setBottom(float v);
        void setWidth(float v);
        void setHeight(float v);
        void setleftTop(float left, float top);
        void setRightBottom(float right, float bottom);
        void setSize(float width, float height);
        void setRect(float left, float top, float right, float bottom);
        void setGeom(float left, float top, float width, float height);
        void setScalingX(float v);
        void setScalingY(float v);
        void setVisible(bool v);
        void render();
    protected:
        virtual void onPaint() const = 0;
    private:
        float m_left;
        float m_top;
        float m_right;
        float m_bottom;
        float m_scalingX;
        float m_scalingY;
        float m_screenLeft;
        float m_screenTop;
        float m_screenRight;
        float m_screenBottom;
        float m_screenScalingX;
        float m_screenScalingY;        
        bool m_visible : 1;
        bool m_screenCoordsOk : 1;
        friend struct WidgetInternals;
    };

} //namespace algui

#endif //ALGUI_WIDGET_HPP
