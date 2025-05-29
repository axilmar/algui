#ifndef ALGUI_SHAREDRESOURCE_HPP
#define ALGUI_SHAREDRESOURCE_HPP


#include <set>
#include "SharedObject.hpp"


namespace algui {


    template <class Resource, class Owner> 
    class SharedResource : public SharedObject<SharedResource<Resource, Owner>> {
    public:
        void addOwner(Owner* owner) {
            m_owners.insert(owner);
        }

        void removeOwner(Owner* owner) {
            m_owners.erase(owner);
        }

        void setChanged() const {
            for (Owner* owner : m_owners) {
                owner->onResourceChanged(*static_cast<const Resource*>(this));
            }
        }

    private:
        std::set<Owner*> m_owners;
    };


} //namespace algui


#endif //ALGUI_SHAREDRESOURCE_HPP
