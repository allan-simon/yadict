#include "ActionId.h"
#include <cppdb/frontend.h>

namespace singletons {

ActionId::ActionId() {
    //TODO replace the hardcodedpath by something in config.js
    cppdb::session sqliteDb("sqlite3:db=../doc/sqlite3.db");
    sqliteDb << "SELECT last_id FROM action_id LIMIT 1;" <<
        cppdb::row >> lastActionId;
}


unsigned int ActionId::get_action_id() {
    unsigned int tempId = lastActionId;
    lastActionId++;
    return tempId;
}


ActionId::~ActionId() {
    cppdb::session sqliteDb("sqlite3:db=../doc/sqlite3.db");
    sqliteDb << "UPDATE action_id SET last_id = ?;" << lastActionId << cppdb::exec;
}

};//end namespace singletons
