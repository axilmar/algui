#ifndef ALGUI_TREENODE_HPP
#define ALGUI_TREENODE_HPP


#include <memory>
#include <stdexcept>
#include "Object.hpp"


namespace algui {


    /**
     * Base class for tree nodes.
     * @param T type of derived class.
     */
    template <class T> class TreeNode : public Object, public std::enable_shared_from_this<T> {
    public:
        /**
         * Removes all children, starting from last child.
         */
        virtual ~TreeNode() {
            while (m_lastChild) {
                removeChild(m_lastChild);
            }
        }

        /**
         * Returns the parent node.
         * @return the parent node, or a null pointer if this does not have a parent.
         */
        std::shared_ptr<T> getParent() const {
            return m_parent ? std::static_pointer_cast<T>(m_parent->shared_from_this()) : nullptr;
        }

        /**
         * Returns pointer to parent without conversion to std::shared_ptr.
         * @return pointer to parent.
         */
        T* getParentPtr() const {
            return m_parent;
        }

        /**
         * Returns the previous sibling node.
         * @return the previous sibling node.
         */
        const std::shared_ptr<T>& getPrevSibling() const {
            return m_prevSibling;
        }

        /**
         * Returns the next sibling node.
         * @return the next sibling node.
         */
        const std::shared_ptr<T>& getNextSibling() const {
            return m_nextSibling;
        }

        /**
         * Returns the first child node.
         * @return the first child node.
         */
        const std::shared_ptr<T>& getFirstChild() const {
            return m_firstChild;
        }

        /**
         * Returns the last child node.
         * @return the last child node.
         */
        const std::shared_ptr<T>& getLastChild() const {
            return m_lastChild;
        }

        /**
         * Returns the root node of this tree.
         * @return the root node of this tree.
         */
        std::shared_ptr<T> getRoot() const {
            T* node = const_cast<T*>(static_cast<const T*>(this));
            for (; node->m_parent; node = node->m_parent) {}
            return std::static_pointer_cast<T>(node->shared_from_this());
        }

        /**
         * Checks if the given node is part of this tree.
         * @param node to check if part of this tree.
         * @return true if the given node is part of this tree (including this node),
         *  false otherwise.
         */
        bool contains(T* node) const {
            if (node) {
                do {
                    if (node == this) {
                        return true;
                    }
                    node = node->m_parent;
                } while (node);
            }
            return false;
        }

        /**
         * Checks if the given node is part of this tree.
         * @param node to check if part of this tree.
         * @return true if the given node is part of this tree (including this node),
         *  false otherwise.
         */
        bool contains(const std::shared_ptr<T>& node) const {
            return contains(node.get());
        }

        /**
         * Adds a child node.
         * @param child child node to add.
         * @param nextSibling the next sibling; if null, the child is added after the last child.
         * @exception std::invalid_argument thrown if child is null or belongs to another tree,
         *  or if nextSibling is not null and not a child of this.
         */
        virtual void addChild(const std::shared_ptr<T>& child, const std::shared_ptr<T>& nextSibling = nullptr) {
            if (!child || child->m_parent || contains(child)) {
                throw std::invalid_argument("TreeNode: addChild: invalid child.");
            }

            if (nextSibling && nextSibling->m_parent != this) {
                throw std::invalid_argument("TreeNode: addChild: invalid next sibling.");
            }

            child->m_parent = static_cast<T*>(this);

            if (nextSibling) {
                child->m_prevSibling = nextSibling->m_prevSibling;
                child->m_nextSibling = nextSibling;
                nextSibling->m_prevSibling->m_nextSibling = child;
                nextSibling->m_prevSibling = child;
            }

            else if (m_lastChild) {
                child->m_prevSibling = m_lastChild;
                m_lastChild->m_nextSibling = child;
                m_lastChild = child;
            }

            else {
                m_firstChild = child;
                m_lastChild = child;
            }
        }

        /**
         * Removes a child node.
         * @param child child node to remove.
         * @exception std::invalid_argument thrown if child is null or if not a child of this node.
         */
        virtual void removeChild(const std::shared_ptr<T>& child) {
            if (!child || child->m_parent != this) {
                throw std::invalid_argument("TreeNode: removeChild: invalid child.");
            }

            child->m_parent = nullptr;

            if (child->m_prevSibling) {
                child->m_prevSibling->m_nextSibling = child->m_nextSibling;
            }
            else {
                m_firstChild = child->m_nextSibling;
            }

            if (child->m_nextSibling) {
                child->m_nextSibling->m_prevSibling = child->m_prevSibling;
            }
            else {
                m_lastChild = child->m_prevSibling;
            }
        }

        /**
         * Removes this node from its parent, if its parent is not null.
         */
        void detach() {
            if (m_parent) {
                m_parent->removeChild(std::static_pointer_cast<T>(this->shared_from_this()));
            }
        }

    private:
        T* m_parent{ nullptr };
        std::shared_ptr<T> m_prevSibling;
        std::shared_ptr<T> m_nextSibling;
        std::shared_ptr<T> m_firstChild;
        std::shared_ptr<T> m_lastChild;
    };


} //namespace algui


#endif //ALGUI_TREENODE_HPP
