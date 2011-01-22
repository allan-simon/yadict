#include "Controller.h"
#include "controllers/Words.h"
#include "shanghainesedict.h"
#include "contents/words.h"

#include "models/TatoHyperDB.h"

namespace controllers {

Words::Words(cppcms::service &serv) : Controller(serv) {
    // TODO have /  and /show directing to some "index" page 
    // as cburgmer pointed out, some people do navigate by crafting url
    cppcms::url_dispatcher* disp = &dispatcher();

  	disp->assign("/show/(.+)", &Words::show, this, 1);
  	disp->assign("/show_all", &Words::show_all, this);
  	disp->assign("/show_random", &Words::show_random, this);

  	disp->assign("/add", &Words::add, this);
  	disp->assign("/add_treat", &Words::add_treat, this);

  	disp->assign("/edit/(\\d+)", &Words::edit, this, 1);
  	disp->assign("/edit_treat", &Words::edit_treat, this);

  	disp->assign("/delete_by_id/(\\d+)", &Words::delete_by_id, this, 1);
}

/**
 * Display all the words with the specified string
 * NOTE: a "string" can in fact refer to several words (plural etc.)
 * or totally different word in different languages
 *
 */

void Words::show(std::string str) {

	contents::Words c;
    initContent(c);

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
	int id = atoi(wordId.c_str());

	contents::WordsEdit c;
    initContent(c);

    c.fetcher = wordModel.getWordWithId(id);
    // if no item with this id
    if (c.fetcher->items[0] == NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show_all"
        );
        tato_hyper_item_fetcher_free(c.fetcher);
        return;
    }

    // Set the value of the form to the current value
    // So one will know what he is editing
    c.editWord.wordId.value(wordId);
    c.editWord.wordLang.selected_id(
        std::string(c.fetcher->items[0]->lang->code)
    );
    c.editWord.wordString.value(
        std::string(c.fetcher->items[0]->str)
    );

    render("words_edit",c);
}


/**
 * virtual page, used to received the data from the words/edit form
 * and treat them
 */
void Words::edit_treat() {

	contents::WordsEdit c;
    initContent(c);
    c.editWord.load(context());
    
    if (c.editWord.validate()) {
        // TODO : handle if something wrong happen while saving
        wordModel.editWord(
            atoi(c.editWord.wordId.value().c_str()), 
            c.editWord.wordLang.selected_id(),
            c.editWord.wordString.value()
        );
    }

    response().set_redirect_header(
        "/" + c.lang +"/words/show_all"
    );
}

/**
 * virtual page, delete the item with the given id
 */
void Words::delete_by_id(std::string wordId) {

	contents::WordsAdd c;
    initContent(c);
    
    //TODO test before if we really have an integer inside the string
    //TODO handle return value    
    wordModel.deleteWord(atoi(wordId.c_str()));

    response().set_redirect_header(
        "/" + c.lang +"/words/show_all"
    );
}





} // End namespace
