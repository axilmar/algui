#include <sstream>
#include <cctype>
#include <filesystem>
#include <allegro5/allegro_color.h>
#include "algui/Theme.hpp"
#include "algui/ResourceCache.hpp"


namespace algui {


    static int getHexDigitValue(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        return -1;
    }


    static bool getHexColorComponent(const char* value, float& v) {
        const int hexDigit1 = getHexDigitValue(value[0]);
        if (hexDigit1 >= 0) {
            const int hexDigit2 = getHexDigitValue(value[1]);
            if (hexDigit2 >= 0) {
                v = (float)(hexDigit1 * 16 + hexDigit2) / 255.0f;
                return true;
            }
        }
        return false;
    }


    static bool htmlColorToRGBA(const char* value, float& r, float& g, float& b, float& a) {
        if (!value) {
            return false;
        }
        if (value[0] != '#') {
            return false;
        }
        ++value;
        if (!getHexColorComponent(value, r)) {
            return false;
        }
        value += 2;
        if (!getHexColorComponent(value, g)) {
            return false;
        }
        value += 2;
        if (!getHexColorComponent(value, b)) {
            return false;
        }
        value += 2;
        a = 1.0f;
        getHexColorComponent(value, a);
        return true;
    }


    static bool equalNoCase(const char* s1, const char* s2) {
        for (;;) {
            if (!*s1) {
                return !*s2;
            }
            if (std::tolower(*s1) != std::tolower(*s2)) {
                break;
            }
            ++s1;
            ++s2;
        }
        return false;
    }


    Theme::Theme() : m_config(nullptr) {
    }


    Theme::Theme(const std::string& filepath) : m_config(nullptr) {
        load(filepath);
    }


    Theme::~Theme() {
        al_destroy_config(m_config);
    }


    void Theme::reset() {
        al_destroy_config(m_config);
        m_config = nullptr;
    }


    bool Theme::load(const std::string& filepath) {
        ALLEGRO_CONFIG* config = al_load_config_file(filepath.c_str());
        if (config) {
            al_destroy_config(m_config);
            m_config = config;
            m_path = std::filesystem::path(filepath).parent_path().string();
            m_filename = std::filesystem::path(filepath).filename().string();
            return true;
        }
        return false;
    }


    std::shared_ptr<ALLEGRO_BITMAP> Theme::getBitmap(const std::string& section, const std::string& key, const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue) {
        if (m_config) {
            const char* path = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (path) {
                const std::string actualPath = m_path + '/' + path;
                ALLEGRO_BITMAP* bitmap = al_load_bitmap(actualPath.c_str());
                if (bitmap) {
                    return ResourceCache::putBitmap(path, bitmap);
                }
            }
        }
        return defaultValue;
    }


    std::shared_ptr<ALLEGRO_FONT> Theme::getFont(const std::string& section, const std::string& key, int size, int flags, const std::shared_ptr<ALLEGRO_FONT>& defaultValue) {
        if (m_config) {
            const char* path = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (path) {
                const std::string actualPath = m_path + '/' + path;
                ALLEGRO_FONT* font = al_load_font(actualPath.c_str(), size, flags);
                if (font) {
                    return ResourceCache::putFont(ResourceCache::FontKey(path, size, flags), font);
                }
            }
        }
        return defaultValue;
    }


    ALLEGRO_COLOR Theme::getColor(const std::string& section, const std::string& key, const ALLEGRO_COLOR& defaultValue) {
        if (m_config) {
            const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (value) {
                float r, g, b, a;
                if (al_color_name_to_rgb(value, &r, &g, &b) || htmlColorToRGBA(value, r, g, b, a)) {
                    return al_map_rgba_f(r, g, b, a);
                }
            }
        }
        return defaultValue;
    }


    int Theme::getInt(const std::string& section, const std::string& key, int defaultValue) {
        if (m_config) {
            const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (value) {
                std::stringstream stream;
                stream << value;
                int resultValue;
                stream >> resultValue;
                if (!stream.fail()) {
                    return resultValue;
                }
            }
        }
        return defaultValue;
    }


    float Theme::getFloat(const std::string& section, const std::string& key, float defaultValue) {
        if (m_config) {
            const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (value) {
                std::stringstream stream;
                stream << value;
                float resultValue;
                stream >> resultValue;
                if (!stream.fail()) {
                    return resultValue;
                }
            }
        }
        return defaultValue;
    }


    bool Theme::getBool(const std::string& section, const std::string& key, bool defaultValue) {
        if (m_config) {
            const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (value) {
                return equalNoCase(value, "true");
            }
        }
        return defaultValue;
    }


    std::string Theme::getString(const std::string& section, const std::string& key, const std::string& defaultValue) {
        if (m_config) {
            const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
            if (value) {
                return value;
            }
        }
        return defaultValue;
    }


} //namespace algui
