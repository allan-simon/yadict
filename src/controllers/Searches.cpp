#include "Controller.h"
#include "controllers/Searches.h"
#include "shanghainesedict.h"
#include "contents/searches.h"

#include "models/TatoHyperDB.h"

#include <cppcms/filters.h>

namespace controllers {
Searches::Searches(cppcms::service &serv) : Controller(serv) {
    // TODO have /  and /show directing to some "index" page 
    // as cburgmer pointed out, some people do navigate by crafting url
    cppcms::url_dispatcher* disp = &dispatcher();

  	disp->assign("/simple-treat$", &Searches::simple_treat, this);
  	disp->assign("/show-result/(.+)/([a-zA-Z]+)$", &Searches::show_result, this,1 ,2);
  	disp->assign("/show-result/(.+)/(\\w+)/(\\d+)/(\\d+)$", &Searches::show_result, this,1 ,2, 3, 4);

}

void Searches::simple_treat() {
	contents::SearchesSimple c;
    init_content(c);
    c.searchesSimple.load(context());
    
    if (!c.searchesSimple.validate()) {
        go_back_to_previous_page();
        return;
    }



    std::ostringstream oss;
    oss << cppcms::filters::urlencode(c.searchesSimple.query.value());
    response().set_redirect_header(
        "/" + c.lang +"/searches/show-result/" +
        oss.str()
         + "/" +
        c.searchesSimple.wordLang.selected_id()
    );

}

void Searches::show_result(std::string query, std::string lang) {
    show_result(query, lang, "1", "10");
}

void Searches::show_result (
        std::string query,
        std::string lang,
        std::string offsetStr,
        std::string sizeStr
) {
    unsigned int size = atoi(sizeStr.c_str());
    unsigned int offset = atoi(offsetStr.c_str()) - 1;

	contents::SearchesSimple c;
	contents::WordsHelper whc;
    init_content(c);
    // TODO filter this as otherwise it will produce strange result
    // if user search "../" etC.
    whc.baseUrl = "/searches/show-result/" + query + "/" + lang;
    whc.lang = c.lang;
    c.queryStr = query;
    c.queryLang = lang;
    whc.words = searchesModel.simple(query, lang, size, offset);
    c.whc = whc;

    render ("searches_show_result", c);
}

};
