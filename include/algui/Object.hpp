#ifndef ALGUI_OBJECT_HPP
#define ALGUI_OBJECT_HPP


#include <string>


namespace algui {


    /**
     * Base class for objects.
     * 
     * This class is used as a basis for objects that cannot be copied or moved,
     * because they contain resources.
     * 
     * It provides a property `id`, which can be used for debugging purposes.
     */
    class Object {
    public:
        /**
         * The default constructor.
         */
        Object() {
        }

        Object(const Object&) = delete;
        Object(Object&&) = delete;

        /**
         * The destructor.
         * 
         * The destructor is virtual, due to the purpose of this class being inheritance.
         */
        virtual ~Object() {
        }

        Object& operator = (const Object&) = delete;
        Object& operator = (Object&&) = delete;

        /**
         * Returns the id of this object.
         * @return the id of this object; an empty string by default.
         */
        const std::string& getId() const {
            return m_id;
        }

        /**
         * Sets the id of this object.
         * @param id the new id of this object.
         */
        void setId(const std::string& id) {
            m_id = id;
        }

    private:
        //the object id
        std::string m_id;
    };


} //namespace algui


#endif //ALGUI_OBJECT_HPP
