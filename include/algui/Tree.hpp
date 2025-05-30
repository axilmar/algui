#ifndef ALGUI_TREE_HPP
#define ALGUI_TREE_HPP


namespace algui {


    /**
     * Base class for trees.
     * @param T type of derived class.
     */
    template <class T> class Tree {
    public:
        /**
         * The default constructor.
         */
        Tree() noexcept {}

        Tree(const Tree&) = delete;
        Tree(Tree&&) = delete;

        /**
         * The destructor.
         * All children nodes are deleted.
         */
        virtual ~Tree() {
            deleteAll();
        }

        Tree& operator =(const Tree&) = delete;
        Tree& operator =(Tree&&) = delete;

        /**
         * Returns the parent node.
         * @return the parent node.
         */
        T* getParent() const noexcept {
            return m_parent;
        }

        /**
         * Returns the previous sibling node.
         * @return the previous sibling node.
         */
        T* getPrevSibling() const noexcept {
            return m_prevSibling;
        }

        /**
         * Returns the next sibling node.
         * @return the next sibling node.
         */
        T* getNextSibling() const noexcept {
            return m_nextSibling;
        }

        /**
         * Returns the first child node.
         * @return the first child node.
         */
        T* getFirstChild() const noexcept {
            return m_firstChild;
        }

        /**
         * Returns the last child node.
         * @return the last child node.
         */
        T* getLastChild() const noexcept {
            return m_lastChild;
        }

        /**
         * Returns the root node.
         * @return the root node.
         */
        T* getRoot() const noexcept {
            T* obj = const_cast<T*>(this);
            for(; obj->m_parent; obj = obj->m_parent) {}
            return obj;
        }

        /**
         * Checks if the given object is part of this tree.
         * @param obj object to check if part of this tree.
         * @return true if the given object is part of this tree, false otherwise.
         */
        bool contains(const T* obj) const noexcept {
            for (; obj; obj = obj->m_parent) {
                if (obj == this) {
                    return true;
                }
            }
            return false;
        }

        /**
         * Adds a child.
         * @param child child to add.
         * @param nextSibling if given, then the child is added before the next sibling, otherwise the child is added as the last child.
         * @return true if the child was added, false if the child belongs to another parent or is ancestor of this or the next sibling is not a child of this.
         */
        virtual bool add(T* child, T* nextSibling = nullptr) noexcept {
            //check params
            if (child->m_parent || child->contains(static_cast<T*>(this)) || (nextSibling && nextSibling->m_parent != this)) {
                return false;
            }

            //set the parent
            child->m_parent = static_cast<T*>(this);

            //link with next sibling
            if (nextSibling) {
                child->m_nextSibling = nextSibling;
                if (nextSibling->m_prevSibling) {
                    child->m_prevSibling = nextSibling->m_prevSibling;
                    nextSibling->m_prevSibling->m_nextSibling = child;
                }
                else {
                    m_firstChild = child;
                }
                nextSibling->m_prevSibling = child;
            }

            //else link as last child
            else {
                if (m_lastChild) {
                    child->m_prevSibling = m_lastChild;
                    m_lastChild->m_nextSibling = child;
                }
                else {
                    m_firstChild = child;
                }
                m_lastChild = child;
            }

            return true;
        }

        /**
         * Removes a child.
         * @param child child to remove.
         * @return true if the child was removed, false if it is not a child of this.
         */
        virtual bool remove(T* child) noexcept {
            if (child->m_parent != this) {
                return false;
            }

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

            child->m_parent = child->m_prevSibling = child->m_nextSibling = nullptr;

            return true;
        }

        /**
         * Removes this node from its parent, if it is a child node.
         */
        void detach() noexcept {
            if (m_parent) {
                m_parent->remove(this);
            }
        }

        /**
         * Removes all children.
         */
        void removeAll() noexcept {
            while (m_lastChild) {
                remove(m_lastChild);
            }
        }

        /**
         * Deletes all children.
         */
        void deleteAll() noexcept {
            while (T* child = m_lastChild) {
                remove(child);
                delete child;
            }
        }

    private:
        T* m_parent{ nullptr };
        T* m_prevSibling{ nullptr };
        T* m_nextSibling{ nullptr };
        T* m_firstChild{ nullptr };
        T* m_lastChild{ nullptr };
    };


} //namespace algui


#endif //ALGUI_TREE_HPP
