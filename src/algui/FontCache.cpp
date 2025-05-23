#include <map>
#include <string>
#include <tuple>
#pragma warning (disable: 4309)
#include "algui/FontCache.hpp"


namespace algui {


    class FontCacheImpl {
    public:
        ALLEGRO_FONT* getFont(const char* filename, int size, int flags) {
            const auto key = std::make_tuple(std::string(filename), size, flags);

            //get the font from the cache
            const auto it = m_fonts.find(key);
            if (it != m_fonts.end()) {
                return it->second;
            }

            //load the font
            ALLEGRO_FONT* font = al_load_font(filename, size, flags);
            if (font) {
                m_fonts[key] = font;
                return font;
            }

            //not found
            return nullptr;
        }

        ~FontCacheImpl() {
            for (const auto& p : m_fonts) {
                al_destroy_font(p.second);
            }
        }

    private:
        using Key = std::tuple<std::string, int, int>;
        std::map<Key, ALLEGRO_FONT*> m_fonts;
    };


    static FontCacheImpl fontClassImpl;


    ALLEGRO_FONT* FontCache::getFont(const char* filename, int size, int flags) {
        return fontClassImpl.getFont(filename, size, flags);
    }


} //namespace algui
