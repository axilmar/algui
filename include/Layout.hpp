#ifndef ALGUI_LAYOUT_HPP
#define ALGUI_LAYOUT_HPP


#include <memory>
#include <vector>


namespace algui {


    class Widget;


    using WidgetPtr = std::shared_ptr<Widget>;
    using WidgetWeakPtr = std::weak_ptr<Widget>;
    using WidgetWeakContainer = std::vector<WidgetWeakPtr>;


    class Layout {
    public:
        virtual ~Layout() {}

        const WidgetWeakContainer& widgets() const;
        void addWidget(const WidgetPtr& widget);
        void removeWidget(const WidgetPtr& widget);

        void invalidatePreferredSize();
        void invalidateLayout();
        void invalidate();

        virtual void apply(const WidgetPtr& widget) const = 0;

    private:
        WidgetWeakContainer m_widgets;
    };


    using LayoutPtr = std::shared_ptr<Layout>;


} //namespace algui


#endif //ALGUI_LAYOUT_HPP
