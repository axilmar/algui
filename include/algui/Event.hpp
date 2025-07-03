#ifndef ALGUI_EVENT_HPP
#define ALGUI_EVENT_HPP


#include <string_view>
#include <utility>


namespace algui {


    /**
     * Base class for events.
     */
    class Event {
    public:
        /**
         * The constructor.
         * @param type event type.
         */
        Event(const std::string_view& type) : m_type(type) {
        }

        /**
         * The destructor.
         * Virtual due to inheritance.
         */
        virtual ~Event() {
        }

        /**
         * Returns the event type.
         * @return the event type.
         */
        const std::string_view& getType() const {
            return m_type;
        }

    private:
        std::string_view m_type;
    };


} //namespace algui


#endif //ALGUI_EVENT_HPP
