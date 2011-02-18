#ifndef YADICT_GENERICS_ACTIONID_H
#define YADICT_GENERICS_ACTIONID_H

#include "Singleton.h"

namespace singletons {
class ActionId : public Singleton<ActionId> {
    friend class Singleton<ActionId>;
    private:
        ActionId();
        unsigned int lastActionId;
    public:
        ~ActionId();
        unsigned int get_action_id();


};

}; //end namespace singletons
#endif
