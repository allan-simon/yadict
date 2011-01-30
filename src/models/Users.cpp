#include <iostream>
#include <sstream>
#include <vector>
#include <cppcms/crypto.h>
#include <cppdb/frontend.h>
#include "models/Users.h"

using namespace cppcms::crypto;
namespace models {


/**
 *
 */
Users::Users(cppdb::session sqliteDb) : SqliteModel(sqliteDb) {
    // TODO ADD check for the username 
    check_passwd_state = sqliteDb.create_prepared_statement(
        "SELECT 1 FROM users "
        "WHERE username = ? AND password = ?"
    );
    add_user_state = sqliteDb.create_prepared_statement(
        "INSERT INTO users(username, password, email)"
        "VALUES(?,?,?)"
    );
    get_all_users_state = sqliteDb.create_prepared_statement(
        "SELECT * FROM users"
    );
}

/**
 *
 */
bool Users::is_login_correct(
    std::string login,
    std::string pass
) {
    std::auto_ptr<message_digest> d(message_digest::md5());
    
    char buf[16];
    d->append(pass.c_str(), pass.size());
    d->readout(buf);

    std::stringstream in;
    in.write(buf, 16);
    

    check_passwd_state.bind(login);
    check_passwd_state.bind(in);
    cppdb::result res = check_passwd_state.row();
   
   
    int checkresult = 0;
    res.fetch(0,checkresult);

    // Don't forget to reset statement
    check_passwd_state.reset();

    if (checkresult == 1 ) {
        return true;
    }
    return false;
}

/**
 *
 */
bool Users::add_user(
    std::string login,
    std::string pass,
    std::string email
) {

    std::auto_ptr<message_digest> d(message_digest::md5());

    char buf[16];
    d->append(pass.c_str(), pass.size());
    d->readout(buf);

    std::stringstream binaryHash;
    binaryHash.write(buf, 16);
    
    add_user_state.bind(login);
    add_user_state.bind(binaryHash);
    add_user_state.bind(email);

    try {
        add_user_state.exec();    
    } catch (cppdb::cppdb_error const &e) {

        return false;
    }
    add_user_state.reset();
    return true;
}

/**
 *
 */
ListOfUsers Users::get_all_users() {
    cppdb::result res = get_all_users_state.query();

    ListOfUsers listOfUsers;

    while (res.next()) {
        UserResult user;
        user.id = res.get<int>("id");
        user.username = res.get<std::string>("username");
        user.email = res.get<std::string>("email");
        std::tm sinceTime = res.get<std::tm>("since");
        user.since = asctime(&sinceTime);
        listOfUsers.push_back(user);
    }

    get_all_users_state.reset();

    return listOfUsers;
}


}
