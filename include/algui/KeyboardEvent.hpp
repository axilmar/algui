#ifndef ALGUI_KEYBOARDEVENT_HPP
#define ALGUI_KEYBOARDEVENT_HPP


#include <utility>
#include "Event.hpp"


namespace algui {


    /**
     * Base class for keyboard events.
     */
    class KeyboardEvent : public Event {
    public:
        /**
         * Constructor.
         * @param type type of event.
         * @param x horizontal mouse coordinate.
         * @param keycode ALLEGRO keycode.
         * @param char_ unicode character.
         * @param modifiers ALLEGRO modifiers bitfield.
         * @param repeated if event is repeated.
         */
        KeyboardEvent(const std::string_view& type, int keycode, int char_, unsigned int modifiers, bool repeated)
            : Event(type)
            , m_keycode(keycode)
            , m_char(char_)
            , m_modifiers(modifiers)
            , m_repeated(repeated)
        {
        }

        /**
         * Returns the keycode.
         * @return the keycode.
         */
        int getKeyCode() const {
            return m_keycode;
        }

        /**
         * Returns the character code.
         * @return the character code.
         */
        int getChar() const {
            return m_char;
        }

        /**
         * Returns the modifiers.
         * @return the modifiers.
         */
        unsigned int getModifiers() const {
            return m_modifiers;
        }

        /**
         * Returns the repeat status of the event.
         * @return the repeat status of the event.
         */
        bool isRepeated() const {
            return m_repeated;
        }

    private:
        int m_keycode;
        int m_char;
        unsigned int m_modifiers;
        bool m_repeated;
    };


} //namespace algui


#endif //ALGUI_KEYBOARDEVENT_HPP
