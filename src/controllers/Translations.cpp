#include "Controller.h"
#include "controllers/Translations.h"
#include "shanghainesedict.h"
#include "contents/translations.h"

#include "models/TatoHyperDB.h"
#include "models/Translations.h"

namespace controllers {

/**
 *
 */
Translations::Translations(cppcms::service &serv): Controller(serv) {
    cppcms::url_dispatcher* d = &dispatcher();

    d->assign("/add-to/(\\d+)", &Translations::add_to, this, 1);
    d->assign("/add-to-treat", &Translations::add_to_treat, this);
    d->assign("/remove/(\\d+)/from/(\\d+)", &Translations::remove, this, 1, 2);

    // only for ""API""
    d->assign("/link/(\\d+)/and/(\\d+)", &Translations::link, this, 1, 2);
}

void Translations::link(std::string origWordId, std::string transWordId) {
    if (!is_logged()) {
        go_back_to_previous_page();
        return;
    }

	int origId = atoi(origWordId.c_str());
	int transId = atoi(origWordId.c_str());


    transModel.add_to(
        transId,
        0,
        origId
    );

    transModel.add_to(
        origId,
        0,
        transId
    );

}

/**
 *
 */
void Translations::add_to(std::string origWordId) {
    if (!is_logged()) {
        go_back_to_previous_page();
        return;
    }

	int origId = atoi(origWordId.c_str());
    contents::TranslationsAdd c;
    init_content(c);

	contents::WordsHelper whc;
    models::Words wordsModel;
    whc.lang = c.lang;
    
    whc.fetcher = wordsModel.get_word_with_id(origId);
    whc.packedTrans = wordsModel.pack_translations(whc.fetcher); 

    TatoHyperItem* word = whc.fetcher->items[0];
     // if no item with this id

    if (word == NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        tato_hyper_item_fetcher_free(whc.fetcher);
        return;
    }
    // set the hidden value of the form
    std::ostringstream oss;
    oss << wordsModel.get_translation_relation(word);

    c.addTranslation.origWordId.value(origWordId);
    c.addTranslation.translationId.value(
        oss.str()
    );
    
    c.whc = whc;
    render("translations_add_to", c);   

    tato_hyper_item_fetcher_free(whc.fetcher);
}

/**
 *
 */

void Translations::add_to_treat() {
    if (!is_logged()) {
        go_back_to_previous_page();
        return;
    }

    contents::TranslationsAdd c;
    init_content(c);

    c.addTranslation.load(context());

    models::Words wordsModel;
    std::string redirectUrl = "/" + c.lang +"/words/show-all" ;

    if (c.addTranslation.validate()) {
        // TODO : handle if something wrong happen while saving
        std::string transLang = c.addTranslation.wordLang.selected_id();
        std::string transStr = c.addTranslation.wordString.value();
        std::string strOrigWordId = c.addTranslation.origWordId.value();
        int origWordId = atoi(strOrigWordId.c_str());
        
        TatoHyperItem *translation = wordsModel.add_word(
            transLang,
            transStr
        );

        // if we were not able to add the word
        // we test if the word didn't exist before
        // TODO with exception we should be able to avoid this
        // by adding a field with the TatoHyperItem in the exception
        // raised by addWord
        if (translation == NULL) {
           translation = wordsModel.get_word_with_lang_str(transLang, transStr); 
        }

        if (translation != NULL) {
            transModel.add_to(
                origWordId,
                atoi(c.addTranslation.translationId.value().c_str()),
                translation->id
            );

            transModel.add_to(
                translation->id,
                0,
                origWordId
            );
        };

        redirectUrl = "/" + c.lang +"/translations/add-to/" + strOrigWordId;

    }

    //TODO handle if the ID of original word send in headers 
    //are not an id
    response().set_redirect_header(redirectUrl);
}

/**
 *
 */
void Translations::remove(std::string transIdStr, std::string origIdStr) {
    if (!is_logged()) {
        go_back_to_previous_page();
        return;
    }

    int transId = atoi(transIdStr.c_str());
    int origId = atoi(origIdStr.c_str());

    transModel.remove(transId, origId);
    transModel.remove(origId, transId);

    go_back_to_previous_page();
}

}

