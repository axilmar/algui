#ifndef ALGUI_TREE_HPP
#define ALGUI_TREE_HPP


namespace algui {


    template <class T> class Tree {
    public:
        Tree() noexcept {}

        Tree(const Tree&) = delete;
        Tree(Tree&&) = delete;

        virtual ~Tree() {
            deleteAll();
        }

        Tree& operator =(const Tree&) = delete;
        Tree& operator =(Tree&&) = delete;

        T* getParent() const noexcept {
            return m_parent;
        }

        T* getPrevSibling() const noexcept {
            return m_prevSibling;
        }

        T* getNextSibling() const noexcept {
            return m_nextSibling;
        }

        T* getFirstChild() const noexcept {
            return m_firstChild;
        }

        T* getLastChild() const noexcept {
            return m_lastChild;
        }

        T* getRoot() const noexcept {
            T* obj = const_cast<T*>(this);
            for(; obj->m_parent; obj = obj->m_parent) {}
            return obj;
        }

        bool contains(const T* obj) const noexcept {
            for (; obj; obj = obj->m_parent) {
                if (obj == this) {
                    return true;
                }
            }
            return false;
        }

        virtual bool add(T* child, T* nextSibling = nullptr) noexcept {
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

        void detach() noexcept {
            if (m_parent) {
                m_parent->remove(this);
            }
        }

        void removeAll() noexcept {
            while (m_lastChild) {
                remove(m_lastChild);
            }
        }

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
