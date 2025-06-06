#include <map>
#include <string>
#include <tuple>
#include "algui/FontCache.hpp"


namespace algui {


    using _FontCacheKey = std::tuple<std::string, int, int>;


    struct _FontCacheValue {
        ALLEGRO_FONT* font;
        size_t refCount;

        _FontCacheValue() : font(nullptr), refCount(0) {
        }

        _FontCacheValue(ALLEGRO_FONT* f) : font(f), refCount(1) {
        }

        ~_FontCacheValue() {
            al_destroy_font(font);
        }
    };


    using _FontCacheMap = std::map<_FontCacheKey, _FontCacheValue>;


    static _FontCacheMap& _getFontCacheMap() {
        static _FontCacheMap _fontCacheMap;
        return _fontCacheMap;
    }


    //Loads a font into memory, or reuses the previously loaded font.
    ALLEGRO_FONT* loadFont(const char* path, int size, int flags) {
        _FontCacheMap& map = _getFontCacheMap();

        //create the key and find it
        _FontCacheKey key = std::make_tuple(std::string(path), size, flags);
        const auto it = map.find(key);

        //if found, increment the ref count and return the font
        if (it != map.end()) {
            ++it->second.refCount;
            return it->second.font;
        }

        //not found; load the font
        ALLEGRO_FONT* font = al_load_font(path, size, flags);
        if (!font) {
            return nullptr;
        }

        //font found; keep it
        map[key] = _FontCacheValue(font);
        return font;
    }


    //Decrements the reference count of the font; unloads the font if the ref count reaches 0.
    bool unloadFont(const char* path, int size, int flags) {
        _FontCacheMap& map = _getFontCacheMap();

        //create the key and find it
        _FontCacheKey key = std::make_tuple(std::string(path), size, flags);
        const auto it = map.find(key);

        //if not found, return false
        if (it == map.end()) {
            return false;
        }

        //decrement its ref count; if ref count not zero yet, do nothing else
        if (--it->second.refCount > 0) {
            return true;
        }

        //unload the font
        map.erase(key);

        //success
        return true;
    }


} //namespace algui
