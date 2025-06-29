#ifndef ALGUI_EVENT_HPP
#define ALGUI_EVENT_HPP


#include <string>
#include <utility>


namespace algui {


    /**
     * Base class for events.
     */
    class Event {
    public:
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
        const std::string& getType() const {
            return m_type;
        }

    protected:
        /**
         * The constructor.
         * @param type event type.
         */
        Event(std::string&& type) : m_type(std::move(type)) {
        }

    private:
        std::string m_type;
    };


} //namespace algui


#endif //ALGUI_EVENT_HPP
