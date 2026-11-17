#include "Widget.hpp"
#include "utility.hpp"


namespace algui {


    const WidgetWeakContainer& Layout::widgets() const {
        return m_widgets;
    }
        

    void Layout::addWidget(const WidgetPtr& widget) {
        WidgetWeakPtr weakPtr(widget);
        auto it = findWeakPtr(m_widgets.begin(), m_widgets.end(), widget);
        if (it == m_widgets.end()) {
            m_widgets.push_back(weakPtr);
        }
    }
        

    void Layout::removeWidget(const WidgetPtr& widget) {
        WidgetWeakPtr weakPtr(widget);
        auto it = findWeakPtr(m_widgets.begin(), m_widgets.end(), widget);
        if (it != m_widgets.end()) {
            m_widgets.erase(it);
        }
    }


    void Layout::invalidatePreferredSize() {
        for (const auto& weakWgt : m_widgets) {
            auto wgt = weakWgt.lock();
            if (wgt) {
                wgt->invalidatePreferredSize();
            }
        }
    }
        
        
    void Layout::invalidateLayout() {
        for (const auto& weakWgt : m_widgets) {
            auto wgt = weakWgt.lock();
            if (wgt) {
                wgt->invalidateLayout();
            }
        }
    }


    void Layout::invalidate() {
        invalidatePreferredSize();
        invalidateLayout();
    }


} //namespace algui
