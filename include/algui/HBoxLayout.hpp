#ifndef ALGUI_HBOXLAYOUT_HPP
#define ALGUI_HBOXLAYOUT_HPP


#include "BoxLayout.hpp"


namespace algui {


    class HBoxLayout : public BoxLayout {
    public:
        void calcPreferredSize(UILayoutNode* parent) const override;

        void doLayout(const UILayoutNode* parent) const override;
    };


} //namespace algui


#endif //ALGUI_HBOXLAYOUT_HPP
