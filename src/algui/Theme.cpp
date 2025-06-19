#include <filesystem>
#include <sstream>
#include <allegro5/allegro_color.h>
#include "algui/Theme.hpp"
#include "algui/ResourceCache.hpp"


#ifdef max
#undef max
#undef min
#endif


namespace algui {


    template <class V, class GetKey, class Get, class Load, class Put> 
    static std::shared_ptr<V> _loadResource(
        const ALLEGRO_CONFIG* config,
        const std::string& configPath,
        const std::string& section,
        const std::string& key,
        const GetKey& getKey,
        const Get& get,
        const Load& load,
        const Put& put)
    {
        //get path value from config file
        const char* pathValue = al_get_config_value(config, section.c_str(), key.c_str());

        //no path value
        if (!pathValue) {
            return nullptr;
        }

        try {
            //get the actual path relative to the config path
            const std::string path = std::filesystem::canonical((std::filesystem::path(configPath) / std::filesystem::path(pathValue))).string();

            //find the cached resource
            const auto resourceKey = getKey(path);
            std::shared_ptr<V> ptr = get(resourceKey);

            //if found, return it
            if (ptr) {
                return ptr;
            }

            //load the resource
            V* resource = load(path);

            //if the resource could not be loaded
            if (!resource) {
                return nullptr;
            }

            //cache the result
            std::shared_ptr<V> result = put(resourceKey, resource);

            return result;
        }
        catch (...) {
        }

        //failure
        return nullptr;
    }


    static bool _getHex(int& c) {
        if (c >= '0' && c <= '9') {
            c = c - '0';
            return true;
        }

        if (c >= 'a' && c <= 'f') {
            c = c - 'a' + 10;
            return true;
        }

        if (c >= 'A' && c <= 'F') {
            c = c - 'A' + 10;
            return true;
        }

        return false;
    }


    static bool _htmlColorToRGBA(const char* value, float& r, float& g, float& b, float& a) {
        if (!value) {
            return false;
        }

        if (*value != '#') {
            return false;
        }
        ++value;

        //parse red
        int red1 = *value;
        if (!_getHex(red1)) {
            return false;
        }
        ++value;
        int red2 = *value;
        if (!_getHex(red2)) {
            return false;
        }
        ++value;

        //parse green
        int green1 = *value;
        if (!_getHex(green1)) {
            return false;
        }
        ++value;
        int green2 = *value;
        if (!_getHex(green2)) {
            return false;
        }
        ++value;

        //parse blue
        int blue1 = *value;
        if (!_getHex(blue1)) {
            return false;
        }
        ++value;
        int blue2 = *value;
        if (!_getHex(blue2)) {
            return false;
        }
        ++value;

        //parse optional alpha
        int alpha1 = -1, alpha2;
        if (*value != '\0') {
            alpha1 = *value;
            if (!_getHex(alpha1)) {
                return false;
            }
            ++value;
            alpha2 = *value;
            if (!_getHex(alpha2)) {
                return false;
            }
        }

        //success
        r = ((red1 * 16) + red2) / 255.f;
        g = ((green1 * 16) + green2) / 255.f;
        b = ((blue1 * 16) + blue2) / 255.f;
        if (alpha1 >= 0) {
            a = ((alpha1 * 16) + alpha2) / 255.f;
        }
        return true;
    }


    static int _compareNoCase(const char* str1, const char* str2) {
        for (; *str1 && *str2; ++str1, ++str2) {
            const int d = std::tolower(*str1) - std::tolower(*str2);
            if (d) {
                return d;
            }
        }
        return std::tolower(*str1) - std::tolower(*str2);
    }


    template <class F>
    static auto _getWidgetResource(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const F& f)
    {
        std::vector<std::string> sections;

        if (!widgetId.empty()) {
            if (!surfaceType.empty()) {
                sections.push_back(surfaceType + '.' + widgetId);
            }
            sections.push_back(widgetId);
        }

        if (!widgetClassName.empty()) {
            if (!surfaceType.empty()) {
                sections.push_back(surfaceType + '.' + widgetClassName);
            }
            sections.push_back(widgetClassName);
        }

        if (!surfaceType.empty()) {
            sections.push_back(surfaceType);
        }

        return f(sections);
    }


