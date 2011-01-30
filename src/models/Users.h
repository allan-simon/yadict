#ifndef MODELS_USERS_H
#define MODELS_USERS_H

#include "models/SqliteModel.h"
#include <iostream>
#include <vector>
#include <booster/locale/date_time.h>

namespace models {

    struct UserResult {
        int id;
        std::string username;
        std::string email;
        std::string lang;
        std::string since;
        int last_time_active;
        bool send_notifications;
        std::string name;
        int birthday;
        std::string description;
        std::string homepage;
        std::string image;
        std::string country_id;
        bool is_public;
    };
    typedef std::vector<UserResult> ListOfUsers;

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

class Users : public SqliteModel {
    private:
        cppdb::statement check_passwd_state; 
        cppdb::statement add_user_state;
        cppdb::statement get_all_users_state;

    public:
        Users(cppdb::session sqliteDb);

        bool is_login_correct(
            std::string login,
            std::string pass
        );

        bool add_user(
            std::string login,
            std::string pass,
            std::string email
        );
        std::vector<UserResult> get_all_users();
};

}

#endif
