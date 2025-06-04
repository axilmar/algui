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
         * Pointers to other members of the tree are initialized to null.
         */
        Tree() {}

        Tree(const Tree&) = delete;
        Tree(Tree&&) = delete;
        
        /**
         * The destructor.
         * It deletes all children nodes.
         */
        virtual ~Tree() {
            deleteAll();
        }
        
        Tree& operator =(const Tree&) = delete;
        Tree& operator =(Tree&&) = delete;
        
        /**
         * Returns the parent.
         * @return the parent.
         */
        T* getParent() const {
            return m_parent;
        }

        /**
         * Returns the previous sibling.
         * @return the previous sibling.
         */
        T* getPrevSibling() const {
            return m_prevSibling;
        }

        /**
         * Returns the next sibling.
         * @return the next sibling.
         */
        T* getNextSibling() const {
            return m_nextSibling;
        }

        /**
         * Returns the first child.
         * @return the first child.
         */
        T* getFirstChild() const {
            return m_firstChild;
        }

        /**
         * Returns the last child.
         * @return the last child.
         */
        T* getLastChild() const {
            return m_lastChild;
        }

        /**
         * Returns the innermost last child.
         * @return the innermost last child.
         */
        T* getInnermostLastChild() const {
            T* node = const_cast<T*>(static_cast<const T*>(this));
            for (; node->m_lastChild; node = node->m_lastChild) {};
            return node;
        }

        /**
         * Returns the previous object, as if the tree was a flat list.
         * @return the previous object, as if the tree was a flat list.
         */
        T* getPrev() const {
            if (m_prevSibling) {
                return m_prevSibling->getInnermostLastChild();
            }
            return m_parent;
        }

        /**
         * Returns the next object, as if the tree was a flat list.
         * @return the next object, as if the tree was a flat list.
         */
        T* getNext() const {
            if (m_firstChild) {
                return m_firstChild;
            }
            for (const T* node = static_cast<const T*>(this);;) {
                if (node->m_nextSibling) {
                    return node->m_nextSibling;
                }
                node = node->m_parent;
                if (!node) {
                    break;
                }
            }
            return nullptr;
        }

        /**
         * Returns the previous of the given node within this tree.
         * @param node node to get the previous of.
         * @return the previous of the given node within this tree.
         */
        T* getPrev(const T* node) const {
            if (containsDescentant(node)) {
                if (node->m_prevSibling) {
                    return node->m_prevSibling->getInnermostLastChild();
                }
                return node->m_parent != this ? node->m_parent : nullptr;
            }
            return nullptr;
        }

        /**
         * Returns the next of the given node within this tree.
         * @param node node to get the next of.
         * @return the next of the given node within this tree.
         */
        T* getNext(const T* node) const {
            if (contains(node)) {
                if (node->m_firstChild) {
                    return node->m_firstChild;
                }
                for (;;) {
                    if (node->m_nextSibling) {
                        return node->m_nextSibling;
                    }
                    node = node->m_parent;
                    if (node == this) {
                        break;
                    }
                }
            }
            return nullptr;
        }

        /**
         * Returns the root.
         * @return the root.
         */
        T* getRoot() const {
            T* node = const_cast<T*>(static_cast<const T*>(this));
            for(; node->m_parent; node = node->m_parent) {}
            return node;
        }

        /**
         * Checks if a node is in this tree.
         * @param node node to check if it belongs to this tree.
         * @return true if the given node belongs in this tree, false otherwise.
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
         * Checks if a node is in of the child trees.
         * @param node node to check if it belongs to this tree.
         * @return true if the given node belongs in this tree, false otherwise.
         */
        bool containsDescentant(const T* node) const {
            if (node != this) {
                return contains(node);
            }
            return false;
        }

        /**
         * Checks if this node is root.
         * @return true if this node is root, i.e. it does not have a parent, false otherwise.
         */
        bool isRoot() const {
            return m_parent == nullptr;
        }

        /**
         * Checks if this node is a child node.
         * @return true if this node is a child node, i.e. it has a parent, false otherwise.
         */
        bool isChild() const {
            return m_parent != nullptr;
        }

        /**
         * Returns the node's depth.
         * The root node is at depth 0.
         * @return the node's depth.
         */
        size_t getDepth() const {
            size_t result = 0;
            for (T* node = m_parent; node; node = node->m_parent, ++result) {}
            return result;
        }

        /**
         * Executes a function for each child.
         * The children are traversed from first to last.
         * @param func function to execute for each child.
         */
        template <class F> void forEach(const F& func) const {
            for (T* child = m_firstChild; child; child = child->m_nextSibling) {
                func(child);
            }
        }

        /**
         * Executes a function for each child.
         * The children are traversed from last to first.
         * @param func function to execute for each child.
         */
        template <class F> void forEachRev(const F& func) const {
            for (T* child = m_lastChild; child; child = child->m_prevSibling) {
                func(child);
            }
        }

        /**
         * Adds a child.
         * @param child to add.
         * @param nextSibling the next sibling of the child; if null, the child is added as the last child.
         * @return true on success, false if the child is already a child of another node, or an ancesctor of this,
         *  or the next sibling is not a child of this.
         */
        virtual bool add(T* child, T* nextSibling = nullptr) {
            if (child->m_parent || child->contains(static_cast<T*>(this)) || (nextSibling && nextSibling->m_parent != this)) {
                return false;
            }

            child->m_parent = static_cast<T*>(this);

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
         * Removes a child node.
         * @param child the child to remove.
         * @return true on success, false if the child is not a child of this node.
         */
        virtual bool remove(T* child) {
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
         * Removes this node from its parent, if there is one. 
         */
        void detach() {
            if (m_parent) {
                m_parent->remove(this);
            }
        }

        /**
         * Removes all children nodes. 
         */
        void removeAll() {
            while (m_lastChild) {
                remove(m_lastChild);
            }
        }

        /**
         * Removes and deletes all children nodes. 
         */
        void deleteAll() {
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
