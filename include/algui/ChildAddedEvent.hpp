#ifndef ALGUI_CHILDADDEDEVENT_HPP
#define ALGUI_CHILDADDEDEVENT_HPP


#include "ChildEvent.hpp"


namespace algui {


    /**
     * Child added event.
     * @param T derived class for TreeNode<T>.
     */
    template <class T> class ChildAddedEvent : public ChildEvent<T> {
    public:
        /**
         * The constructor.
         * @param target event target.
         * @param child the child.
         */
        ChildAddedEvent(std::shared_ptr<T>&& target, const std::shared_ptr<T>& child)
            : ChildEvent<T>("childAdded", std::move(target), child)
        {
        }
    };


} //namespace algui


#endif //ALGUI_CHILDADDEDEVENT_HPP
