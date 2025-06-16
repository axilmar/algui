#ifndef ALGUI_THEME_HPP
#define ALGUI_THEME_HPP


#include <memory>
#include <initializer_list>
#include <string>
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
        Theme(const char* path);

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
         */
        bool load(const char* path);

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
            const std::initializer_list<const char*>& sections, 
            const std::initializer_list<const char*>& keys, 
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
            const std::initializer_list<const char*>& sections, 
            const std::initializer_list<const char*>& keys, 
            int size,
            int flags,
            const std::shared_ptr<ALLEGRO_FONT>& defaultValue = std::shared_ptr<ALLEGRO_FONT>()) const;

        /**
         * Returns a color from a config entry.
         * The config entry value represents either an html color entry or an html color value (e.g. "#ff12ac").
         * Entries with alpha values are also supported (e.g. "#ff12ac05").
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the color found, or the default value if not found.
         */
        ALLEGRO_COLOR getColor(
            const std::initializer_list<const char*>& sections,
            const std::initializer_list<const char*>& keys,
            const ALLEGRO_COLOR& defaultValue = al_map_rgb(0, 0, 0)) const;

        /**
         * Returns an int from a config entry.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        int getInt(
            const std::initializer_list<const char*>& sections,
            const std::initializer_list<const char*>& keys,
            int defaultValue = 0) const;

        /**
         * Returns a float from a config entry.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        float getFloat(
            const std::initializer_list<const char*>& sections,
            const std::initializer_list<const char*>& keys,
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
            const std::initializer_list<const char*>& sections,
            const std::initializer_list<const char*>& keys,
            bool defaultValue = 0) const;

        /**
         * Returns a string from a config entry.
         * @param sections sections to search.
         * @param keys keys to search for.
         * @param defaultValue the default value.
         * @return either the value found, or the default value if not found.
         */
        const char* getString(
            const std::initializer_list<const char*>& sections,
            const std::initializer_list<const char*>& keys,
            const char* defaultValue = nullptr) const;

    private:
        ALLEGRO_CONFIG* m_config;
        std::string m_path;
    };


} //namespace algui


#endif //ALGUI_THEME_HPP
