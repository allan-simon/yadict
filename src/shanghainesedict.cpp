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


Shanghainesedict::Shanghainesedict(cppcms::service &serv) : 
	cppcms::application(serv),
	pc(serv),
	wc(serv)
	//uc(*this)
{

	add(pc,"^$",0);
	add(wc,"^/words(.*)",1);
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

    // if we known the language
    if (p != lang_map.end()) {
	    context().locale(p->second);
        session()["lang"] = p->first;
        // if the other part of the url is random crap => 404
	    if (!dispatcher().dispatch(matches[2])) {
		    response().make_error_response(cppcms::http::response::not_found);
	    }
    // if we don't know the lang / the lang is missing in the url
    } else {
        // we set it to english
        // TODO should be the lang provided by the web browser
        session()["lang"] = "eng";
        //if we know how to dispatch the url
        //then it means that only the /lang/ was missing
	    if (!dispatcher().dispatch(matches[1]+matches[2])) {
            response().set_redirect_header("/eng/" + matches[1] + matches[2] );
        } else {
            // otherwise we generate a 404
		    response().make_error_response(cppcms::http::response::not_found);
        }

    }
}



} // namespace apps
