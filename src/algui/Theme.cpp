#include <stdexcept>
#include <functional>
#include <sstream>
#pragma warning (disable: 4309)
#include "allegro5/allegro_color.h"
#include "algui/Theme.hpp"
#include "algui/FontCache.hpp"


namespace algui {


    using ActionFunction = std::function<void(const char* key, const char* value)>;


    static void processConfig(ALLEGRO_CONFIG* config, const ActionFunction& action) {
        ALLEGRO_CONFIG_SECTION* section = nullptr;
        const char* sectionName = al_get_first_config_section(config, &section);
        while (sectionName) {
            ALLEGRO_CONFIG_ENTRY* entry = nullptr;
            const char* key = al_get_first_config_entry(config, sectionName, &entry);
            while (key) {
                const char* value = al_get_config_value(config, sectionName, key);
                if (value) {
                    action(key, value);
                }
                key = al_get_next_config_entry(&entry);
            }
            sectionName = al_get_next_config_section(&section);
        }
    }


    //checkout if the given value represents a font
    static bool isFont(const char* value) {
        ALLEGRO_FONT* font = al_load_font(value, 16, 0);
        if (font) {
            al_destroy_font(font);
            return true;
        }
        return false;
    }


    static bool isHexDigit(char c) {
        return c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
    }


