#ifndef ALGUI_RESOURCECACHE_HPP
#define ALGUI_RESOURCECACHE_HPP


#include <memory>
#include <string>
#include <tuple>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


namespace algui {


    /**
     * Resource cache.
     * 
     * Allows reuse of fonts and bitmaps.
     */
    class ResourceCache {
    public:
        /**
         * Bitmaps are referenced via their path.
         */
        using BitmapKey = std::string;

        /**
         * Fonts are referenced via their path, size and flags.
         */
        using FontKey = std::tuple<std::string, int, int>;

        /**
         * Returns a bitmap.
         * @param key key for the resource.
         * @return pointer to the resource or an empty pointer if the resource does not exist.
         */
        static std::shared_ptr<ALLEGRO_BITMAP> getBitmap(const BitmapKey& key);

        /**
         * Returns a font.
         * @param key key for the resource.
         * @return pointer to the resource or an empty pointer if the resource does not exist.
         */
        static std::shared_ptr<ALLEGRO_FONT> getFont(const FontKey& key);

        /**
         * Puts a bitmap into the resource cache.
         * @param key the key for the resource.
         * @param bitmap the resource.
         * @return shared pointer to the resource; when the last shared pointer of it goes away,
         *  the resource entry is removed automatically from the resource cache.
         */
        static std::shared_ptr<ALLEGRO_BITMAP> putBitmap(const BitmapKey& key, ALLEGRO_BITMAP* bitmap);

        /**
         * Puts a font into the resource cache.
         * @param key the key for the resource.
         * @param font the resource.
         * @return shared pointer to the resource; when the last shared pointer of it goes away,
         *  the resource entry is removed automatically from the resource cache.
         */
        static std::shared_ptr<ALLEGRO_FONT> putFont(const FontKey& key, ALLEGRO_FONT* font);
    };


} //namespace algui


#endif //ALGUI_RESOURCECACHE_HPP
