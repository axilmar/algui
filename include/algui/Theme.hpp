#ifndef ALGUI_THEME_HPP
#define ALGUI_THEME_HPP


#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


namespace algui {


    /**
     * A theme class.
     * 
     * Wrapper over ALLEGRO_CONFIG.
     * 
     * For resources loaded from files, the config values represent relative to the config file paths.
     * The rest of the resources are presented as values.
     * 
     * Resources for widgets can be fetched from sections with the following names, in the following order:
     * 
     *  1) [<surface type> '.' <widget id>]
     *  2) [<widget id>]
     *  3) [<surface type> '.' <widget class name>]
     *  4) [<widget class name>]
     *  5) [<surface type>]
     *
     * If the surface type is empty, then (1), (3) and (5) are skipped.
     * If the widget id is empty, then (1) and (2) are skipped.
     * If the widget class name is empty, then (3) and (4) are skipped.
     *
     * This organization allows specifying resources for
     * 
     *  1) a specific widget's surface type,
     *  2) a specific widget,
     *  3) a specific widget class' surface type,
     *  4) a specific widget class,
     *  5) a generic surface type,
     * 
     * in that specific order, allowing customization per widget, or per class, or per surface type.
     */
    class Theme {
    public:
        /**
         * Empty theme constructor.
         */
        Theme();

        /**
         * Constructor from config file.
         * @param path path to the config file.
         */
        Theme(const std::string& path);

        Theme(const Theme&) = delete;
        Theme(Theme&&) = delete;

        /**
         * The destructor.
         * Destroys the config file.
         */
        ~Theme();

        Theme& operator =(const Theme&) = delete;
        Theme& operator =(Theme&&) = delete;

        /**
         * Checks if the config is loaded or not.
         * @return true if there is no loaded config, false otherwise.
         */
        bool isEmpty() const;

        /**
         * Loads a config from a file.
         * The existing config is replaced only if the new config is successfully loaded.
         * @param path path to config.
         * @return true on success, false on failure; in case of failure, the theme contents are not modified.
         */
        bool load(const std::string& path);

        /**
         * Deletes the config, if it is not empty.
         */
        void reset();

        /**
         * Returns a bitmap from a config entry.
         * The config entry value represents a path to a bitmap relative to the config path.
         * If the bitmap is loaded, it is cached, and therefore it will not be loaded again in subsequent calls.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        std::shared_ptr<ALLEGRO_BITMAP> getBitmap(
            const std::vector<std::string>& sections, 
            const std::vector<std::string>& keys, 
            const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue = std::shared_ptr<ALLEGRO_BITMAP>()) const;

        /**
         * Returns a font from a config entry.
         * The config entry value represents a path to a font relative to the config path.
         * If the font is loaded, it is cached, and therefore it will not be loaded again in subsequent calls.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param size font size.
         * @param flags font flags.
         * @param defaultValue the default value.
         * @return either the font loaded from disk or cache, or the default value if not found.
         */
        std::shared_ptr<ALLEGRO_FONT> getFont(
            const std::vector<std::string>& sections, 
            const std::vector<std::string>& keys, 
            int size,
            int flags,
            const std::shared_ptr<ALLEGRO_FONT>& defaultValue = std::shared_ptr<ALLEGRO_FONT>()) const;

        /**
         * Returns a color from a config entry.
         * The config entry value represents either an html color name entry or an html color value (e.g. "#ff12ac").
         * Entries with alpha values are also supported (e.g. "#ff12ac05").
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the color found, or the default value if not found.
         */
        ALLEGRO_COLOR getColor(
            const std::vector<std::string>& sections,
            const std::vector<std::string>& keys,
            const ALLEGRO_COLOR& defaultValue = al_map_rgb(0, 0, 0)) const;

        /**
         * Returns an int from a config entry.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        int getInt(
            const std::vector<std::string>& sections,
            const std::vector<std::string>& keys,
            int defaultValue = 0) const;

        /**
         * Returns a float from a config entry.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        float getFloat(
            const std::vector<std::string>& sections,
            const std::vector<std::string>& keys,
            float defaultValue = 0) const;

        /**
         * Returns a boolean from a config entry.
         * The config entry value must be either "true" or "false".
         * Comparison is case insensitive.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        bool getBool(
            const std::vector<std::string>& sections,
            const std::vector<std::string>& keys,
            bool defaultValue = 0) const;

        /**
         * Returns a string from a config entry.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        std::string getString(
            const std::vector<std::string>& sections,
            const std::vector<std::string>& keys,
            const std::string& defaultValue = std::string()) const;

        /**
         * Returns an enumerated value.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        template <class E, std::enable_if_t<std::is_enum_v<E>, bool> = true>
        E getEnum(
            const std::vector<std::string>& sections,
            const std::vector<std::string>& keys,
            const E defaultValue,
            const std::map<std::string, E>& values)
        {
            std::string str = getString(sections, keys, _getDefaultValueString(values, defaultValue));
            const auto it = values.find(str);
            if (it != values.end()) {
                return it->second;
            }
            return defaultValue;
        }

        /**** widget-specific functions ****/

