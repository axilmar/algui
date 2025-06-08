#ifndef ALGUI_THEME_HPP
#define ALGUI_THEME_HPP


#include <string>
#include <memory>
#pragma warning (disable: 4309)
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_color.h"


namespace algui {


    /**
     * A theme is a wrapper around ALLEGRO_CONFIG which allows loading of resources from config values.
     * Bitmaps and fonts are loaded through the resource cache functions in order to be shared via widgets.
     */
    class Theme {
    public:
        /**
         * The default constructor.
         * It creates an empty theme object.
         */
        Theme();

        /**
         * The constructor from path.
         * @param path path of the resource file to load.
         */
        Theme(const char* path);

        Theme(const Theme&) = delete;
        Theme(Theme&&) = delete;

        /**
         * Destroys the internal config file.
         */
        ~Theme();

        Theme& operator =(const Theme&) = delete;
        Theme& operator =(Theme&&) = delete;

        /**
         * Checks if it is empty.
         * @return true if there is no config file, false otherwise.
         */
        bool isEmpty() const {
            return m_config == nullptr;
        }

        /**
         * Loads a config file.
         * @param path path to the config file.
         */
        bool load(const char* path);

        /**
         * Returns a bitmap loaded from the path specified by the given section and key.
         * The path specified in the config value is relative to the allegro config path.
         * @param section section.
         * @param key key.
         * @return a bitmap loaded from the path or null if it could not be loaded.
         */
        std::shared_ptr<ALLEGRO_BITMAP> getBitmap(const char* section, const char* key) const;

        /**
         * Returns a font loaded from the path specified by the given section, key, and size/flags.
         * The path specified in the config value is relative to the allegro config path.
         * @param section section.
         * @param key key.
         * @return a font loaded from the path or null if it could not be loaded.
         */
        std::shared_ptr<ALLEGRO_FONT> getFont(const char* section, const char* key, int size, int flags) const;

        /**
         * Returns a color from either an HTML color name or an HTML color (#ffffff) from value of the given section and key.
         * @param section section.
         * @param key key.
         * @param defaultValue default value used in case of error.
         * @return the color or the default value.
         */
        ALLEGRO_COLOR getColor(const char* section, const char* key, const ALLEGRO_COLOR& defaultValue = al_map_rgb(0, 0, 0)) const;

        /**
         * Returns an int from value of the given section and key.
         * @param section section.
         * @param key key.
         * @param defaultValue default value used in case of error.
         * @return the value or the default value.
         */
        int getInt(const char* section, const char* key, int defaultValue = 0) const;

        /**
         * Returns a float from value of the given section and key.
         * @param section section.
         * @param key key.
         * @param defaultValue default value used in case of error.
         * @return the value or the default value.
         */
        float getFloat(const char* section, const char* key, float defaultValue = 0.0f) const;

        /**
         * Returns a boolean value from value of the given section and key.
         * The values recognized as true are any case versions of the word 'true', or the letter 'T', or the value 1.
         * Any other values are recongized as false.
         * @param section section.
         * @param key key.
         * @param defaultValue default value used in case of error.
         * @return the value or the default value.
         */
        bool getBool(const char* section, const char* key, bool defaultValue = false) const;

        /**
         * Returns a string from value of the given section and key.
         * @param section section.
         * @param key key.
         * @param defaultValue default value used in case of error.
         * @return the value or the default value.
         */
        std::string getString(const char* section, const char* key, const std::string& defaultValue = std::string()) const;

    private:
        ALLEGRO_CONFIG* m_config;
        std::string m_path;
    };


} //namespace algui


#endif //ALGUI_THEME_HPP

