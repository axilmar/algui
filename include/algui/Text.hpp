#ifndef ALGUI_TEXT_HPP
#define ALGUI_TEXT_HPP


#include "Widget.hpp"
#include "UString.hpp"
#include "Alignment.hpp"


namespace algui {


    class Text : public Widget {
    public:
        struct Style {
            std::shared_ptr<ALLEGRO_FONT> font;
            int fontSize;
            int fontFlags;
            ALLEGRO_COLOR color;
            float embossedOffsetX;
            float embossedOffsetY;
            ALLEGRO_COLOR embossedColor;
            Style();
            Style(const ALLEGRO_COLOR& color);
        };

        Text();

        const UString& getText() const;

        const Style& getDisabledStyle() const;

        const Style& getDisabledErrorStyle() const;

        const Style& getEnabledStyle() const;

        const Style& getHighlightedStyle() const;

        const Style& getPressedStyle() const;

        const Style& getSelectedStyle() const;

        const Style& getFocusedStyle() const;

        const Style& getErrorStyle() const;

        const Style& getSelectedHighlightedStyle() const;

        const Style& getSelectedPressedStyle() const;

        void setText(const UString& str);

        void setDisabledStyle(const Style& style);

        void setDisabledErrorStyle(const Style& style);

        void setEnabledStyle(const Style& style);

        void setHighlightedStyle(const Style& style);

        void setPressedStyle(const Style& style);

        void setSelectedStyle(const Style& style);

        void setFocusedStyle(const Style& style);

        void setErrorStyle(const Style& style);

        void setSelectedHighlightedStyle(const Style& style);

        void setSelectedPressedStyle(const Style& style);

    protected:
        void visualState() override;

        void paint() const override;

        void themed() override;

        void scaled() override;

    private:
        UString m_text;
        Style m_disabledStyle;
        Style m_disabledErrorStyle;
        Style m_enabledStyle;
        Style m_highlightedStyle;
        Style m_pressedStyle;
        Style m_selectedStyle;
        Style m_focusedStyle;
        Style m_errorStyle;
        Style m_selectedHighlightedStyle;
        Style m_selectedPressedStyle;
        Alignment m_horizontalAlignment;
        Alignment m_verticalAlignment;

        const Style& _getActiveStyle() const;
        void _readFont(const std::shared_ptr<Theme>& theme, const std::string& name, Style& style);
        void _readStyle(const std::shared_ptr<Theme>& theme, const std::string& name, Style& style);
    };


} //namespace algui


#endif //ALGUI_TEXT_HPP
