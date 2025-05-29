#include <algorithm>
#include "algui/HBoxLayout.hpp"
#include "algui/UILayoutNode.hpp"


namespace algui {


    void HBoxLayout::calcPreferredSize(UILayoutNode* parent) const {
        float minWidth = 0.0f;
        float minHeight = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        float maxWidth = 0.0f;
        float maxHeight = 0.0f;

        size_t childCount = 0;

        for (UILayoutNode* child = parent->getFirstChild(); child; child = child->getNextSibling()) {
            if (child->getPositionType() == PositionType::Managed && child->isVisible()) {
                //min sizes
                minWidth += child->getMinPreferredWidth();
                minHeight = std::max(minHeight, child->getMinPreferredHeight());

                //sizes
                width += child->getPreferredWidth();
                height = std::max(height, child->getPreferredHeight());

                //max sizes
                maxWidth += child->getMaxPreferredWidth();
                maxHeight = std::max(maxHeight, child->getMaxPreferredHeight());

                ++childCount;
            }
        }

        //add spacing
        if (childCount > 0) {
            const float spacingValue = calcPixelValue(getSpacing(), parent->UINode::getWidth(), parent->UINode::getScalingX());
            const float totalSpacing = spacingValue * (childCount - 1);
            minWidth += spacingValue;
            width += spacingValue;
            maxWidth += spacingValue;
        }

        //add margins
        const float leftMarginValue = calcPixelValue(getLeftMargin(), parent->UINode::getWidth(), parent->UINode::getScalingX());;
        const float topMarginValue = calcPixelValue(getTopMargin(), parent->UINode::getHeight(), parent->UINode::getScalingY());
        const float rightMarginValue = calcPixelValue(getLeftMargin(), parent->UINode::getWidth(), parent->UINode::getScalingX());;
        const float bottomMarginValue = calcPixelValue(getBottomMargin(), parent->UINode::getHeight(), parent->UINode::getScalingY());

        const float totalHorizontalMargins = leftMarginValue + rightMarginValue;
        const float totalVerticalMargins = topMarginValue + bottomMarginValue;

        minWidth += totalHorizontalMargins;
        minHeight += totalVerticalMargins;

        width += totalHorizontalMargins;
        height += totalVerticalMargins;

        maxWidth += totalHorizontalMargins;
        maxHeight += totalVerticalMargins;

        //set preferred sizes
        parent->setPreferredSize(minWidth, minHeight, width, height, maxWidth, maxHeight);
    }


    void HBoxLayout::doLayout(const UILayoutNode* parent) const {
        float width = 0.0f;
        float x = 0.0f;
        const float y = calcPixelValue(getTopMargin(), parent->UINode::getHeight(), parent->UINode::getScalingY());
        float height = 0.0f;

        size_t childCount = 0;
        for (UILayoutNode* child = parent->getFirstChild(); child; child = child->getNextSibling()) {
            if (child->getPositionType() == PositionType::Managed && child->isVisible()) {
                //child->UINode::setGeometry(x, );
                width += child->getPreferredWidth();
                height = std::max(height, child->getPreferredHeight());
                ++childCount;
            }
        }

        if (childCount > 0) {
            width += calcPixelValue(getSpacing(), parent->UINode::getWidth(), parent->UINode::getScalingX()) * (childCount - 1);
        }

        height += y + calcPixelValue(getBottomMargin(), parent->UINode::getHeight(), parent->UINode::getScalingY());

        if (width < parent->UINode::getWidth()) {
        }
        else if (width > parent->UINode::getWidth()) {
        }
    }


} //namespace algui
