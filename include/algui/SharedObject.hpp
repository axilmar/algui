#ifndef ALGUI_SHAREDOBJECT_HPP
#define ALGUI_SHAREDOBJECT_HPP


#include <memory>
#include "Object.hpp"


namespace algui {


    /**
     * Base class for objects to be managed via shared pointers.
     * @param T type of derived class.
     */
    template <class T> class SharedObject : public std::enable_shared_from_this<T> {
    public:
        /**
         * Function to get a shared_from_this for a derived type (const version).
         * @return shared pointer to derived type.
         */
        template <class Derived>
        std::shared_ptr<const Derived> sharedFromThis() const {
            return std::static_pointer_cast<const Derived>(this->shared_from_this());
        }

        /**
         * Function to get a shared_from_this for a derived type (mutable version).
         * @return shared pointer to derived type.
         */
        template <class Derived>
        std::shared_ptr<Derived> sharedFromThis() {
            return std::static_pointer_cast<Derived>(this->shared_from_this());
        }
    };


} //namespace algui


#endif //ALGUI_SHAREDOBJECT_HPP