    static int hexValue(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f') {
            return 10 + c - 'a';
        }
        return 10 + c - 'A';
    }


    static int readHex2(const char* value) {
        return hexValue(value[0]) * 16 + hexValue(value[1]);
    }


    //al_color_html_to_rgb does not return a boolean,
    //and therefore it cannot be used for parsing
    static bool color_html_to_rgb(const char* value, float* r, float* g, float* b) {
        if (!value) {
            return false;
        }

        const int len = strlen(value);
        if (len < 7) {
            return false;
        }

        if (value[0] != '#') {
            return false;
        }

        for (size_t i = 1; i < 7; ++i) {
            if (!isHexDigit(value[i])) {
                return false;
            }
        }

        const int rv = readHex2(value + 1);
        const int gv = readHex2(value + 3);
        const int bv = readHex2(value + 5);

        *r = rv / 255.0f;
        *g = gv / 255.0f;
        *b = bv / 255.0f;

        return true;
    }


    //loads a color; either the name is recognized, or html color.
    static ALLEGRO_COLOR loadColor(const char* value, bool& ok) {
        //try color name
        float r, g, b;
        ok = al_color_name_to_rgb(value, &r, &g, &b);
        if (ok) {
            return al_map_rgb_f(r, g, b);
        }

        //try html color
        ok = color_html_to_rgb(value, &r, &g, &b);
        if (ok) {
            return al_map_rgb_f(r, g, b);
        }

        return {};
    }


    static bool loadBoolean(const char* value, bool& ok) {
        ok = false;

        //try true
        if (strcmp(value, "true") == 0 || strcmp(value, "True") == 0 || strcmp(value, "TRUE") == 0) {
            ok = true;
            return true;
        }

        //try false
        if (strcmp(value, "false") == 0 || strcmp(value, "False") == 0 || strcmp(value, "FALSE") == 0) {
            ok = true;
            return false;
        }

        return false;
    }


    static float loadFloat(const char* value, bool& ok) {
        ok = false;

        if (!value) {
            return false;
        }

        bool dotFound = false;
        int len = 0;
        for (const char* t = value; *t; ++t, ++len) {
            if (*t == '.') {
                dotFound = true;
            }
            else if (*t < '0' || *t > '9') {
                return false;
            }
        }
        const bool suffixFound = value[len - 1] == 'f' || value[len - 1] == 'F';

        if (!dotFound && !suffixFound) {
            return false;
        }

        std::stringstream stream;
        stream << value;
        float f;
        stream >> f;

        if (!stream.fail()) {
            ok = true;
            return f;
        }

        return 0;
    }


    static int loadInt(const char* value, bool& ok) {
        ok = false;

        std::stringstream stream;
        stream << value;
        int i;
        stream >> i;

        if (!stream.fail()) {
            ok = true;
            return i;
        }

        return 0;
    }


    Theme::Theme() {
    }


    Theme::Theme(const char* filename) {
        //load config
        ALLEGRO_CONFIG* config = al_load_config_file(filename);
        if (!config) {
            throw std::invalid_argument(filename);
        }

        //prepare the action callback
        ActionFunction action = [&](const char* key, const char* value) {
            ALLEGRO_BITMAP* bitmap = al_load_bitmap(value);
            if (bitmap) {
                m_bitmaps[key] = bitmap;
                return;
            }

            if (isFont(value)) {
                m_fontPaths[key] = value;
                return;
            }

            ALLEGRO_SAMPLE* sample = al_load_sample(value);
            if (sample) {
                m_samples[key] = sample;
                return;
            }

            bool ok;
            ALLEGRO_COLOR color = loadColor(value, ok);
            if (ok) {
                m_colors[key] = color;
                return;
            }

            const bool boolean = loadBoolean(value, ok);
            if (ok) {
                m_bools[key] = boolean;
                return;
            }

            const float floatValue = loadFloat(value, ok);
            if (ok) {
                m_floats[key] = floatValue;
                return;
            }

            const int intValue = loadInt(value, ok);
            if (ok) {
                m_ints[key] = intValue;
                return;
            }

            m_strings[key] = value;
        };

        //process the config
        processConfig(config, action);

        //destroy the config
        al_destroy_config(config);
    }


    Theme::~Theme() {
        for (const auto& p : m_bitmaps) {
            al_destroy_bitmap(p.second);
        }
        for (const auto& p : m_fonts) {
            al_destroy_font(p.second);
        }
        for (const auto& p : m_samples) {
            al_destroy_sample(p.second);
        }
    }


    ALLEGRO_BITMAP* Theme::getBitmap(const char* name, ALLEGRO_BITMAP* defaultValue) const {
        const auto it = m_bitmaps.find(name);
        return it != m_bitmaps.end() ? it->second : defaultValue;
    }


    void Theme::setBitmap(const char* name, ALLEGRO_BITMAP* bitmap) {
        if (bitmap) {
            const auto it = m_bitmaps.find(name);
            if (it != m_bitmaps.end()) {
                if (it->second != bitmap) {
                    al_destroy_bitmap(it->second);
                    it->second = bitmap;
                }
            }
            else {
                m_bitmaps.insert(std::make_pair(name, bitmap));
            }
        }
        else {
            const auto it = m_bitmaps.find(name);
            if (it != m_bitmaps.end()) {
                al_destroy_bitmap(it->second);
                m_bitmaps.erase(it);
            }
        }
    }


    ALLEGRO_COLOR Theme::getColor(const char* name, const ALLEGRO_COLOR& defaultValue) const {
        const auto it = m_colors.find(name);
        return it != m_colors.end() ? it->second : defaultValue;
    }


    void Theme::setColor(const char* name, ALLEGRO_COLOR color) {
        m_colors[name] = color;
    }


    ALLEGRO_FONT* Theme::getFont(const char* name, int size, int flags, ALLEGRO_FONT* defaultValue) const {
        //find internal font
        const auto key = std::make_tuple(name, size, flags);
        const auto it = m_fonts.find(key);
        if (it != m_fonts.end()) {
            return it->second;
        }

        //get font filename
        const auto itFilename = m_fontPaths.find(name);
        if (itFilename == m_fontPaths.end()) {
            return nullptr;
        }

        //find font via FontCache
        return FontCache::getFont(itFilename->second.c_str(), size, flags);
    }


    void Theme::setFont(const char* name, ALLEGRO_FONT* font, int size, int flags) {
        const auto key = std::make_tuple(name, size, flags);
        if (font) {
            const auto it = m_fonts.find(key);
            if (it != m_fonts.end()) {
                if (it->second != font) {
                    al_destroy_font(it->second);
                    it->second = font;
                }
            }
            else {
                m_fonts.insert(std::make_pair(key, font));
            }
        }
        else {
            const auto it = m_fonts.find(key);
            if (it != m_fonts.end()) {
                al_destroy_font(it->second);
                m_fonts.erase(it);
            }
        }
    }


    int Theme::getInt(const char* name, int defaultValue) const {
        const auto it = m_ints.find(name);
        return it != m_ints.end() ? it->second : defaultValue;
    }


    void Theme::setInt(const char* name, int value) {
        m_ints[name] = value;
    }


    float Theme::getFloat(const char* name, float defaultValue) const {
        const auto it = m_floats.find(name);
        return it != m_floats.end() ? it->second : defaultValue;
    }


    void Theme::setFloat(const char* name, float value) {
        m_floats[name] = value;
    }


    std::any Theme::getAny(const char* name, const std::any& defaultValue) const {
        const auto it = m_any.find(name);
        return it != m_any.end() ? it->second : defaultValue;
    }


    void Theme::setAny(const char* name, const std::any& any) {
        m_any[name] = any;
    }


    ALLEGRO_SAMPLE* Theme::getSample(const char* name, ALLEGRO_SAMPLE* defaultValue) const {
        const auto it = m_samples.find(name);
        return it != m_samples.end() ? it->second : defaultValue;
    }


    void Theme::setSample(const char* name, ALLEGRO_SAMPLE* sample) {
        if (sample) {
            const auto it = m_samples.find(name);
            if (it != m_samples.end()) {
                if (it->second != sample) {
                    al_destroy_sample(it->second);
                    it->second = sample;
                }
            }
            else {
                m_samples.insert(std::make_pair(name, sample));
            }
        }
        else {
            const auto it = m_samples.find(name);
            if (it != m_samples.end()) {
                al_destroy_sample(it->second);
                m_samples.erase(it);
            }
        }
    }


    bool Theme::getBool(const char* name, bool defaultValue) const {
        const auto it = m_bools.find(name);
        return it != m_bools.end() ? it->second : defaultValue;
    }
    

    void Theme::setBool(const char* name, bool value) {
        m_bools[name] = value;
    }


    std::string Theme::getString(const char* name, const std::string& defaultValue) const {
        const auto it = m_strings.find(name);
        return it != m_strings.end() ? it->second : defaultValue;
    }
    

    void Theme::setString(const char* name, const std::string& value) {
        m_strings[name] = value;
    }


} //namespace algui
