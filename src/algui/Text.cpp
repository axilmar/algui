#include "algui/Text.hpp"


namespace algui {


    Text::Style::Style()
        : fontSize(12)
        , fontFlags(0)
        , color(al_map_rgb(0, 0, 0))
    {
    }


    Text::Style::Style(const ALLEGRO_COLOR& color)
        : fontSize(12)
        , fontFlags(0)
        , color(color)
    {
    }


    Text::Text()
        : m_horizontalAlignment(Alignment::Start)
        , m_verticalAlignment(Alignment::Start)
        , m_disabledStyle(al_map_rgb(192, 192, 192))
        , m_disabledErrorStyle(al_map_rgb(128, 80, 80))
        , m_selectedStyle(al_map_rgb(255, 255, 255))
        , m_errorStyle(al_map_rgb(255, 0, 0))
        , m_selectedHighlightedStyle(al_map_rgb(255, 255, 255))
        , m_selectedPressedStyle(al_map_rgb(255, 255, 255))
    {
    }


    const UString& Text::getText() const {
        return m_text;
    }


    const Text::Style& Text::getDisabledStyle() const {
        return m_disabledStyle;
    }


    const Text::Style& Text::getDisabledErrorStyle() const {
        return m_disabledErrorStyle;
    }


    const Text::Style& Text::getEnabledStyle() const {
        return m_enabledStyle;
    }


    const Text::Style& Text::getHighlightedStyle() const {
        return m_highlightedStyle;
    }


    const Text::Style& Text::getPressedStyle() const {
        return m_pressedStyle;
    }


    const Text::Style& Text::getSelectedStyle() const {
        return m_selectedStyle;
    }


    const Text::Style& Text::getFocusedStyle() const {
        return m_focusedStyle;
    }


    const Text::Style& Text::getErrorStyle() const {
        return m_errorStyle;
    }


    const Text::Style& Text::getSelectedHighlightedStyle() const {
        return m_selectedHighlightedStyle;
    }


    const Text::Style& Text::getSelectedPressedStyle() const {
        return m_selectedPressedStyle;
    }


    void Text::setText(const UString& str) {
        if (str == m_text) {
            return;
        }
        m_text = str;
        invalidateVisualState();
    }


    void Text::setDisabledStyle(const Style& style) {
        m_disabledStyle = style;
        invalidateVisualState();
    }


    void Text::setDisabledErrorStyle(const Style& style) {
        m_disabledErrorStyle = style;
        invalidateVisualState();
    }


    void Text::setEnabledStyle(const Style& style) {
        m_enabledStyle = style;
        invalidateVisualState();
    }


    void Text::setHighlightedStyle(const Style& style) {
        m_highlightedStyle = style;
        invalidateVisualState();
    }


    void Text::setPressedStyle(const Style& style) {
        m_pressedStyle = style;
        invalidateVisualState();
    }


    void Text::setSelectedStyle(const Style& style) {
        m_selectedStyle = style;
        invalidateVisualState();
    }


    void Text::setFocusedStyle(const Style& style) {
        m_focusedStyle = style;
        invalidateVisualState();
    }


    void Text::setErrorStyle(const Style& style) {
        m_errorStyle = style;
        invalidateVisualState();
    }


    void Text::setSelectedHighlightedStyle(const Style& style) {
        m_selectedHighlightedStyle = style;
        invalidateVisualState();
    }


    void Text::setSelectedPressedStyle(const Style& style) {
        m_selectedPressedStyle = style;
        invalidateVisualState();
    }


    void Text::visualState() {
        if (!getFlexible()) {
            const Style& activeStyle = _getActiveStyle();
            if (activeStyle.font && m_text) {
                int x, y, w, h;
                al_get_ustr_dimensions(activeStyle.font.get(), m_text, &x, &y, &w, &h);
                setSize(w, h);
            }
        }
    }


