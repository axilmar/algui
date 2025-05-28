#ifndef ALGUI_EVENTTYPE_HPP
#define ALGUI_EVENTTYPE_HPP


#include <string>


namespace algui {


    /**
     * Event types.
     */
    class EventType {
    public:
        /** Child added event; fired by the node that a child was added to it; event points to the added child; does not bubble. */
        static const std::string childAdded;

        /** Child removed event; fired by the node that a child was removed from it; event points to the removed child; does not bubble. */
        static const std::string childRemoved;

        /** Node enabled event; fired by the node that became enabled. */
        static const std::string enabled;

        /** Node disabled event; fired by the node that became disabled. */
        static const std::string disabled;

        /** focus; fired by the node that gets the focus; does not bubble. */
        static const std::string focus;

        /** focus in; fired by the node that gets the focus; bubbles up to root. */
        static const std::string focusIn;

        /** blur; fired by the node that loses the focus; does not bubble. */
        static const std::string blur;

        /** focus out; fired by the node that loses the focus; bubbles up to root. */
        static const std::string focusOut;

        /** mouse entered; fired when the mouse enters a node's area, including any children. Event points to ALLEGRO_EVENT. */
        static const std::string mouseEnter;

        /** mouse moved; fired when the mouse moves over a node's area, including any children. Event points to ALLEGRO_EVENT. */
        static const std::string mouseMove;

        /** mouse left; fired when the mouse leaves node's area. Event points to ALLEGRO_EVENT. */
        static const std::string mouseLeave;

        /** mouse button pressed; fired when the mouse is pressed over a node. Event points to ALLEGRO_EVENT. */
        static const std::string mouseButtonDown;

        /** mouse button held pressed; fired when the mouse is pressed continuously over a node. Event points to ALLEGRO_EVENT. */
        static const std::string mouseButtonHeldDown;

        /** mouse button released; fired when the mouse is released over a node. Event points to ALLEGRO_EVENT. */
        static const std::string mouseButtonUp;

        /** mouse wheel; fired when the mouse wheel is rolled over a node. Event points to ALLEGRO_EVENT. */
        static const std::string mouseWheel;

        /** mouse button clicked; fired when the mouse is pressed and then released over the same node within a specified period of time.
          Event points to ALLEGRO_EVENT. */
        static const std::string click;

        /** mouse button clicked; fired when the mouse is pressed and then released twice over the same node within a specified period of time.
          Event points to ALLEGRO_EVENT. */
        static const std::string doubleClick;

        /** mouse entered while in drag and drop. Event points to ALLEGRO_EVENT. */
        static const std::string dragEnter;

        /** mouse moved while in drag and drop. Event points to ALLEGRO_EVENT. */
        static const std::string drag;

        /** mouse left while in drag and drop. Event points to ALLEGRO_EVENT. */
        static const std::string dragLeave;

        /** mouse released while in drag and drop. Event points to ALLEGRO_EVENT. */
        static const std::string drop;

        /** a key was pressed when the node has the input focus. Event points to ALLEGRO_EVENT. */
        static const std::string keyDown;

        /** a key was released when the node has the input focus. Event points to ALLEGRO_EVENT. */
        static const std::string keyUp;

        /** a character was created from a key press, while the node has the input focus. Event points to ALLEGRO_EVENT. */
        static const std::string keyChar;

        /** a key was pressed but the node with the input focus ignored it.
            Fired only in the bubble phase. Event points to ALLEGRO_EVENT. */
        static const std::string unusedKeyDown;

        /** a key was released but the node with the input focus ignored it.
            Fired only in the bubble phase. Event points to ALLEGRO_EVENT. */
        static const std::string unusedKeyUp;

        /** a character was generated as the result of a key press, but the node with the input focus ignored it.
            Fired only in the bubble phase. Event points to ALLEGRO_EVENT. */
        static const std::string unusedKeyChar;

        /** a timer event; does not have a phase (so it uses phase 0). Event points to ALLEGRO_EVENT. */
        static const std::string timer;
    };


} //namespace algui


#endif //ALGUI_EVENTTYPE_HPP
