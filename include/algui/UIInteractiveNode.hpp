#ifndef ALGUI_UIINTERACTIVENODE_HPP
#define ALGUI_UIINTERACTIVENODE_HPP


#include "UIVisualStateNode.hpp"
#include "EventTarget.hpp"
#include "allegro5/allegro.h"


namespace algui {


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
         * Removes all children.
         */
        ~UIInteractiveNode();

        /**
         * Adds a child node.
         * It fires the childAdded event.
         * @param child child node to add.
         * @param nextSibling the next sibling; if null, the child is added after the last child.
         * @exception std::invalid_argument thrown if child is null or belongs to another tree,
         *  or if nextSibling is not null and not a child of this.
         */
        void addChild(const std::shared_ptr<UINode>& child, const std::shared_ptr<UINode>& nextSibling = nullptr) override;
        /**
         * Removes a child.
         * It fires the childRemoved event.
         * Resets the child's internal mouse state, if the child had the mouse.
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
         * It fires an 'enabled' or 'disabled' event.
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
         * @return true if the focus was given to this node, otherwise false.
         */
        virtual bool setFocused(bool v) override;

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
         * Checks if the given point lies within the node.
         * The comparison is done in screen coordinates, which means the node
         * must have been rendered on the screen at least once.
         * The default implementation checks the coordinates against the node's screen rectangle.
         * @param screenX the horizontal coordinate.
         * @param screenY the vertical coordinate.
         * @return true if the point lies inside the node, false otherwise.
         */
        virtual bool intersects(float screenX, float screenY) const;

        /**
         * Returns the child under the given coordinates.
         * The child must be visible in order to be tested for intersection with the coordinates,
         * using the method `intersects(screenX, screenY)`.
         * @param screenX the horizontal coordinate.
         * @param screenY the vertical coordinate.
         * @return child under the given coordinates, null otherwise.
         */
        UIInteractiveNode* getChildFromPoint(float screenX, float screenY) const;

    private:
        static UIInteractiveNode* focusedNode;
        UIInteractiveNode* m_childWithMouse{ nullptr };
        bool m_hasMouse{ false };

        void resetFocus();
        void resetMouseState();
        bool dispatchEventUp(const std::string& eventName, const Event& event) const;

        bool doMouseEnter(const ALLEGRO_EVENT& event);
        bool doMouseMove(const ALLEGRO_EVENT& event);
        bool doMouseLeave(const ALLEGRO_EVENT& event);
        bool doMouseButtonDown(const ALLEGRO_EVENT& event);
        bool doMouseButtonHeldDown(const ALLEGRO_EVENT& event);
        bool doMouseButtonUp(const ALLEGRO_EVENT& event);

        bool doMouseWheel(const ALLEGRO_EVENT& event);
        bool doClick(const ALLEGRO_EVENT& event);
        bool doDoubleClick(const ALLEGRO_EVENT& event);

        bool doDragEnter(const ALLEGRO_EVENT& event);
        bool doDrag(const ALLEGRO_EVENT& event);
        bool doDragLeave(const ALLEGRO_EVENT& event);
        bool doDrop(const ALLEGRO_EVENT& event);

        bool doKeyDown(const ALLEGRO_EVENT& event);
        bool doKeyUp(const ALLEGRO_EVENT& event);
        bool doKeyChar(const ALLEGRO_EVENT& event);
        bool doUnusedKeyDown(const ALLEGRO_EVENT& event);
        bool doUnusedKeyUp(const ALLEGRO_EVENT& event);
        bool doUnusedKeyChar(const ALLEGRO_EVENT& event);

        bool doTimer(const ALLEGRO_EVENT& event);

    };


} //namespace algui


#endif //ALGUI_UIINTERACTIVENODE_HPP
