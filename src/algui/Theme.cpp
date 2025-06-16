#include <tuple>
#include <map>
#include <filesystem>
#include <sstream>
#include <allegro5/allegro_color.h>
#include "algui/Theme.hpp"


namespace algui {


    using _BitmapKey = std::string;
    using _FontKey = std::tuple<std::string, int, int>;


    static std::map<_BitmapKey, std::weak_ptr<ALLEGRO_BITMAP>> _bitmaps;
    static std::map<_FontKey, std::weak_ptr<ALLEGRO_FONT>> _fonts;


    template <class K, class V, class KF, class LF, class DF> 
    static std::shared_ptr<V> _loadResource(
        ALLEGRO_CONFIG* config,
        const std::string& configPath,
        const char* section,
        const char* key,
        std::map<K, std::weak_ptr<V>>& cache,
        const KF& keygen,
        const LF& loader,
        const DF& dtor)
    {
        //get path value from config file
        const char* pathValue = al_get_config_value(config, section, key);

        //no path value
        if (!pathValue) {
            return nullptr;
        }

        try {
            //get the actual path relative to the config path
            const std::string path = std::filesystem::canonical((std::filesystem::path(configPath) / std::filesystem::path(pathValue))).string();

            //get the key
            const K key = keygen(path);

            //find the cached resource
            auto it = cache.find(key);

            //if found, return it
            if (it != cache.end()) {
                return it->second.lock();
            }

            //load the resource
            V* resource = loader(path);

            //if the resource could not be loaded
            if (!resource) {
                return nullptr;
            }

            //the result; when destroyed, not only the resource is destroyed, but the cache entry is removed as well
            std::shared_ptr<V> result{ resource, [dtor, key, &cache](V* resource)
                {
                    dtor(resource);
                    cache.erase(key);
                }
            };

            //cache the result
            cache[key] = result;

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


    Theme::Theme() 
        : m_config(nullptr)
    {
    }


    Theme::Theme(const char* path)
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


    bool Theme::load(const char* path) {
        ALLEGRO_CONFIG* config = al_load_config_file(path);
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
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                std::shared_ptr<ALLEGRO_BITMAP> resource = _loadResource(m_config, m_path, section, key, _bitmaps, 
                    [](const std::string& path) {
                        return path;
                    },
                    [](const std::string& path) {
                        return al_load_bitmap(path.c_str());
                    },
                    [](ALLEGRO_BITMAP* bmp) {
                        al_destroy_bitmap(bmp);
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
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        int size,
        int flags,
        const std::shared_ptr<ALLEGRO_FONT>& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                std::shared_ptr<ALLEGRO_FONT> resource = _loadResource(m_config, m_path, section, key, _fonts,
                    [&](const std::string& path) {
                        return std::make_tuple(path, size, flags);
                    },
                    [&](const std::string& path) {
                        return al_load_font(path.c_str(), size, flags);
                    },
                    [](ALLEGRO_FONT* font) {
                        al_destroy_font(font);
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
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        const ALLEGRO_COLOR& defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                const char* value = al_get_config_value(m_config, section, key);
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
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        int defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                const char* value = al_get_config_value(m_config, section, key);
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
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        float defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                const char* value = al_get_config_value(m_config, section, key);
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
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        bool defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                const char* value = al_get_config_value(m_config, section, key);
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


    const char* Theme::getString(
        const std::initializer_list<const char*>& sections,
        const std::initializer_list<const char*>& keys,
        const char* defaultValue) const
    {
        if (!m_config) {
            return defaultValue;
        }
        for (const char* section : sections) {
            for (const char* key : keys) {
                const char* value = al_get_config_value(m_config, section, key);
                if (value) {
                    return value;
                }
            }
        }
        return defaultValue;
    }

} //namespace algui
