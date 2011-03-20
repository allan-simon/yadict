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
	wc(serv),
    tc(serv),
    sc(serv),
    mc(serv),
	uc(serv),
    meanings(serv),
    logs(serv)
{

	add(wc,"^/words(.*)",1);
	add(tc,"^/translations(.*)",1);
    add(sc,"^/searches(.*)",1);
    add(mc,"^/metas(.*)",1);
    add(uc,"^/users(.*)",1);
    add(meanings,"^/meanings(.*)",1);
    add(logs,"^/logs(.*)",1);
    //NOTE important to add the page controller at the end
    //as its regexp is more global
	add(pc,"(.*)",1);

    // TODO to move in the language singleton
    cppcms::json::object langs = settings().at("shanghainesedict.interfacelangs").object();
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
    session().load();
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
        std::string toDispatch = "/" + matches[1] + matches[2];

        if (url == "/") {
            response().set_redirect_header("/eng" );
            return;
        }
        //then it means that only the /lang/ was missing
        //we try to add the language before the previous url
        //TODO maybe we can avoid redirection if we know that
        //the produced url will lead to nowhere
        //bu doing this :
	    //if (dispatcher().dispatch(toDispatch)) {
        //   redirect
        //} else { 
        //  404
        //}
        //doesn't work  because dispatch actually fill the response
        //so the header can't be written anymore
        //so we need a function that check url without calling the handler
        response().set_redirect_header("/eng" + toDispatch);
    }
}



} // namespace apps
