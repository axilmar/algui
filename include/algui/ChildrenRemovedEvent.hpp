#ifndef ALGUI_CHILDRENREMOVEDEVENT_HPP
#define ALGUI_CHILDRENREMOVEDEVENT_HPP


#include "TreeNodeEvent.hpp"


namespace algui {


    /**
     * Children removed event.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class ChildrenRemovedEvent : public TreeNodeEvent<T> {
    public:
        /**
         * The constructor.
         * @param target event target.
         */
        ChildrenRemovedEvent(std::shared_ptr<T>&& target)
            : TreeNodeEvent<T>("childrenRemoved", std::move(target))
        {
        }
    };


} //namespace algui


#endif //ALGUI_CHILDRENREMOVEDEVENT_HPP
