#include <limits>
#include "algui/Widget.hpp"

namespace algui {

    struct WidgetInternals {
        static void calcRootScreenCoords(Widget* wgt) {
            wgt->m_screenLeft = wgt->m_left;
            wgt->m_screenTop = wgt->m_top;
            wgt->m_screenRight = wgt->m_right;
            wgt->m_screenBottom = wgt->m_bottom;
            wgt->m_screenScalingX = wgt->m_scalingX;
            wgt->m_screenScalingY = wgt->m_scalingY;
            wgt->m_screenCoordsOk = true;
        }

        static void calcChildScreenCoords(Widget* wgt, Widget* parent) {
            wgt->m_screenLeft = parent->m_screenLeft + wgt->m_left * parent->m_screenScalingX;
            wgt->m_screenTop = parent->m_screenTop + wgt->m_top * parent->m_screenScalingY;
            wgt->m_screenRight = parent->m_screenLeft + wgt->m_right * parent->m_screenScalingX;
            wgt->m_screenBottom = parent->m_screenTop + wgt->m_bottom * parent->m_screenScalingY;
            wgt->m_screenScalingX = wgt->m_scalingX * parent->m_screenScalingX;
            wgt->m_screenScalingY = wgt->m_scalingY * parent->m_screenScalingY;
            wgt->m_screenCoordsOk = true;
        }

        static void paint(const Widget* wgt) {
            wgt->onPaint();
        }

        static void paintRoot(Widget* wgt) {
            if (wgt->m_visible) {
                if (wgt->m_screenCoordsOk) {
                    paint(wgt);
                    wgt->forEach([&](Widget* child) {
                        paintChild(child);
                    });
                }
                else {
                    calcRootScreenCoords(wgt);
                    paint(wgt);
                    wgt->forEach([&](Widget* child) {
                        calcScreenCoordsAndPaintChild(child, wgt);
                    });
                }
            }
        }

        static void paintChild(Widget* wgt) {
            if (wgt->m_visible) {
                if (wgt->m_screenCoordsOk) {
                    paint(wgt);
                    wgt->forEach([&](Widget* child) {
                        paintChild(child);
                    });
                }
                else {
                    calcChildScreenCoords(wgt, wgt->getParent());
                    paint(wgt);
                    wgt->forEach([&](Widget* child) {
                        calcScreenCoordsAndPaintChild(child, wgt);
                    });
                }
            }
        }

        static void calcScreenCoordsAndPaintChild(Widget* wgt, Widget* parent) {
            if (wgt->m_visible) {
                calcChildScreenCoords(wgt, parent);
                paint(wgt);
                wgt->forEach([&](Widget* child) {
                    calcScreenCoordsAndPaintChild(child, wgt);
                });
            }
        }
    };

    Widget::Widget()
        : m_left(0)
        , m_top(0)
        , m_right(0)
        , m_bottom(0)
        , m_scalingX(1)
        , m_scalingY(1)
        , m_screenLeft(0)
        , m_screenTop(0)
        , m_screenRight(0)
        , m_screenBottom(0)
        , m_screenScalingX(1)
        , m_screenScalingY(1)
        , m_visible(true)
        , m_screenCoordsOk(true)
    {
    }

    float Widget::getLeft() const {
        return m_left;
    }

    float Widget::getTop() const {
        return m_top;
    }

    float Widget::getRight() const {
        return m_right;
    }

    float Widget::getBottom() const {
        return m_bottom;
    }

    float Widget::getWidth() const {
        return m_right - m_left;
    }

    float Widget::getHeight() const {
        return m_bottom - m_top;
    }

    float Widget::getScalingX() const {
        return m_scalingX;
    }

    float Widget::getScalingY() const {
        return m_scalingY;
    }

    float Widget::getScreenLeft() const {
        return m_screenLeft;
    }

    float Widget::getScreenTop() const {
        return m_screenTop;
    }

    float Widget::getScreenRight() const {
        return m_screenRight;
    }

    float Widget::getScreenBottom() const {
        return m_screenBottom;
    }

    bool Widget::isVisible() const {
        return m_visible;
    }

    void Widget::setLeft(float v) {
        if (v != m_left) {
            const float w = getWidth();
            m_left = v;
            m_right = v + w;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setTop(float v) {
        if (v != m_top) {
            const float h = getHeight();
            m_top = v;
            m_bottom = v + h;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setRight(float v) {
        if (v != m_right) {
            const float w = getWidth();
            m_right = v;
            m_left = v - w;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setBottom(float v) {
        if (v != m_bottom) {
            const float h = getHeight();
            m_bottom = v;
            m_top = v - h;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setWidth(float v) {
        const float newRight = m_left + (v >= 0 ? v : 0);
        if (newRight != m_right) {
            m_right = newRight;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setHeight(float v) {
        const float newBottom = m_top + (v >= 0 ? v : 0);
        if (newBottom != m_bottom) {
            m_bottom = newBottom;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setleftTop(float left, float top) {
        if (left != m_left || top != m_top) {
            float w = getWidth();
            float h = getHeight();
            m_left = left;
            m_top = top;
            m_right = left + w;
            m_bottom = top + h;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setRightBottom(float right, float bottom) {
        if (right != m_right || bottom != m_bottom) {
            float w = getWidth();
            float h = getHeight();
            m_right = right;
            m_bottom = bottom;
            m_left = right - w;
            m_top = bottom - h;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setSize(float width, float height) {
        const float newRight = m_left + (width >= 0 ? width : 0);
        const float newBottom = m_top + (height >= 0 ? height : 0);
        if (newRight != m_right || newBottom != m_bottom) {
            m_right = newRight;
            m_bottom = newBottom;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setRect(float left, float top, float right, float bottom) {
        if (right < left) {
            right = left;
        }
        if (bottom < top) {
            bottom = top;
        }
        if (left != m_left || top != m_top || right != m_right || bottom != m_bottom) {
            m_left = left;
            m_top = top;
            m_right = right;
            m_bottom = bottom;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setGeom(float left, float top, float width, float height) {
        const float newRight = m_left + (width >= 0 ? width : 0);
        const float newBottom = m_top + (height >= 0 ? height : 0);
        if (left != m_left || top != m_top || newRight != m_right || newBottom != m_bottom) {
            m_left = left;
            m_top = top;
            m_right = newRight;
            m_bottom = newBottom;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setScalingX(float v) {
        if (v != m_scalingX) {
            m_scalingX = v;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setScalingY(float v) {
        if (v != m_scalingY) {
            m_scalingY = v;
            m_screenCoordsOk = false;
        }
    }

    void Widget::setVisible(bool v) {
        if (v != m_visible) {
            m_visible = v;
        }
    }

    void Widget::render() {
        WidgetInternals::paintRoot(this);
    }

} //namespace algui
