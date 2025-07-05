#ifndef ALGUI_THEME_HPP
#define ALGUI_THEME_HPP


#include <memory>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


namespace algui {


    /**
     * A theme.
     * 
     * It contains an ALLEGRO_CONFIG structure, which can contain:
     * 
     *  - paths to bitmaps.
     *  - paths to fonts.
     *  - colors (either html colors or html color names).
     *  - int values.
     *  - float values.
     *  - bool values ("true" for true, anything else for false).
     *  - string values.
     * 
     * Paths are relative to the config file path.
     * 
     * Bitmaps and fonts are cached through the class `ResourceCache`.
     */
    class Theme {
    public:
        /**
         * The default constructor.
         * The theme is empty.
         */
        Theme();

        /**
         * Constructor from config file.
         * @param filepath path to config file.
         */
        Theme(const std::string& filepath);

        Theme(const Theme&) = delete;        
        Theme(Theme&&) = delete;

        /**
         * The destructor.
         * It destroys the internal config.
         */
        ~Theme();

        Theme& operator =(const Theme&) = delete;
        Theme& operator =(Theme&&) = delete;

        /**
         * Returns a pointer to the internal allegro config.
         * @return a pointer to the internal allegro config.
         */
        const ALLEGRO_CONFIG* get() const {
            return m_config;
        }

        /**
         * Returns a pointer to the internal allegro config.
         * @return a pointer to the internal allegro config.
         */
        operator const ALLEGRO_CONFIG* () const {
            return m_config;
        }

        /**
         * Returns a pointer to the internal allegro config.
         * @return a pointer to the internal allegro config.
         */
        operator ALLEGRO_CONFIG* () {
            return m_config;
        }

        /**
         * Returns the path of the config file.
         * @return the path of the config file.
         */
        const std::string& getPath() const {
            return m_path;
        }

        /**
         * Returns the filename of the config file.
         * @return the filename of the config file.
         */
        const std::string& getFilename() const {
            return m_filename;
        }

        /**
         * Checks if the theme is empty.
         * @return true if empty, false otherwise.
         */
        bool isEmpty() const {
            return m_config == nullptr;
        }

        /**
         * Destroys the config file.
         */
        void reset();

        /**
         * Loads the config file.
         * @param filepath path to config file.
         * @return true on success, false if there was an error.
         */
        bool load(const std::string& filepath);

        /**
         * Returns a bitmap.
         * The loaded resource is cached through the class `ResourceCache`.
         * @param section config section.
         * @param key config key; the value must contain a path relative to the config path.
         * @param defaultValue the value to return in case of error.
         * @return the loaded resource or the default.
         */
        std::shared_ptr<ALLEGRO_BITMAP> getBitmap(const std::string& section, const std::string& key, const std::shared_ptr<ALLEGRO_BITMAP>& defaultValue = {});

        /**
         * Returns a font.
         * The loaded resource is cached through the class `ResourceCache`.
         * @param section config section.
         * @param key config key; the value must contain a path relative to the config path.
         * @param size font size.
         * @param flags font flags.
         * @param defaultValue the value to return in case of error.
         * @return the loaded resource or the default.
         */
        std::shared_ptr<ALLEGRO_FONT> getFont(const std::string& section, const std::string& key, int size, int flags, const std::shared_ptr<ALLEGRO_FONT>& defaultValue = {});

        /**
         * Returns a color.
         * @param section config section.
         * @param key config key; the value must contain either an html color (i.e.g #ffaabb11[cc], alpha is optional) or an html color name.
         * @param defaultValue the value to return in case of error.
         * @return the resource or the default.
         */
        ALLEGRO_COLOR getColor(const std::string& section, const std::string& key, const ALLEGRO_COLOR& defaultValue = al_map_rgb(0, 0, 0));

        /**
         * Returns an integer.
         * @param section config section.
         * @param key config key.
         * @param defaultValue the value to return in case of error.
         * @return the resource or the default.
         */
        int getInt(const std::string& section, const std::string& key, int defaultValue = 0);

        /**
         * Returns a float.
         * @param section config section.
         * @param key config key.
         * @param defaultValue the value to return in case of error.
         * @return the resource or the default.
         */
        float getFloat(const std::string& section, const std::string& key, float defaultValue = 0);

        /**
         * Returns a boolean.
         * @param section config section.
         * @param key config key; the value must contain "true" for true, anything else for false.
         * @param defaultValue the value to return in case of error.
         * @return the resource or the default.
         */
        bool getBool(const std::string& section, const std::string& key, bool defaultValue = false);

        /**
         * Returns a string.
         * @param section config section.
         * @param key config key.
         * @param defaultValue the value to return in case of error.
         * @return the resource or the default.
         */
        std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue = {});

    private:
        ALLEGRO_CONFIG* m_config;
        std::string m_path;
        std::string m_filename;
    };


} //namespace algui


#endif //ALGUI_THEME_HPP
