#ifndef ALGUI_PROPERTYCONTAINER_HPP
#define ALGUI_PROPERTYCONTAINER_HPP


#include <string>
#include <map>
#include <any>
#include <stdexcept>


namespace algui {


    /**
     * An object that allows the specification of properties at runtime.
     */
    class PropertyContainer {
    public:
        /**
         * Retrieves a property.
         * @param name property name.
         * @param defaultValue value to return in case the object does not have the property.
         * @exception std::bad_any_cast thrown if the property type does not match the type of value stored internally.
         */
        template <class T> T getProperty(const std::string& name, const T& defaultValue = T()) const {
            const auto it = m_properties.find(name);
            if (it != m_properties.end()) {
                return std::any_cast<T>(it->second);
            }
            return defaultValue;
        }

        /**
         * Sets a property.
         * @param name property name.
         * @param value property value.
         */
        template<class T> void setProperty(const std::string& name, const T& value) {
            m_properties[name] = value;
        }

    private:
        std::map<std::string, std::any> m_properties;
    };


} //namespace algui


#endif //ALGUI_PROPERTYCONTAINER_HPP
