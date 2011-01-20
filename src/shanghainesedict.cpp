#include <shanghainesedict.h>
#include <contents/content.h>

#include <cppcms/json.h>
#include <cppcms/application.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/http_response.h>
#include <cppcms/session_interface.h>

#include <booster/regex.h>

#include <iostream>
namespace apps {


Shanghainesedict::Shanghainesedict(cppcms::service &w) : 
	cppcms::application(w),
	pc(*this),
	wc(*this)
	//uc(*this)
{

	add(pc);
	add(wc);
	//add(uc);

    cppcms::json::object langs = settings().at("shanghainesedict.languages").object();
    for(cppcms::json::object::const_iterator p=langs.begin();p!=langs.end();++p) {
        lang_map[p->first]=p->second.str();
    }
}
static const booster::regex lang_regex("^/(\\w+)(/.*)?$");

void Shanghainesedict::main(std::string url) {
    std::cout << "call to main with url " << url << std::endl;
	booster::smatch matches;
	booster::regex_match(url, matches, lang_regex);
    std::map<std::string,std::string>::const_iterator p = lang_map.find(
        std::string(matches[1])
    );

    if (p == lang_map.end()) {
        session()["lang"] = "eng";
	    if (!dispatcher().dispatch(matches[1])) {
            response().set_redirect_header("/eng/" + matches[1] + matches[2] );
        } else {
		    response().make_error_response(cppcms::http::response::not_found);
        }
    }
    else {
	    context().locale(p->second);
        session()["lang"] = p->first;
	    if (!dispatcher().dispatch(matches[2])) {
		    response().make_error_response(cppcms::http::response::not_found);
	    }
    }
}



} // namespace apps
