#include "Controller.h"
#include "controllers/Words.h"
#include "shanghainesedict.h"
#include "contents/words.h"

#include "models/TatoHyperDB.h"

namespace controllers {

Words::Words(apps::Shanghainesedict& shdictapp) : Controller(shdictapp) {
    // TODO have /words/  and /words/show directing to some "index" page 
    // as cburgmer pointed out, some people do navigate by crafting url
  	shdictapp.dispatcher().assign("/words/show/(.+)", &Words::show, this, 1);
  	shdictapp.dispatcher().assign("/words/show_all", &Words::show_all, this);
  	shdictapp.dispatcher().assign("/words/show_random", &Words::show_random, this);
}

/**
 * Display all the words with the specified string
 * NOTE: a "string" can in fact refer to several words (plural etc.)
 * or totally different word in different languages
 *
 */

void Words::show(std::string str) {

	contents::Words c;
    //initContent(c);

    c.fetcher = wordModel.getWordsWithStr(str);
    render ("words_show", c);
}

/**
 * Display all the words in the database
 */

void Words::show_all() {

	contents::Words c;
    //initContent(c);

    std::ostringstream oss;
    oss << wordModel.getRandomWordId();
    std::cout << "lang = " << c.lang << std::endl; 

    render ("words_show_all", c);
}


/**
 * Display a random word
 */

void Words::show_random() {

	contents::Words c;
    initContent(c);

    c.fetcher =  wordModel.getRandomWord();
    if (c.fetcher->items[0] != NULL) {
        std::cout << "Will redirect to : " <<
            "/" + c.lang +"/words/show/" + std::string(c.fetcher->items[0]->str)
            << std::endl ;

        response().set_redirect_header(
            "/" + c.lang +"/words/show/" + std::string(c.fetcher->items[0]->str)
        );

     } else { // if there's no word in the database
        // TODO maybe redirect to /words/show
        // with an error message in session to explain no words exist
        response().set_redirect_header("/" + c.lang );
    }

}


} // End namespace
