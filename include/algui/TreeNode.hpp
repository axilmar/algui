#ifndef ALGUI_TREENODE_HPP
#define ALGUI_TREENODE_HPP


#include <stdexcept>
#include <memory>
#include "EventTarget.hpp"
#include "TreeNodeChildAddedEvent.hpp"
#include "TreeNodeChildRemovedEvent.hpp"
#include "TreeNodeChildrenREmovedEvent.hpp"


namespace algui {


    /**
     * Base class for tree nodes.
     * @param T type of derived class.
     */
    template <class T> class TreeNode : public EventTarget {
    public:
        /**
         * Returns a pointer to the parent node.
         * @return a pointer to the parent node.
         */
        std::shared_ptr<T> getParent() const {
            return m_parent ? m_parent->shared_from_this() : nullptr;
        }

        /**
         * Returns a pointer to the previous sibling node.
         * @return a pointer to the previous sibling node.
         */
        const std::shared_ptr<T>& getPrevSibling() const {
            return m_prevSibling;
        }

        /**
         * Returns a pointer to the next sibling node.
         * @return a pointer to the next sibling node.
         */
        const std::shared_ptr<T>& getNextSibling() const {
            return m_nextSibling;
        }

        /**
         * Returns a pointer to the first child node.
         * @return a pointer to the first child node.
         */
        const std::shared_ptr<T>& getFirstChild() const {
            return m_firstChild;
        }

        /**
         * Returns a pointer to the last child node.
         * @return a pointer to the last child node.
         */
        const std::shared_ptr<T>& getLastChild() const {
            return m_lastChild;
        }

        /**
         * Returns a pointer to the root node.
         * @return a pointer to the root node.
         */
        std::shared_ptr<T> getRoot() const {
            T* root = const_cast<T*>(static_cast<const T*>(this));
            for (; root->m_parent; root = root->m_parent) {}
            return root->sharedFromThis<T>();
        }

        /**
         * Checks if the given node is part of this tree.
         * @return true if the given node is part of this tree, false otherwise.
         */
        bool contains(const T* node) const {
            for (; node; node = node->m_parent) {
                if (node == this) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Checks if the given node is part of this tree.
         * @return true if the given node is part of this tree, false otherwise.
         */
        bool contains(const std::shared_ptr<T>& node) const {
            return contains(node.get());
        }

        /**
         * Adds a child node.
         * Emits a `TreeNodeChildAddedEvent`.
         * @param child child to add; must not be null, must not already be a child, must not be an ancestor of this.
         * @param nextSibling optional; if not given, then the child is added as last child, otherwise it is added before this; must be a child of this.
         * @exception std::invalid_argument thrown if the constraints mentioned above are violated.
         */
        virtual void addChild(const std::shared_ptr<T>& child, const std::shared_ptr<T>& nextSibling = nullptr) {
            if (!child) {
                throw std::invalid_argument("TreeNode: addChild: child is null.");
            }

            if (child->m_parent) {
                throw std::invalid_argument("TreeNode: addChild: child already has a parent.");
            }

            if (child->contains(static_cast<T*>(this))) {
                throw std::invalid_argument("TreeNode: addChild: child contains this.");
            }

            if (nextSibling && nextSibling->m_parent != this) {
                throw std::invalid_argument("TreeNode: addChild: nextSibling is not a child of this.");
            }

            std::shared_ptr<T> prevSibling = nextSibling ? nextSibling->m_prevSibling : m_lastChild;

            child->m_parent = static_cast<T*>(this);
            child->m_prevSibling = prevSibling;
            child->m_nextSibling = nextSibling;

            (prevSibling ? prevSibling->m_nextSibling : m_firstChild) = child;
            (nextSibling ? nextSibling->m_prevSibling : m_lastChild) = child;

            dispatchEvent(TreeNodeChildAddedEvent(sharedFromThis<T>(), child));
        }

        /**
         * Removes a child node.
         * Emits a `TreeNodeChildRemovedEvent`.
         * @param child child to remove; must not be null, must be a child of this.
         * @exception std::invalid_argument thrown if the constraints mentioned above are violated.
         */
        virtual void removeChild(const std::shared_ptr<T>& child) {
            if (!child) {
                throw std::invalid_argument("TreeNode: removeChild: child is null.");
            }

            if (child->m_parent != this) {
                throw std::invalid_argument("TreeNode: removeChild: not a child.");
            }

            remove(child);

            dispatchEvent(TreeNodeChildRemovedEvent(sharedFromThis<T>(), child));
        }

        /**
         * Removes all children.
         * Emits a `TreeNodeChildrenRemovedEvent`.
         */
        virtual void removeChildren() {
            while (m_lastChild) {
                remove(m_lastChild);
            }
            dispatchEvent(TreeNodeChildrenRemovedEvent(sharedFromThis<T>()));
        }

    private:
        T* m_parent{ nullptr };
        std::shared_ptr<T> m_prevSibling;
        std::shared_ptr<T> m_nextSibling;
        std::shared_ptr<T> m_firstChild;
        std::shared_ptr<T> m_lastChild;

        void remove(const std::shared_ptr<T>& child) {
            (child->m_prevSibling ? child->m_prevSibling->m_nextSibling : m_firstChild) = child->m_nextSibling;
            (child->m_nextSibling ? child->m_nextSibling->m_prevSibling : m_lastChild) = child->m_prevSibling;

            child->m_parent = nullptr;
            child->m_prevSibling.reset();
            child->m_nextSibling.reset();
        }
    };


} //namespace algui


#endif //ALGUI_TREENODE_HPP