    static const char _hexDigits[] = "0123456789abcdef";


    static void _writeColorComponent(std::stringstream& stream, float c) {
        const int v = std::max(0, std::min((int)(255.0f * c), 255));
        const int h = v / 16;
        const int l = v % 16;
        stream << _hexDigits[h];
        stream << _hexDigits[l];
    }


    static std::string _getString(const ALLEGRO_COLOR& color) {
        std::stringstream stream;
        stream << '#';
        _writeColorComponent(stream, color.r);
        _writeColorComponent(stream, color.g);
        _writeColorComponent(stream, color.b);
        _writeColorComponent(stream, color.a);
        return stream.str();
    }


    static std::string _getString(int value) {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }


    static std::string _getString(float value) {
        std::stringstream stream;
        stream << value;
        return stream.str();
    }


    static std::string _getString(bool value) {
        return value ? "true" : "false";
    }


    Theme::Theme() 
        : m_config(nullptr)
    {
    }


    Theme::Theme(const std::string& path)
        : m_config(nullptr)
    {
        load(path);
    }


    Theme::~Theme() {
        al_destroy_config(m_config);
    }


    bool Theme::isEmpty() const {
        return m_config;
    }


    bool Theme::load(const std::string& path) {
        ALLEGRO_CONFIG* config = al_load_config_file(path.c_str());
        if (!config) {
            return false;
        }
        al_destroy_config(m_config);
        m_config = config;
        m_path = path;
        return true;
    }


    void Theme::reset() {
        al_destroy_config(m_config);
        m_config = nullptr;
    }


