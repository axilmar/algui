#ifndef ALGUI_BOXLAYOUT_HPP
#define ALGUI_BOXLAYOUT_HPP


#include "Layout.hpp"
#include "AlignmentType.hpp"
#include "CoordType.hpp"


namespace algui {


    class BoxLayout : public Layout {
    public:
        const SizeValue& getLeftMargin() const {
            return m_leftMargin;
        }

        void setLeftMargin(const SizeValue& v) {
            if (v != m_leftMargin) {
                m_leftMargin = v;
                setChanged();
            }
        }

        const SizeValue& getTopMargin() const {
            return m_topMargin;
        }

        void setTopMargin(const SizeValue& v) {
            if (v != m_topMargin) {
                m_topMargin = v;
                setChanged();
            }
        }

        const SizeValue& getRightMargin() const {
            return m_rightMargin;
        }

        void setRightMargin(const SizeValue& v) {
            if (v != m_rightMargin) {
                m_rightMargin = v;
                setChanged();
            }
        }

        const SizeValue& getBottomMargin() const {
            return m_bottomMargin;
        }

        void setBottomMargin(const SizeValue& v) {
            if (v != m_bottomMargin) {
                m_bottomMargin = v;
                setChanged();
            }
        }

        const SizeValue& getSpacing() const {
            return m_spacing;
        }

        void setSpacing(const SizeValue& v) {
            if (v != m_spacing) {
                m_spacing = v;
                setChanged();
            }
        }

        AlignmentType getHorizontalAlignment() const {
            return m_horizontalAlignment;
        }

        void setHorizontalAlignment(AlignmentType v) {
            if (v != m_horizontalAlignment) {
                m_horizontalAlignment = v;
                setChanged();
            }
        }

        AlignmentType getVerticalAlignment() const {
            return m_verticalAlignment;
        }

        void setVerticalAlignment(AlignmentType v) {
            if (v != m_verticalAlignment) {
                m_verticalAlignment = v;
                setChanged();
            }
        }

    private:
        SizeValue m_leftMargin{ 0.0f };
        SizeValue m_topMargin{ 0.0f };
        SizeValue m_rightMargin{ 0.0f };
        SizeValue m_bottomMargin{ 0.0f };
        SizeValue m_spacing{ 0.0f };
        AlignmentType m_horizontalAlignment{ AlignmentType::Center };
        AlignmentType m_verticalAlignment{ AlignmentType::Center };
    };


} //namespace algui


#endif //ALGUI_BOXLAYOUT_HPP
