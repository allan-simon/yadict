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

  	disp->assign("/show/(.+)$", &Words::show, this, 1);
  	disp->assign("/show-in/(.+)/(.+)$", &Words::show_in, this, 1, 2);
  	disp->assign("/show-random$", &Words::show_random, this);

  	disp->assign("/show-all$", &Words::show_all, this);
  	disp->assign("/show-all/(\\d+)/(\\d+)$", &Words::show_all, this, 1, 2);

  	disp->assign("/show-all-langs-filter-treat$", &Words::show_all_langs_filter_treat, this);

  	disp->assign("/show-all-in/(\\w+)", &Words::show_all_in, this, 1);
  	disp->assign("/show-all-in/(\\w+)/(\\d+)/(\\d+)", &Words::show_all_in, this, 1, 2, 3);

  	disp->assign("/add$", &Words::add, this);
  	disp->assign("/add-treat$", &Words::add_treat, this);

  	disp->assign("/edit/(\\d+)$", &Words::edit, this, 1);
  	disp->assign("/edit-treat$", &Words::edit_treat, this);

  	disp->assign("/delete-by-id/(\\d+)$", &Words::delete_by_id, this, 1);

  	disp->assign("/stardict-export/(\\w+)/(\\w+)$", &Words::startdict_export, this, 1, 2);
}

/**
 * Display all the words with the specified string
 * NOTE: a "string" can in fact refer to several words (plural etc.)
 * or totally different word in different languages
 *
 */

void Words::show(std::string str) {

	contents::WordsShow c;
	contents::WordsHelper whc;
    init_content(c);
    c.wordStr = str;
    whc.lang = c.lang;

    whc.words = wordModel.get_words_with_str(str);
    c.whc = whc;
    render ("words_show", c);
}

/**
 *
 */
void Words::show_in(std::string wordStr, std::string wordLang) {

	contents::WordsShow c;
    init_content(c);
    c.wordStr = wordStr;
    c.wordLang = wordLang;

	contents::WordsHelper whc(
        wordModel.get_word_with_lang_str(wordLang, wordStr)
    );
    whc.lang = c.lang;

    c.whc = whc;
    render ("words_show", c);
}




/**
 * Display all the words in the database
 */

void Words::show_all() {
    show_all("1", "50");
}

void Words::show_all(std::string offsetStr, std::string sizeStr) {
    unsigned int size = atoi(sizeStr.c_str());
    unsigned int offset = atoi(offsetStr.c_str()) - 1;

	contents::Words c;
	contents::WordsHelper whc;
    init_content(c);

    whc.baseUrl = "/words/show-all";
    whc.lang = c.lang;
    whc.words = wordModel.get_all_words(offset, size);

    c.whc = whc;
    c.paginationSize = size;
    render ("words_show_all", c);
}
/**
 *
 */

void Words::show_all_in(std::string lang) {
    show_all_in(lang, "1", "50");
}

void Words::show_all_in(std::string filterLang, std::string offsetStr, std::string sizeStr) {
    unsigned int size = atoi(sizeStr.c_str());
    unsigned int offset = atoi(offsetStr.c_str()) - 1;

	contents::WordsAllIn c;
	contents::WordsHelper whc;
    init_content(c);
    c.filterLang = filterLang;

    whc.baseUrl = "/words/show-all-in/" + filterLang;
    whc.lang = c.lang;
    whc.words = wordModel.get_all_words_in_lang(filterLang, offset, size);

    c.whc = whc;
    c.paginationSize = size;
    render ("words_show_all_in", c);
}

void Words::show_all_langs_filter_treat() {
	forms::LangsFilter langsFilter;
    langsFilter.load(context());

    if (langsFilter.validate()) {
        response().set_redirect_header(
            "/" + get_interface_lang() +"/words/show-all-in" +
            "/" + langsFilter.langFilter.selected_id()
        );

    } else {
        response().set_redirect_header(
            request().http_referer()
        );
    }

}


/**
 * Display a random word
 */

void Words::show_random() {

    results::Word word = wordModel.get_random_word();
    if (word.exists()) {
        response().set_redirect_header(
            "/" + get_interface_lang() +"/words/show-in" +
            "/" + word.text + 
            "/" + word.lang 
        );

     } else { // if there's no word in the database
        // TODO maybe redirect to /words/show
        // with an error message in session to explain no words exist
        go_back_to_previous_page();
    }
}

/**
 * Display the page with a form to add new words
 */
void Words::add() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN(); 

	contents::WordsAdd c;
    init_content(c);

    render("words_add",c);
}


/**
 * virtual page, used to received the data from the words/add form
 * and treat them
 */
void Words::add_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	contents::WordsAdd c;
    init_content(c);
    c.addWord.load(context());
    
    if (c.addWord.validate()) {
        // TODO : handle if something wrong happen while saving
        results::Word word = wordModel.add_word(
            c.addWord.wordLang.selected_id(),
            c.addWord.wordString.value(),
            get_current_user_id()
        );
        if (word.exists()) {
            
            response().set_redirect_header(
                "/" + c.lang +"/words/show-in"
                "/" + word.text + "/" + word.lang
            );
            return; 
        }
    }

    response().set_redirect_header(
        "/" + c.lang +"/words/show-all"
    );
}

/**
 * Display the page with a form to edit the information about a word
 */
void Words::edit(std::string wordId) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	int id = atoi(wordId.c_str());

	contents::WordsEdit c;
    init_content(c);

	contents::WordsHelper whc(
        wordModel.get_word_with_id(id)
    );
    whc.lang = c.lang;
    // if no item with this id
    if (whc.empty()) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        return;
    }

    // Set the value of the form to the current value
    // So one will know what he is editing
    c.editWord.wordId.value(wordId);
    c.editWord.wordLang.selected_id(whc.words[0].lang);
    c.editWord.wordString.value(whc.words[0].text);


    c.whc = whc;
    render("words_edit",c);
}


/**
 * virtual page, used to received the data from the words/edit form
 * and treat them
 */
void Words::edit_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	forms::EditWord editWord;
    editWord.load(context());
    
    if (!editWord.validate()) {
        go_back_to_previous_page();
        return;
    }
    // TODO : handle if something wrong happen while saving
    wordModel.edit_word(
        atoi(editWord.wordId.value().c_str()), 
        editWord.wordLang.selected_id(),
        editWord.wordString.value(),
        get_current_user_id()
    );

    response().set_redirect_header(
        "/" + get_interface_lang() +"/words/show-in"
        "/" + editWord.wordString.value() +
        "/" +editWord.wordLang.selected_id()
    );
}

/**
 * virtual page, delete the item with the given id
 */
void Words::delete_by_id(std::string wordId) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
    
    //TODO test before if we really have an integer inside the string
    //TODO handle return value    
    wordModel.delete_word(
        atoi(wordId.c_str()),
        get_current_user_id()
    );

    go_back_to_previous_page();
}

/**
 *
 */
void Words::startdict_export(
    std::string src,
    std::string dest
) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();
    TatoHyperDB::get_instance("")->startdict_export(src, dest);
    
    go_back_to_previous_page();
}

} // End namespace