    std::shared_ptr<ALLEGRO_BITMAP> Theme::getBitmap(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                std::shared_ptr<ALLEGRO_BITMAP> resource = _loadResource<ALLEGRO_BITMAP>(m_config, m_path, section, key, 
                    [](const std::string& path) {
                        return path;
                    },
                    [](const std::string& path) {
                        return ResourceCache::getBitmap(path);
                    },
                    [](const std::string& path) {
                        return al_load_bitmap(path.c_str());
                    },
                    [](const std::string& key, ALLEGRO_BITMAP* bmp) {
                        return ResourceCache::putBitmap(key, bmp);
                    }
                );
                if (resource) {
                    return resource;
                }
            }
        }
        return defaultValue;
    }


    std::shared_ptr<ALLEGRO_FONT> Theme::getFont(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        int size,
        int flags,
        const std::shared_ptr<ALLEGRO_FONT>& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                std::shared_ptr<ALLEGRO_FONT> resource = _loadResource<ALLEGRO_FONT>(m_config, m_path, section, key,
                    [=](const std::string& path) {
                        return std::make_tuple(path, size, flags);
                    },
                    [](const std::tuple<std::string, int, int>& key) {
                        return ResourceCache::getFont(key);
                    },
                    [=](const std::string& path) {
                        return al_load_font(path.c_str(), size, flags);
                    },
                    [](const std::tuple<std::string, int, int>& key, ALLEGRO_FONT* font) {
                        return ResourceCache::putFont(key, font);
                    }
                );
                if (resource) {
                    return resource;
                }
            }
        }
        return defaultValue;
    }


    ALLEGRO_COLOR Theme::getColor(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        const ALLEGRO_COLOR& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
                if (!value) {
                    continue;
                }

                float r, g, b;

                if (al_color_name_to_rgb(value, &r, &g, &b)) {
                    return al_map_rgb_f(r, g, b);
                }

                float a = 1.0f;

                if (_htmlColorToRGBA(value, r, g, b, a)) {
                    return al_map_rgba_f(r, g, b, a);
                }
            }
        }
        return defaultValue;
    }



    int Theme::getInt(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        int defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
                if (!value) {
                    continue;
                }

                int v;

                std::stringstream stream;
                stream << value;
                stream >> v;

                if (stream.fail()) {
                    continue;
                }

                return v;
            }
        }
        return defaultValue;
    }


    float Theme::getFloat(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        float defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
                if (!value) {
                    continue;
                }

                float v;

                std::stringstream stream;
                stream << value;
                stream >> v;

                if (stream.fail()) {
                    continue;
                }

                return v;
            }
        }
        return defaultValue;
    }


    bool Theme::getBool(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        bool defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
                if (!value) {
                    continue;
                }
                if (_compareNoCase(value, "true") == 0) {
                    return true;
                }
                if (_compareNoCase(value, "false") == 0) {
                    return false;
                }
            }
        }
        return defaultValue;
    }


    std::string Theme::getString(
        const std::vector<std::string>& sections,
        const std::vector<std::string>& keys,
        const std::string& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const std::string& section : sections) {
            if (section.empty()) {
                continue;
            }
            for (const std::string& key : keys) {
                if (key.empty()) {
                    continue;
                }
                const char* value = al_get_config_value(m_config, section.c_str(), key.c_str());
                if (value) {
                    return value;
                }
            }
        }
        return defaultValue;
    }


    std::shared_ptr<ALLEGRO_BITMAP> Theme::getBitmap(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getBitmap(sections, std::vector<std::string>{key}, defaultValue);
        });
    }


    std::shared_ptr<ALLEGRO_FONT> Theme::getFont(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        int size,
        int flags,
        const std::shared_ptr<ALLEGRO_FONT>& defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getFont(sections, std::vector<std::string>{key}, size, flags, defaultValue);
        });
    }


    ALLEGRO_COLOR Theme::getColor(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        const ALLEGRO_COLOR& defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getColor(sections, std::vector<std::string>{key}, defaultValue);
        });
    }


    int Theme::getInt(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        int defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getInt(sections, std::vector<std::string>{key}, defaultValue);
        });
    }


    float Theme::getFloat(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        float defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getFloat(sections, std::vector<std::string>{key}, defaultValue);
        });
    }


    bool Theme::getBool(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        bool defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getBool(sections, std::vector<std::string>{key}, defaultValue);
        });
    }


    std::string Theme::getString(
        const std::string& widgetClassName,
        const std::string& widgetId,
        const std::string& surfaceType,
        const std::string& key,
        const std::string& defaultValue) const
    {
        return _getWidgetResource(widgetClassName, widgetId, surfaceType, [&](const std::vector<std::string>& sections) {
            return this->getString(sections, std::vector<std::string>{key}, defaultValue);
        });
    }


    bool Theme::addBitmap(const std::string& section, const std::string& key, const std::string& path) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), path.c_str());
            return true;
        }
        return false;
    }


    bool Theme::addFont(const std::string& section, const std::string& key, const std::string& path) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), path.c_str());
            return true;
        }
        return false;
    }


    bool Theme::addColor(const std::string& section, const std::string& key, const ALLEGRO_COLOR& color) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), _getString(color).c_str());
            return true;
        }
        return false;
    }


    bool Theme::addInt(const std::string& section, const std::string& key, int value) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), _getString(value).c_str());
            return true;
        }
        return false;
    }


    bool Theme::addFloat(const std::string& section, const std::string& key, float value) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), _getString(value).c_str());
            return true;
        }
        return false;
    }


    bool Theme::addBool(const std::string& section, const std::string& key, bool value) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), _getString(value).c_str());
            return true;
        }
        return false;
    }


    bool Theme::addString(const std::string& section, const std::string& key, const std::string& value) {
        ALLEGRO_CONFIG* config = _getConfig();
        if (config) {
            al_set_config_value(config, section.c_str(), key.c_str(), value.c_str());
            return true;
        }
        return false;
    }


    bool Theme::remove(const std::string& section, const std::string& key) {
        if (m_config) {
            al_set_config_value(m_config, section.c_str(), key.c_str(), nullptr);
            return true;
        }
        return false;
    }


    bool Theme::save(const std::string& path) {
        if (m_config) {
            return al_save_config_file(path.c_str(), m_config);
        }
        return false;
    }


    ALLEGRO_CONFIG* Theme::_getConfig() {
        if (!m_config) {
            m_config = al_create_config();
        }
        return m_config;
    }


} //namespace algui
