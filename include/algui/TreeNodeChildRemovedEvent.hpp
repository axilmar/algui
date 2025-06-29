#ifndef ALGUI_TREENODECHILDREMOVEDEVENT_HPP
#define ALGUI_TREENODECHILDREMOVEDEVENT_HPP


#include "TreeNodeChildEvent.hpp"


namespace algui {


    /**
     * Child removed event.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class TreeNodeChildRemovedEvent : public TreeNodeChildEvent<T> {
    public:
        /**
         * The constructor.
         * @param target event target.
         * @param child the child.
         */
        TreeNodeChildRemovedEvent(const std::shared_ptr<T>& target, const std::shared_ptr<T>& child)
            : TreeNodeChildEvent<T>("treeNodeChildRemoved", target, child)
        {
        }
    };


} //namespace algui


#endif //ALGUI_TREENODECHILDREMOVEDEVENT_HPP