        /**
         * Returns a bitmap from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param key name of the property.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        std::shared_ptr<ALLEGRO_BITMAP> getBitmap(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue = std::shared_ptr<ALLEGRO_BITMAP>()) const;

        /**
         * Returns a font from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param keys names for the property value.
         * @param size font size.
         * @param flags font flags.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        std::shared_ptr<ALLEGRO_FONT> getFont(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            int size,
            int flags,
            const std::shared_ptr<ALLEGRO_FONT>& defaultValue = std::shared_ptr<ALLEGRO_FONT>()) const;

        /**
         * Returns a color from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param keys names for the property value.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        ALLEGRO_COLOR getColor(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            const ALLEGRO_COLOR& defaultValue = al_map_rgb(0, 0, 0)) const;

        /**
         * Returns an int from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param keys names for the property value.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        int getInt(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            int defaultValue = 0) const;

        /**
         * Returns a float from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param keys names for the property value.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        float getFloat(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            float defaultValue = 0) const;

        /**
         * Returns a boolean from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param keys names for the property value.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        bool getBool(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            bool defaultValue = 0) const;

        /**
         * Returns a string from widget information.
         * @param widgetClassName name of the widget class requesting the resource.
         * @param widgetId id of the widget instance requesting the resource.
         * @param surfaceType name of the surface type that represents the current surface type used by the widget.
         * @param keys names for the property value.
         * @param defaultValue the default value.
         * @return either the bitmap loaded from disk or cache, or the default value if not found.
         */
        std::string getString(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const std::vector<std::string>& keys,
            const std::string& defaultValue = std::string()) const;

        /**
         * Returns an enumerated value.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        template <class E, std::enable_if_t<std::is_enum_v<E>, bool> = true>
        E getEnum(
            const std::string& widgetClassName,
            const std::string& widgetId,
            const std::string& surfaceType,
            const E defaultValue,
            const std::map<std::string, E>& values)
        {
            std::string str = getString(widgetClassName, widgetId, surfaceType, _getDefaultValueString(values, defaultValue));
            const auto it = values.find(str);
            if (it != values.end()) {
                return it->second;
            }
            return defaultValue;
        }

        /**** mutating functions ****/

        /**
         * Adds a bitmap entry to the theme.
         * The bitmap is added to the resource cache.
         * @param section section.
         * @param key key.
         * @param path path to the bitmap, relative to the config file path.
         * @param bitmap bitmap.
         * @return true on success, false otherwise.
         */
        bool addBitmap(const std::string& section, const std::string& key, const std::string& path);

        /**
         * Adds a font entry to the theme.
         * The font is added to the resource cache.
         * @param section section.
         * @param key key.
         * @param path path to the font, relative to the config file path.
         * @return true on success, false otherwise.
         */
        bool addFont(const std::string& section, const std::string& key, const std::string& path);

        /**
         * Adds a color entry to the theme.
         * The color is saved as an html color (e.g. #f0125508).
         * @param section section.
         * @param key key.
         * @param color color.
         * @return true on success, false otherwise.
         */
        bool addColor(const std::string& section, const std::string& key, const ALLEGRO_COLOR& color);

        /**
         * Adds an integer entry to the theme.
         * @param section section.
         * @param key key.
         * @param value value.
         * @return true on success, false otherwise.
         */
        bool addInt(const std::string& section, const std::string& key, int value);

        /**
         * Adds an float entry to the theme.
         * @param section section.
         * @param key key.
         * @param value value.
         * @return true on success, false otherwise.
         */
        bool addFloat(const std::string& section, const std::string& key, float value);

        /**
         * Adds a boolean entry to the theme.
         * @param section section.
         * @param key key.
         * @param value value.
         * @return true on success, false otherwise.
         */
        bool addBool(const std::string& section, const std::string& key, bool value);

        /**
         * Adds a string entry to the theme.
         * @param section section.
         * @param key key.
         * @param value value.
         * @return true on success, false otherwise.
         */
        bool addString(const std::string& section, const std::string& key, const std::string& value);

        /**
         * Adds an enum entry to the theme.
         * @param section section.
         * @param key key.
         * @param value value.
         * @param valueStrings map of values to string.
         * @return true on success, false otherwise.
         */
        template <class E, std::enable_if_t<std::is_enum_v<E>, bool> = true>
        bool addEnum(const std::string& section, const std::string& key, E value, const std::map<E, std::string> valueStrings) {
            const auto it = valueStrings.find(value);
            if (it != valueStrings.end()) {
                return addString(section, key, value, it->second);
            }
            return false;
        }

        /**
         * Removes an entry.
         * @param section section.
         * @param key key.
         * @return true on success, false if the entry is not found.
         */
        bool remove(const std::string& section, const std::string& key);

        /**
         * Saves this theme to the given file.
         * @param path path and filename of the config file.
         * @return true on success, false otherwise.
         */
        bool save(const std::string& path);

    private:
        ALLEGRO_CONFIG* m_config;
        std::string m_path;
        ALLEGRO_CONFIG* _getConfig();

        template <class E>
        static std::string _getDefaultValueString(const std::map<std::string, E>& values, E defaultValue) {
            for (const auto p : values) {
                if (p.second == defaultValue) {
                    return p.first;
                }
            }
            throw std::invalid_argument("Theme: Invalid default value string.");
        }
    };


} //namespace algui


#endif //ALGUI_THEME_HPP
