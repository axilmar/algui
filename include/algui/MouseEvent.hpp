#ifndef ALGUI_MOUSEMOVEEVENT_HPP
#define ALGUI_MOUSEMOVEEVENT_HPP


#include <utility>
#include "InteractiveUINodeEvent.hpp"


namespace algui {


    /**
     * Base class for mouse events.
     */
    class MouseEvent : public InteractiveUINodeEvent {
    public:
        /**
         * Constructor.
         * @param type type of eevent.
         * @param x horizontal mouse coordinate.
         * @param y vertical mouse coordinate.
         * @param z horizontal wheel coordinate.
         * @param w vertical wheel coordinate.
         * @param button mouse button.
         * @param capture capture phase flag.
         */
        MouseEvent(std::string&& type, std::shared_ptr<InteractiveUINode>&& target, int x, int y, int z, int w, int button, bool capture)
            : InteractiveUINodeEvent(std::move(type), std::move(target))
            , m_x(x)
            , m_y(y)
            , m_z(z)
            , m_w(w)
            , m_button(button)
            , m_capture(capture)
        {
        }

        /**
         * Returns the horizontal mouse coordinate, relative to the screen.
         * @return the horizontal mouse coordinate, relative to the screen.
         */
        int getX() const {
            return m_x;
        }

        /**
         * Returns the vertical mouse coordinate, relative to the screen.
         * @return the vertical mouse coordinate, relative to the screen.
         */
        int getY() const {
            return m_y;
        }

        /**
         * Returns the horizontal wheel coordinate, relative to the screen.
         * @return the horizontal wheel coordinate, relative to the screen.
         */
        int getZ() const {
            return m_z;
        }

        /**
         * Returns the vertical wheel coordinate, relative to the screen.
         * @return the vertical wheel coordinate, relative to the screen.
         */
        int getW() const {
            return m_w;
        }

        /**
         * Returns the mouse button.
         * @return the mouse button.
         */
        int getButton() const {
            return m_button;
        }

        /**
         * Returns true if the event is dispatched in capture phase, else in bubble phase.
         * @return true if the event is dispatched in capture phase, else in bubble phase.
         */
        bool isCapture() const {
            return m_capture;
        }

    private:
        int m_x;
        int m_y;
        int m_z;
        int m_w;
        int m_button;
        bool m_capture;
    };


} //namespace algui


#endif //ALGUI_MOUSEMOVEEVENT_HPP
