#include "Controller.h"
#include "controllers/Logs.h"
#include "contents/logs.h"

namespace controllers {

Logs::Logs(cppcms::service &serv) :
    Controller(serv),
    logsModel(cppdb::session("sqlite3:db=../doc/sqlite3.db"))
{
    // TODO have /  and /show directing to some "index" page 
    // as cburgmer pointed out, some people do navigate by crafting url
    cppcms::url_dispatcher* disp = &dispatcher();

  	disp->assign("/show-last-words$", &Logs::show_last_words, this);
  	disp->assign("/show-last-metas$", &Logs::show_last_metas, this);
  	//disp->assign("/revert-action/(\\d+)$", &Logs::revert_action, this, 1);

}


/**
 *
 */
void Logs::show_last_words() {
    contents::Logs c;
    init_content(c);
    c.wordsLogs = logsModel.get_last_word_actions();

    render("logs_show_last",c);
}


/**
 *
 */
void Logs::show_last_metas() {
    contents::LogsShowLastMetas c;
    init_content(c);
    c.metasLogs = logsModel.get_last_meta_actions();

    render("logs_show_last_metas",c);
}



}; // end namespace
