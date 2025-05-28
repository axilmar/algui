#ifndef ALGUI_EVENT_HPP
#define ALGUI_EVENT_HPP


#include <string_view>


namespace algui {


    /**
     * Base class for events.
     */
    class Event {
    public:
        /**
         * The destructor.
         * It is virtual, due to event inheritance.
         */
        virtual ~Event() {
        }
    };


} //namespace algui


#endif //ALGUI_EVENT_HPP
