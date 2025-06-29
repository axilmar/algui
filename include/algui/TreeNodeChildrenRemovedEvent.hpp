#ifndef ALGUI_TREENODECHILDRENREMOVEDEVENT_HPP
#define ALGUI_TREENODECHILDRENREMOVEDEVENT_HPP


#include "TreeNodeEvent.hpp"


namespace algui {


    /**
     * Children removed event.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class TreeNodeChildrenRemovedEvent : public TreeNodeEvent<T> {
    public:
        /**
         * The constructor.
         * @param target event target.
         */
        TreeNodeChildrenRemovedEvent(const std::shared_ptr<T>& target)
            : TreeNodeEvent<T>("treeNodeChildrenRemoved", target)
        {
        }
    };


} //namespace algui


#endif //ALGUI_TREENODECHILDRENREMOVEDEVENT_HPP
