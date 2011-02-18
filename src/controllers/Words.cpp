#include "Controller.h"
#include "controllers/Words.h"
#include "shanghainesedict.h"
#include "contents/words.h"


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
    whc.fetcher = wordModel.get_words_with_str(str);
    models::TranslationsMap packedTransWithoutMeaning;
    whc.packedMeaningsTrans = wordModel.pack_translations(
        whc.fetcher,
        packedTransWithoutMeaning
    ); 
    whc.packedTransWithoutMeaning = packedTransWithoutMeaning;    

    c.whc = whc;
    render ("words_show", c);
    tato_hyper_item_fetcher_free(whc.fetcher);
}

/**
 *
 */
void Words::show_in(std::string wordStr, std::string wordLang) {

	contents::WordsShow c;
	contents::WordsHelper whc;
    init_content(c);
    c.wordStr = wordStr;
    whc.lang = c.lang;

    TatoHyperItemFetcher *fetcher = tato_hyper_item_fetcher_new(1, 0);
    tato_hyper_item_fetcher_add(
        fetcher,
        wordModel.get_word_with_lang_str(wordLang, wordStr)
    );
    whc.fetcher = fetcher;
    models::TranslationsMap packedTransWithoutMeaning;
    whc.packedMeaningsTrans = wordModel.pack_translations(
        whc.fetcher,
        packedTransWithoutMeaning
    ); 
    whc.packedTransWithoutMeaning = packedTransWithoutMeaning;    

    c.whc = whc;
    render ("words_show", c);
    tato_hyper_item_fetcher_free(whc.fetcher);
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
    whc.fetcher = wordModel.get_all_words(offset, size);

    c.whc = whc;
    render ("words_show_all", c);
    tato_hyper_item_fetcher_free(whc.fetcher);
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
    whc.fetcher = wordModel.get_all_words_in_lang(filterLang, offset, size);

    c.whc = whc;
    render ("words_show_all_in", c);
    tato_hyper_item_fetcher_free(whc.fetcher);
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

    TatoHyperItemFetcher *fetcher =  wordModel.get_random_word();
    TatoHyperItem* item = fetcher->items[0];
    if (item != NULL) {
        response().set_redirect_header(
            "/" + get_interface_lang() +"/words/show-in" +
            "/" + std::string(item->str) + 
            "/" + std::string(item->lang->code)
        );

     } else { // if there's no word in the database
        // TODO maybe redirect to /words/show
        // with an error message in session to explain no words exist
        go_back_to_previous_page();
    }
    tato_hyper_item_fetcher_free(fetcher);
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
        TatoHyperItem* item = wordModel.add_word(
            c.addWord.wordLang.selected_id(),
            c.addWord.wordString.value(),
            get_current_user_id()
        );
        if (item == NULL) {
            
            std::ostringstream oss;
            oss << item->id ;
            response().set_redirect_header(
                "/" + c.lang +"/words/add-to/" + oss.str()
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
	contents::WordsHelper whc;
    init_content(c);
    whc.lang = c.lang;

    whc.fetcher = wordModel.get_word_with_id(id);
    // if no item with this id
    if (whc.is_empty()) {
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
        "/" + get_interface_lang() +"/words/show-all"
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



} // End namespace
