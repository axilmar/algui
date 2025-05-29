#ifndef ALGUI_LAYOUT_HPP
#define ALGUI_LAYOUT_HPP


#include "SharedResource.hpp"


namespace algui {


    class UILayoutNode;


    class Layout : public SharedResource<Layout, UILayoutNode> {
    public:
        virtual void calcPreferredSize(UILayoutNode* parent) const = 0;

        virtual void doLayout(const UILayoutNode* parent) const = 0;
    };


} //namespace algui


#endif //ALGUI_LAYOUT_HPP
