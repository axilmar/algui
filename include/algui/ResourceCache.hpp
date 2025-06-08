#ifndef ALGUI_RESOURCECACHE_HPP
#define ALGUI_RESOURCECACHE_HPP


#include <memory>
#pragma warning (disable: 4309)
#include "allegro5/allegro_font.h"


namespace algui {


    /**
     * Loads a font from a file into memory, or reuses the previously loaded font.
     * @param path path to font file.
     * @param size font size.
     * @param flags font flags.
     * @return the allegro font loaded or null if there was an error.
     */
    std::shared_ptr<ALLEGRO_FONT> loadFont(const char* path, int size, int flags);


    /**
     * Loads a bitmap from a file into memory, or reuses the previously loaded bitmap.
     * @param path path to bitmap file.
     * @return the allegro bitmap loaded or null if there was an error.
     */
    std::shared_ptr<ALLEGRO_BITMAP> loadBitmap(const char* path);


} //namespace algui


#endif //ALGUI_RESOURCECACHE_HPP
