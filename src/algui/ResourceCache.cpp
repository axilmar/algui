#include <map>
#include "algui/ResourceCache.hpp"


namespace algui {


    template <class K, class V> class _ResourceMap {
    public:
        std::shared_ptr<V> get(const K& key) const {
            const auto it = m_resources.find(key);
            if (it != m_resources.end()) {
                return it->second.lock();
            }
            return std::shared_ptr<V>();
        }

        template <class F>
        std::shared_ptr<V> put(const K& key, V* v, const F& dtor) {
            const auto eraser = [=](V* v) {
                m_resources.erase(key);
                dtor(v);
            };
            std::shared_ptr<V> ptr{v, eraser};
            m_resources[key] = ptr;
            return ptr;
        }

    private:
        std::map<K, std::weak_ptr<V>> m_resources;
    };


    using _BitmapKey = ResourceCache::BitmapKey;
    using _FontKey = ResourceCache::FontKey;


    using _BitmapResourceMap = _ResourceMap<_BitmapKey, ALLEGRO_BITMAP>;
    using _FontResourceMap = _ResourceMap<_FontKey, ALLEGRO_FONT>;


    static _BitmapResourceMap& _getBitmapResourceMap() {
        static _BitmapResourceMap map;
        return map;
    }


    static _FontResourceMap& _getFontResourceMap() {
        static _FontResourceMap map;
        return map;
    }


    std::shared_ptr<ALLEGRO_BITMAP> ResourceCache::getBitmap(const BitmapKey& key) {
        return _getBitmapResourceMap().get(key);
    }


    std::shared_ptr<ALLEGRO_FONT> ResourceCache::getFont(const FontKey& key) {
        return _getFontResourceMap().get(key);

    }


    std::shared_ptr<ALLEGRO_BITMAP> ResourceCache::putBitmap(const BitmapKey& key, ALLEGRO_BITMAP* bitmap) {
        return _getBitmapResourceMap().put(key, bitmap, &al_destroy_bitmap);
    }


    std::shared_ptr<ALLEGRO_FONT> ResourceCache::putFont(const FontKey& key, ALLEGRO_FONT* font) {
        return _getFontResourceMap().put(key, font, &al_destroy_font);
    }


} //namespace algui
