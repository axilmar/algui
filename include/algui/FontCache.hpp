#ifndef ALGUI_FONTCACHE_HPP
#define ALGUI_FONTCACHE_HPP


#pragma warning (disable: 4309)
#include "allegro5/allegro_font.h"


namespace algui {


    /**
     * Loads a font into memory, or reuses the previously loaded font.
     * The font's reference count is incremented.
     * @param path path to font file.
     * @param size font size.
     * @param flags font flags.
     * @return the allegro font loaded or null if there was an error.
     */
    ALLEGRO_FONT* loadFont(const char* path, int size, int flags);


    /**
     * Decrements the reference count of the font.
     * If the reference count becomes 0, the font is destroyed.
     * @param path path to font file.
     * @param size font size.
     * @param flags font flags.
     * @return true if the font existed, false otherwise.
     */
    bool unloadFont(const char* path, int size, int flags);


} //namespace algui


#endif //ALGUI_FONTCACHE_HPP
