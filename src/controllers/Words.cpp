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
  	disp->assign("/show-all", &Words::show_all, this);
  	disp->assign("/show-random", &Words::show_random, this);

  	disp->assign("/add", &Words::add, this);
  	disp->assign("/add-treat", &Words::add_treat, this);

  	disp->assign("/edit/(\\d+)", &Words::edit, this, 1);
  	disp->assign("/edit-treat", &Words::edit_treat, this);

  	disp->assign("/delete-by-id/(\\d+)", &Words::delete_by_id, this, 1);
}

/**
 * Display all the words with the specified string
 * NOTE: a "string" can in fact refer to several words (plural etc.)
 * or totally different word in different languages
 *
 */

void Words::show(std::string str) {

	contents::Words c;
	contents::WordsHelper whc;
    initContent(c);

    whc.fetcher = wordModel.getWordsWithStr(str);
    c.whc = whc;
    render ("words_show", c);
    tato_hyper_item_fetcher_free(whc.fetcher);
}

/**
 * Display all the words in the database
 */

void Words::show_all() {

	contents::Words c;
	contents::WordsHelper whc;
    initContent(c);

    whc.fetcher = wordModel.getAllWords();

    c.whc = whc;
    render ("words_show_all", c);
    tato_hyper_item_fetcher_free(whc.fetcher);
}


/**
 * Display a random word
 */

void Words::show_random() {

	contents::Words c;
	contents::WordsHelper whc;
    initContent(c);

    whc.fetcher =  wordModel.getRandomWord();
    if (whc.fetcher->items[0] != NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show/" + std::string(whc.fetcher->items[0]->str)
        );

     } else { // if there's no word in the database
        // TODO maybe redirect to /words/show
        // with an error message in session to explain no words exist
        response().set_redirect_header("/" + c.lang );
    }
    tato_hyper_item_fetcher_free(whc.fetcher);
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
        "/" + c.lang +"/words/show-all"
    );
}

/**
 * Display the page with a form to edit the information about a word
 */
void Words::edit(std::string wordId) {
	int id = atoi(wordId.c_str());

	contents::WordsEdit c;
	contents::WordsHelper whc;

    initContent(c);

    whc.fetcher = wordModel.getWordWithId(id);
    // if no item with this id
    if (whc.fetcher->items[0] == NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        tato_hyper_item_fetcher_free(whc.fetcher);
        return;
    }

    // Set the value of the form to the current value
    // So one will know what he is editing
    c.editWord.wordId.value(wordId);
    c.editWord.wordLang.selected_id(
        std::string(whc.fetcher->items[0]->lang->code)
    );
    c.editWord.wordString.value(
        std::string(whc.fetcher->items[0]->str)
    );

    c.whc = whc;
    render("words_edit",c);
    tato_hyper_item_fetcher_free(whc.fetcher);
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
        "/" + c.lang +"/words/show-all"
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
        "/" + c.lang +"/words/show-all"
    );
}





} // End namespace
