#ifndef ALGUI_TREENODECHILDADDEDEVENT_HPP
#define ALGUI_TREENODECHILDADDEDEVENT_HPP


#include "TreeNodeChildEvent.hpp"


namespace algui {


    /**
     * Child added event.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class TreeNodeChildAddedEvent : public TreeNodeChildEvent<T> {
    public:
        /**
         * The constructor.
         * @param target event target.
         * @param child the child.
         */
        TreeNodeChildAddedEvent(const std::shared_ptr<T>& target, const std::shared_ptr<T>& child)
            : TreeNodeChildEvent<T>("treeNodeChildAdded", target, child)
        {
        }
    };


} //namespace algui


#endif //ALGUI_TREENODECHILDADDEDEVENT_HPP