    void Text::paint() const {
        const Style& activeStyle = _getActiveStyle();
        if (activeStyle.font && m_text) {
            int x, y, w, h;
            al_get_ustr_dimensions(activeStyle.font.get(), m_text, &x, &y, &w, &h);
            float tx, ty;
            switch (m_horizontalAlignment) {
                case Alignment::Start:
                    tx = getX1();
                    break;
                case Alignment::Middle:
                    tx = getX1() + ((this->getX2() - this->getX1()) - w) / 2;
                    break;
                case Alignment::End:
                    tx = getX2() - w;
                    break;
                }
            switch (m_verticalAlignment) {
                case Alignment::Start:
                    ty = getY1();
                    break;
                case Alignment::Middle:
                    ty = getY1() + ((this->getY2() - this->getY1()) - h) / 2;
                    break;
                case Alignment::End:
                    ty = getY2() - h;
                    break;
            }
            if (activeStyle.embossedOffsetX != 0 || activeStyle.embossedOffsetY != 0) {
                al_draw_ustr(activeStyle.font.get(), activeStyle.embossedColor, tx + activeStyle.embossedOffsetX, ty + activeStyle.embossedOffsetY, 0, m_text);
            }
            al_draw_ustr(activeStyle.font.get(), activeStyle.color, tx, ty, 0, m_text);
        }
    }


    void Text::themed() {
        const std::shared_ptr<Theme>& theme = getTheme();
        _readStyle(theme, "Disabled", m_disabledStyle);
        _readStyle(theme, "DisabledError", m_disabledErrorStyle);
        _readStyle(theme, "Enabled", m_enabledStyle);
        _readStyle(theme, "Highlighted", m_highlightedStyle);
        _readStyle(theme, "Pressed", m_pressedStyle);
        _readStyle(theme, "Selected", m_selectedStyle);
        _readStyle(theme, "Focused", m_focusedStyle);
        _readStyle(theme, "Error", m_errorStyle);
        _readStyle(theme, "SelectedHighlighted", m_selectedHighlightedStyle);
        _readStyle(theme, "SelectedPressed", m_selectedPressedStyle);
        invalidateVisualState();
    }


    void Text::scaled() {
        const std::shared_ptr<Theme>& theme = getTheme();
        if (theme) {
            _readFont(theme, "Disabled", m_disabledStyle);
            _readFont(theme, "DisabledError", m_disabledErrorStyle);
            _readFont(theme, "Enabled", m_enabledStyle);
            _readFont(theme, "Highlighted", m_highlightedStyle);
            _readFont(theme, "Pressed", m_pressedStyle);
            _readFont(theme, "Selected", m_selectedStyle);
            _readFont(theme, "Focused", m_focusedStyle);
            _readFont(theme, "Error", m_errorStyle);
            _readFont(theme, "SelectedHighlighted", m_selectedHighlightedStyle);
            _readFont(theme, "SelectedPressed", m_selectedPressedStyle);
        }
    }


    const Text::Style& Text::_getActiveStyle() const {
        switch (getTreeVisualState()) {
            case VisualState::Disabled:
                return m_disabledStyle;

            case VisualState::Error:
                return m_errorStyle;

            case VisualState::DisabledError:
                return m_disabledErrorStyle;

            case VisualState::Focused:
                return m_focusedStyle;

            case VisualState::Selected:
                return m_selectedStyle;

            case VisualState::Highlighted:
                return m_highlightedStyle;

            case VisualState::Pressed:
                return m_pressedStyle;

            case VisualState::SelectedHighlighted:
                return m_selectedHighlightedStyle;

            case VisualState::SelectedPressed:
                return m_selectedPressedStyle;
        }

        return m_enabledStyle;
    }


    void Text::_readFont(const std::shared_ptr<Theme>& theme, const std::string& name, Style& style) {
        style.font = theme->getFont("Text", getId(), {}, { name + ".Font" }, style.fontSize * getTreeVerticalScaling(), style.fontFlags);
    }


    void Text::_readStyle(const std::shared_ptr<Theme>& theme, const std::string& name, Style& style) {
        style.fontSize = theme->getInt("Text", getId(), {}, { name + ".Font.Size" }, 12);
        style.fontFlags = theme->getInt("Text", getId(), {}, { name + ".Font.Flags" }, 0);
        style.font = theme->getFont("Text", getId(), {}, { name + ".Font" }, style.fontSize, style.fontFlags);
        style.color = theme->getColor("Text", getId(), {}, { name + ".Color" }, style.color);
        style.embossedOffsetX = theme->getFloat("Text", getId(), {}, { name + ".Embossed.OffsetX" }, 0.0f);
        style.embossedOffsetY = theme->getFloat("Text", getId(), {}, { name + ".Embossed.OffsetY" }, 0.0f);
        style.embossedColor = theme->getColor("Text", getId(), {}, { name + ".Embossed.Color" }, al_map_rgb(255, 255, 255));
    }


} //namespace algui

