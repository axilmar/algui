#ifndef ALGUI_INTERACTIVEUINODE_HPP
#define ALGUI_INTERACTIVEUINODE_HPP


#include "UINode.hpp"


namespace algui {


    /**
     * Base class for interactive UI nodes.
     */
    class InteractiveUINode : public UINode {
    public:
        /**
         * Returns a pointer to the closest ancestor node that is an interactive UI node.
         * @return a pointer to the closest ancestor node that is an interactive UI node.
         */
        std::shared_ptr<InteractiveUINode> getParent() const;

        /**
         * Returns a raw pointer to the closest ancestor node that is an interactive UI node.
         * @return a raw pointer to the closest ancestor node that is an interactive UI node.
         */
        InteractiveUINode* getParentPtr() const;

        /**
         * Returns a pointer to the first child node that is interactive.
         * @return a pointer to the first child node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getFirstChild() const;

        /**
         * Returns a raw pointer to the first child node that is interactive.
         * @return a raw pointer to the first child node that is interactive.
         */
        InteractiveUINode* getFirstChildPtr() const;

        /**
         * Returns a pointer to the last child node that is interactive.
         * @return a pointer to the last child node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getLastChild() const;

        /**
         * Returns a raw pointer to the last child node that is interactive.
         * @return a raw pointer to the last child node that is interactive.
         */
        InteractiveUINode* getLastChildPtr() const;

        /**
         * Returns a pointer to the first previous sibling node that is interactive.
         * @return a pointer to the first previous sibling node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getPrevSibling() const;

        /**
         * Returns a raw pointer to the first previous sibling node that is interactive.
         * @return a raw pointer to the first previous sibling node that is interactive.
         */
        InteractiveUINode* getPrevSiblingPtr() const;

        /**
         * Returns a pointer to the first next sibling node that is interactive.
         * @return a pointer to the first next sibling node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getNextSibling() const;

        /**
         * Returns a raw pointer to the first next sibling node that is interactive.
         * @return a raw pointer to the first next sibling node that is interactive.
         */
        InteractiveUINode* getNextSiblingPtr() const;

        /**
         * Returns a pointer to the highest ancestor node that is interactive.
         * @return a pointer to the highest ancestor node that is interactive.
         */
        std::shared_ptr<InteractiveUINode> getRoot() const;

        /**
         * Returns a raw pointer to the highest ancestor node that is interactive.
         * @return a raw pointer to the highest ancestor node that is interactive.
         */
        InteractiveUINode* getRootPtr() const;

        /**
         * Checks if this node is enabled.
         * @return true if the node is enabled, false otherwise.
         */
        bool isEnabled() const;

        /**
         * Sets the enabled state.
         * It emits the InteractiveUINodeEnabledChangedEvent event.
         * @param v the new enabled state.
         */
        void setEnabled(bool v);

    private:
        int m_flags{1};
        bool _isEnabledAncestorTree() const;
        static void _setEnabledTree(UINode* node, bool parentEnabledTree);
    };


} //namespace algui


#endif //ALGUI_INTERACTIVEUINODE_HPP
