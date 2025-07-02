#ifndef ALGUI_CHILDREMOVEDEVENT_HPP
#define ALGUI_CHILDREMOVEDEVENT_HPP


#include "ChildEvent.hpp"


namespace algui {


    /**
     * Child removed event.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class ChildRemovedEvent : public ChildEvent<T> {
    public:
        /**
         * The constructor.
         * @param target event target.
         * @param child the child.
         */
        ChildRemovedEvent(std::shared_ptr<T>&& target, const std::shared_ptr<T>& child)
            : ChildEvent<T>("childRemoved", std::move(target), child)
        {
        }
    };


} //namespace algui


#endif //ALGUI_CHILDREMOVEDEVENT_HPP
