#ifndef ALGUI_UIINTERACTIVENODE_HPP
#define ALGUI_UIINTERACTIVENODE_HPP


#include "UIVisualStateNode.hpp"
#include "EventTarget.hpp"
#include "allegro5/allegro.h"


namespace algui {


    /**
     * Event types.
     */
    class EventType {
    public:
        /** focus; received by node that gets the focus; does not bubble. */
        static const std::string focus;

        /** focus in; received by node that gets the focus; bubbles up to root. */
        static const std::string focusIn;

        /** blur; received by node that loses the focus; does not bubble. */
        static const std::string blur;

        /** focus out; received by node that loses the focus; bubbles up to root. */
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


    /**
     * Event phase.
     */
    enum class EventPhase {
        /**
         * Capture phase.
         * Events are dispatched from parent to child.
         */
        Capture = 0,

        /**
         * Bubble phase.
         * Events are dispatched from child to parent.
         */
        Bubble = 1
    };


    /**
     * Base class for interactive nodes.
     * 
     * Interactive nodes have event handlers, and can accept the input focus.
     */
    class UIInteractiveNode : public UIVisualStateNode, public EventTarget {
    public:
        /**
         * Takes care of resetting the global focused node pointer,
         * if that is contained within the tree specified by this node.
         */
        ~UIInteractiveNode();

        /**
         * Takes care of internally resetting state when a child is removed.
         * @param child child to remove.
         */
        void removeChild(const std::shared_ptr<UINode>& child) override;

            /**
         * Interface for checking if the underlying node is an interactive node.
         * @return always true.
         */
        bool isInteractiveNode() const override {
            return true;
        }

        /**
         * Sets the enabled state.
         * If the node is disabled and it contains the input focus,
         * then the input focus is lost.
         * @param v the new enabled state.
         */
        void setEnabled(bool v) override;

        /**
         * Sets or resets the input focus.
         * If the node receives the input focus, then the event `focus` is fired,
         * followed by a `focusin` event which bubbles up to root.
         * If the node loses the input focus, then the event `blur` is fired, followed
         * by a `focusout` event which bubbles up to root.
         * @param v the new focused state.
         */
        void setFocused(bool v) override;

        /**
         * Returns the currently focused node.
         * @return the currently focused node.
         */
        static UIInteractiveNode* getFocusedNode() {
            return focusedNode;
        }

        /**
         * Returns the closest ancestor node that is an interactive node.
         * @return the closest ancestor node that is an interactive node.
         */
        UIInteractiveNode* getParent() const;

        /**
         * Returns the closest previous sibling node that is an interactive node.
         * @return the closest previous sibling node that is an interactive node.
         */
        UIInteractiveNode* getPrevSibling() const;

        /**
         * Returns the closest next sibling node that is an interactive node.
         * @return the closest next sibling node that is an interactive node.
         */
        UIInteractiveNode* getNextSibling() const;

        /**
         * Returns the first child node that is an interactive node.
         * @return the first child node that is an interactive node.
         */
        UIInteractiveNode* getFirstChild() const;

        /**
         * Returns the last child node that is an interactive node.
         * @return the last child node that is an interactive node.
         */
        UIInteractiveNode* getLastChild() const;

        /**
         * Returns the furthest ancestor node that is an interactive node.
         * @return the furthest ancestor node that is an interactive node.
         */
        UIInteractiveNode* getRoot() const;

        /**
         * Dispatches an event.
         * @param eventName event name.
         * @param event event data.
         * @param phase event phase.
         * @return true if the event was processed, false otherwise.
         */
        virtual bool dispatchEvent(const std::string& eventName, const void* event = nullptr, EventPhase phase = EventPhase::Capture) const {
            return EventTarget::dispatchEvent(eventName, event, (int)phase);
        }

        /**
         * Checks if the given point lies within the node.
         * The comparison is done in screen coordinates, which means the node
         * must have been rendered on the screen at least once.
         * The default implementation checks the coordinates against the node's screen rectangle.
         * @param screenX the horizontal coordinate.
         * @param screenY the vertical coordinate.
         * @return true if the point lies inside the node, false otherwise.
         */
        virtual bool intersects(float screenX, float screenY) const;

    protected:
        void onResetState() override;

        virtual bool onMouseEnter(const ALLEGRO_EVENT& event);
        virtual bool onMouseMove(const ALLEGRO_EVENT& event);
        virtual bool onMouseLeave(const ALLEGRO_EVENT& event);
        virtual bool onMouseButtonDown(const ALLEGRO_EVENT& event);
        virtual bool onMouseButtonHeldDown(const ALLEGRO_EVENT& event);
        virtual bool onMouseButtonUp(const ALLEGRO_EVENT& event);

        virtual bool onMouseWheel(const ALLEGRO_EVENT& event);
        virtual bool onClick(const ALLEGRO_EVENT& event);
        virtual bool onDoubleClick(const ALLEGRO_EVENT& event);

        virtual bool onDragEnter(const ALLEGRO_EVENT& event);
        virtual bool onDrag(const ALLEGRO_EVENT& event);
        virtual bool onDragLeave(const ALLEGRO_EVENT& event);
        virtual bool onDrop(const ALLEGRO_EVENT& event);

        virtual bool onKeyDown(const ALLEGRO_EVENT& event);
        virtual bool onKeyUp(const ALLEGRO_EVENT& event);
        virtual bool onKeyChar(const ALLEGRO_EVENT& event);
        virtual bool onUnusedKeyDown(const ALLEGRO_EVENT& event);
        virtual bool onUnusedKeyUp(const ALLEGRO_EVENT& event);
        virtual bool onUnusedKeyChar(const ALLEGRO_EVENT& event);

        virtual bool onTimer(const ALLEGRO_EVENT& event);

    private:
        static UIInteractiveNode* focusedNode;
        UIInteractiveNode* childWithMouse{ nullptr };
        bool dispatchEventUp(const std::string& eventName, const void* event = nullptr) const;
    };


} //namespace algui


#endif //ALGUI_UIINTERACTIVENODE_HPP
