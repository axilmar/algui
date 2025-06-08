#include <map>
#include <string>
#include <tuple>
#include <functional>
#include "algui/ResourceCache.hpp"


namespace algui {


    //internal resource map object
    template <class Key, class Value>
    class _ResourceMap {
    private:
        //entry
        struct _Entry {
            Key key;
            std::weak_ptr<Value> value;
        };

    public:
        //loader function type
        using Loader = std::shared_ptr<Value> (*)(const Key&, _ResourceMap<Key, Value>*);

        //the default constructor
        _ResourceMap(const Loader& loader)
            : m_loader(loader)
        {
        }

        //load a resource
        std::shared_ptr<Value> load(const Key& key) {
            const auto it = m_keyToEntry.find(key);

            //resource found; return it as a shared ptr
            if (it != m_keyToEntry.end()) {
                return it->second.value.lock();
            }

            //load value
            std::shared_ptr<Value> value = m_loader(key, this);

            //value loaded successfully
            if (value) {
                m_keyToEntry[key] = _Entry{ key, value };
                return value;
            }

            //value failed to load
            return nullptr;
        }

        //called by the shared ptr destructor
        void remove(const Key& key) {
            m_keyToEntry.erase(key);
        }

    private:
        Loader m_loader;
        std::map<Key, _Entry> m_keyToEntry;
    };


    using _FontKey = std::tuple<std::string, int, int>;


    //font loader
    static std::shared_ptr<ALLEGRO_FONT> _fontLoader(const _FontKey& key, _ResourceMap<_FontKey, ALLEGRO_FONT>* resourceMap) {
        ALLEGRO_FONT* font = al_load_font(std::get<0>(key).c_str(), std::get<1>(key), std::get<2>(key));
        
        if (!font) {
            return nullptr;
        }

        std::shared_ptr<ALLEGRO_FONT> result{ font, [resourceMap, key](ALLEGRO_FONT* font) {
            resourceMap->remove(key);
            al_destroy_font(font);
        }};

        return result;
    }


    //bitmap loader
    static std::shared_ptr<ALLEGRO_BITMAP> _bitmapLoader(const std::string& key, _ResourceMap<std::string, ALLEGRO_BITMAP>* resourceMap) {
        ALLEGRO_BITMAP* bitmap = al_load_bitmap(key.c_str());

        if (!bitmap) {
            return nullptr;
        }

        std::shared_ptr<ALLEGRO_BITMAP> result{ bitmap, [resourceMap, key](ALLEGRO_BITMAP* bitmap) {
            resourceMap->remove(key);
            al_destroy_bitmap(bitmap);
        } };

        return result;
    }


    //get font resource map
    static _ResourceMap<_FontKey, ALLEGRO_FONT>& _getFontResourceMap() {
        static _ResourceMap<_FontKey, ALLEGRO_FONT> _fontResourceMap(_fontLoader);
        return _fontResourceMap;
    }


    //get bitmap resource map
    static _ResourceMap<std::string, ALLEGRO_BITMAP>& _getBitmapResourceMap() {
        static _ResourceMap<std::string, ALLEGRO_BITMAP> _bitmapResourceMap(_bitmapLoader);
        return _bitmapResourceMap;
    }


    //Loads a font into memory, or reuses the previously loaded font.
    std::shared_ptr<ALLEGRO_FONT> loadFont(const char* path, int size, int flags) {
        return _getFontResourceMap().load(std::make_tuple(std::string(path), size, flags));
    }


    //Loads a bitmap from a file into memory, or reuses the previously loaded bitmap.
    std::shared_ptr<ALLEGRO_BITMAP> loadBitmap(const char* path) {
        return _getBitmapResourceMap().load(path);
    }


} //namespace algui
