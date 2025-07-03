#ifndef ALGUI_SHAREDOBJECT_HPP
#define ALGUI_SHAREDOBJECT_HPP


#include <memory>


namespace algui {


    /**
     * Base class for shared objects.
     */
    class SharedObject : public std::enable_shared_from_this<SharedObject> {
    public:
        /**
         * The default constructor.
         */
        SharedObject() {
        }

        /**
         * The copy constructor.
         * Deleted because the object is meant to be shared.
         */
        SharedObject(const SharedObject&) = delete;

        /**
         * The move constructor.
         * Deleted because the object is meant to be shared.
         */
        SharedObject(SharedObject&&) = delete;

        /**
         * The destructor.
         * Virtual due to polymorphism.
         */
        virtual ~SharedObject() {
        }

        /**
         * The copy assignment operator.
         * Deleted because the object is meant to be shared.
         */
        SharedObject& operator = (const SharedObject&) = delete;

        /**
         * The move assignment operator.
         * Deleted because the object is meant to be shared.
         */
        SharedObject& operator = (SharedObject&&) = delete;

        /**
         * Returns a shared ptr to const T.
         * @return a shared ptr to const T.
         */
        template <class T> std::shared_ptr<const T> sharedFromThis() const {
            return std::dynamic_pointer_cast<const T>(this->shared_from_this());
        }

        /**
         * Returns a shared ptr to T.
         * @return a shared ptr to T.
         */
        template <class T> std::shared_ptr<T> sharedFromThis() {
            return std::dynamic_pointer_cast<T>(this->shared_from_this());
        }

        /**
         * Converts this to T.
         * @return conversion of this to T or nullptr if the conversion is not valid.
         */
        template <class T> const T* as() const {
            return dynamic_cast<const T*>(this);
        }

        /**
         * Converts this to T.
         * @return conversion of this to T or nullptr if the conversion is not valid.
         */
        template <class T> T* as() {
            return dynamic_cast<T*>(this);
        }
    };


} //namespace algui


#endif //ALGUI_SHAREDOBJECT_HPP
