#ifndef ALGUI_CHILDEVENT_HPP
#define ALGUI_CHILDEVENT_HPP


#include "TreeNodeEvent.hpp"


namespace algui {


    /**
     * Base class for tree node child events.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class ChildEvent : public TreeNodeEvent<T> {
    public:
        /**
         * Returns the affected child.
         * @return the affected child.
         */
        const std::shared_ptr<T>& getChild() const {
            return m_child;
        }

    protected:
        /**
         * The constructor.
         * @param type event type.
         * @param target event target.
         * @param child the child.
         */
        ChildEvent(std::string&& type, std::shared_ptr<T>&& target, const std::shared_ptr<T>& child)
            : TreeNodeEvent<T>(std::move(type), std::move(target))
            , m_child(child)
        {
        }

    private:
        std::shared_ptr<T> m_child;
    };


} //namespace algui


#endif //ALGUI_CHILDEVENT_HPP
