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

  	shdictapp.dispatcher().assign("/words/add", &Words::add, this);
  	shdictapp.dispatcher().assign("/words/add_treat", &Words::add_treat, this);

  	shdictapp.dispatcher().assign("/words/edit/(\\d+)", &Words::edit, this, 1);
  	shdictapp.dispatcher().assign("/words/edit_treat", &Words::edit_treat, this);
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
    initContent(c);

    c.fetcher = wordModel.getAllWords();

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
        response().set_redirect_header(
            "/" + c.lang +"/words/show/" + std::string(c.fetcher->items[0]->str)
        );

     } else { // if there's no word in the database
        // TODO maybe redirect to /words/show
        // with an error message in session to explain no words exist
        response().set_redirect_header("/" + c.lang );
    }

}

/**
 * Display the page with a form to add new words
 */
void Words::add() {

	contents::WordsAdd c;
    initContent(c);

    render("words_add",c);
}


/**
 * virtual page, used to received the data from the words/add form
 * and treat them
 */
void Words::add_treat() {

	contents::WordsAdd c;
    initContent(c);
    c.addWord.load(context());
    
    if (c.addWord.validate()) {
        // TODO : handle if something wrong happen while saving
        wordModel.addWord(
            c.addWord.wordLang.selected_id(),
            c.addWord.wordString.value()
        );
    }

    response().set_redirect_header(
        "/" + c.lang +"/words/show_all"
    );
}

/**
 * Display the page with a form to edit the information about a word
 */
void Words::edit(std::string wordId) {

	contents::WordsAdd c;
    initContent(c);

    render("words_edit",c);
}


/**
 * virtual page, used to received the data from the words/edit form
 * and treat them
 */
void Words::edit_treat() {

	contents::WordsAdd c;
    initContent(c);
    c.addWord.load(context());
    
    if (c.addWord.validate()) {
        // TODO : handle if something wrong happen while saving
        /*
        wordModel.editWord(
            c.addWord.wordLang.selected_id(),
            c.addWord.wordString.value()
        );
        */
    }

    response().set_redirect_header(
        "/" + c.lang +"/words/show_all"
    );
}





} // End namespace
