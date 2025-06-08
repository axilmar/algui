#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include "algui/Theme.hpp"
#include "algui/ResourceCache.hpp"


namespace algui {


    //make a relative path
    static std::string _relativePath(const std::string& configPath, const std::string& resourcePath) {
        return std::filesystem::relative(std::filesystem::path(resourcePath), std::filesystem::path(configPath)).string();
    }


    //the default constructor
    Theme::Theme()
        : m_config(nullptr)
    {
    }


    //the loading constructor
    Theme::Theme(const char* path) 
        : m_config(nullptr)
    {
        load(path);
    }


    //the destructor
    Theme::~Theme() {
        al_destroy_config(m_config);
    }


    //loads a config
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


    //get bitmap
    std::shared_ptr<ALLEGRO_BITMAP> Theme::getBitmap(const char* section, const char* key) const {
        if (!m_config) {
            return nullptr;
        }

        const char* path = al_get_config_value(m_config, section, key);

        if (!path) {
            return nullptr;
        }

        return loadBitmap(_relativePath(m_path, path).c_str());
    }


    //get font
    std::shared_ptr<ALLEGRO_FONT> Theme::getFont(const char* section, const char* key, int size, int flags) const {
        if (!m_config) {
            return nullptr;
        }

        const char* path = al_get_config_value(m_config, section, key);

        if (!path) {
            return nullptr;
        }

        return loadFont(_relativePath(m_path, path).c_str(), size, flags);
    }


    //get color
    ALLEGRO_COLOR Theme::getColor(const char* section, const char* key, const ALLEGRO_COLOR& defaultValue) const {
        if (!m_config) {
            return defaultValue;
        }

        const char* value = al_get_config_value(m_config, section, key);

        if (!value) {
            return defaultValue;
        }

        float r, g, b;

        if (al_color_name_to_rgb(value, &r, &g, &b)) {
            return al_map_rgb_f(r, g, b);
        }

        return al_color_html(value);
    }


    //get int
    int Theme::getInt(const char* section, const char* key, int defaultValue) const {
        if (!m_config) {
            return defaultValue;
        }

        const char* value = al_get_config_value(m_config, section, key);

        if (!value) {
            return defaultValue;
        }

        std::stringstream stream;
        stream << value;
        
        int result;
        stream >> result;

        return stream.fail() ? defaultValue : result;
    }


    //get float
    float Theme::getFloat(const char* section, const char* key, float defaultValue) const {
        if (!m_config) {
            return defaultValue;
        }

        const char* value = al_get_config_value(m_config, section, key);

        if (!value) {
            return defaultValue;
        }

        std::stringstream stream;
        stream << value;

        float result;
        stream >> result;

        return stream.fail() ? defaultValue : result;
    }


    //get bool
    bool Theme::getBool(const char* section, const char* key, bool defaultValue) const {
        if (!m_config) {
            return defaultValue;
        }

        std::string value = al_get_config_value(m_config, section, key);
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });

        if (value == "true" || value == "t" || value == "1") {
            return true;
        }

        return defaultValue;
    }


    //get string
    std::string Theme::getString(const char* section, const char* key, const std::string& defaultValue) const {
        if (!m_config) {
            return defaultValue;
        }

        const char* value = al_get_config_value(m_config, section, key);

        if (!value) {
            return defaultValue;
        }

        return value;
    }


} //namespace algui
