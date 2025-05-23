#ifndef ALGUI_FONTCACHE_HPP
#define ALGUI_FONTCACHE_HPP


#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"


namespace algui {


    /**
     * Font cache implementation.
     */
    class FontCache {
    public:
        /**
         * It retrieves a font either from internal cache or from disk.
         * If retrieved from disk, then it is stored in cache for later retrieval.
         * @param filename name of the font file.
         * @param size font size.
         * @param flags font flags.
         * @return the font or null if not found.
         */
        static ALLEGRO_FONT* getFont(const char* filename, int size, int flags);
    };


} //namespace algui


#endif //ALGUI_FONTCACHE_HPP
