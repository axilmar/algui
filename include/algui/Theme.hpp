#ifndef ALGUI_THEME_HPP
#define ALGUI_THEME_HPP


#include <any>
#include <string>
#include <map>
#include <tuple>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_color.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_audio.h"


namespace algui {


    /**
     * A theme is a collection of:
     * 
     *  - bitmaps
     *  - fonts
     *  - samples
     *  - colors
     *  - values: integers, floats, booleans and string.
     * 
     * It holds the data required for widgets to present themselves with specific graphics and sound.
     * 
     * The data can also be loaded from an allegro config file, which contains key and value pairs.
     * 
     * For each type of object, the following information is kept:
     * 
     *  - bitmaps: path to the bitmap file.
     *  - fonts: path to the font file.
     *  - samples: path to the sample file.
     *  - colors: either an html color name or an html color value (e.g. either blue or #0000ff).
     *  - integers: any integer.
     *  - floats: any float with a suffix (e.g. 32f or 3.2f).
     *  - booleans: true, True, TRUE and false, False or FALSE.
     *  - strings: any string.
     * 
     * For example:
     * 
     *  button.disabled.backgroundBitmap = ./button/backgroundDisabled.jpg
     *  button.disabled.backgroundColor = #cccccc
     *  button.enabled.font = roboto.ttf
     * 
     * There is no limit as what the keys are, they can be anything.
     * 
     * A theme object is the owner of all the resources loaded.
     * When it is destroyed, all resources of it are destroyed.
     */
    class Theme {
    public:
        /**
         * The default constructor.
         * An empty theme is created.
         */
        Theme();

        /**
         * A constructor from a file.
         * @param filename name of the config file to open.
         */
        Theme(const char* filename);

        Theme(const Theme& theme) = delete;
        Theme(Theme&& theme) = delete;

        /**
         * The destructor.
         * It destroys all resources.
         */
        virtual ~Theme();

        Theme& operator = (const Theme& theme) = delete;
        Theme& operator = (Theme&& theme) = delete;

        /**
         * Returns a bitmap.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        ALLEGRO_BITMAP* getBitmap(const char* name, ALLEGRO_BITMAP* defaultValue = nullptr) const;

        /**
         * Sets a bitmap.
         * The bitmap will be destroyed when the theme is destroyed.
         * The previous bitmap is destroyed, if it exists.
         * @param name the name of the resource.
         * @param bitmap the new resource.
         */
        void setBitmap(const char* name, ALLEGRO_BITMAP* bitmap);

        /**
         * Returns a color.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        ALLEGRO_COLOR getColor(const char* name, const ALLEGRO_COLOR& defaultValue = al_map_rgb(0, 0, 0)) const;

        /**
         * Sets a color.
         * @param name the name of the resource.
         * @param color the new resource.
         */
        void setColor(const char* name, ALLEGRO_COLOR color);

        /**
         * Returns a font.
         * The font will be cached via the class FontCache.
         * @param name name of the resource.
         * @param size size of the font.
         * @param flags font flags.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        ALLEGRO_FONT* getFont(const char* name, int size, int flags, ALLEGRO_FONT* defaultValue) const;

        /**
         * Sets a font.
         * The font will be destroyed when the theme is destroyed.
         * The previous font is destroyed, if it exists.
         * @param name the name of the resource.
         * @param font the new resource.
         * @param size font size.
         * @param flags font flags.
         */
        void setFont(const char* name, ALLEGRO_FONT* font, int size, int flags);

        /**
         * Returns an integer.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        int getInt(const char* name, int defaultValue = int()) const;

        /**
         * Sets an integer.
         * @param name the name of the resource.
         * @param value the new resource.
         */
        void setInt(const char* name, int value);

        /**
         * Returns a float.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        float getFloat(const char* name, float defaultValue = float()) const;

        /**
         * Sets a float.
         * @param name the name of the resource.
         * @param value the new resource.
         */
        void setFloat(const char* name, float value);

        /**
         * Returns a resource that does not fit any of the supported categories.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        std::any getAny(const char* name, const std::any& defaultValue = std::any()) const;

        /**
         * Sets a resource that does not fit any of the supported categories.
         * @param name the name of the resource.
         * @param any the new resource.
         */
        void setAny(const char* name, const std::any& any);

        /**
         * Returns a sample.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        ALLEGRO_SAMPLE* getSample(const char* name, ALLEGRO_SAMPLE* defaultValue) const;

        /**
         * Sets a sample.
         * The sample will be destroyed when the theme is destroyed.
         * The previous sample is destroyed, if it exists.
         * @param name the name of the resource.
         * @param sample the new resource.
         */
        void setSample(const char* name, ALLEGRO_SAMPLE* sample);

        /**
         * Returns a boolean.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        bool getBool(const char* name, bool defaultValue = false) const;

        /**
         * Sets a boolean.
         * @param name the name of the resource.
         * @param value the new resource.
         */
        void setBool(const char* name, bool value);

        /**
         * Returns a string.
         * @param name name of the resource.
         * @param defaultValue default value for the resource.
         * @return the found resource or the default value if there is no resource with that given name.
         */
        std::string getString(const char* name, const std::string& defaultValue = std::string()) const;

        /**
         * Sets a string.
         * @param name the name of the resource.
         * @param value the new resource.
         */
        void setString(const char* name, const std::string& value);

    private:
        //objects that need cleanup
        std::map<std::string, ALLEGRO_BITMAP*> m_bitmaps;
        std::map<std::tuple<std::string, int, int>, ALLEGRO_FONT*> m_fonts;
        std::map<std::string, ALLEGRO_SAMPLE*> m_samples;

        //fonts are kept via filename
        std::map<std::string, std::string> m_fontPaths;

        //values
        std::map<std::string, ALLEGRO_COLOR> m_colors;
        std::map<std::string, bool> m_bools;
        std::map<std::string, float> m_floats;
        std::map<std::string, int> m_ints;
        std::map<std::string, std::string> m_strings;
        std::map<std::string, std::any> m_any;
    };


} //namespace algui


#endif //ALGUI_THEME_HPP
