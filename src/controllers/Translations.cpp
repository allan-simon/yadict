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

    d->assign("/add-to-word/(\\d+)", &Translations::add_to_word, this, 1);
    d->assign("/add-to-word-treat", &Translations::add_to_word_treat, this);

    d->assign("/add-to-meaning/(\\d+)/(\\d+)", &Translations::add_to_meaning, this, 1, 2);
    d->assign("/add-to-meaning-treat", &Translations::add_to_meaning_treat, this);

    d->assign("/remove/(\\d+)/from/(\\d+)", &Translations::remove, this, 1, 2);
    d->assign("/remove/(\\d+)/from-meaning/(\\d+)", &Translations::remove_from_meaning, this, 1, 2);

    // only for ""API""
    d->assign("/link/(\\d+)/and/(\\d+)", &Translations::link, this, 1, 2);
}

void Translations::link(std::string origWordId, std::string transWordId) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	int origId = atoi(origWordId.c_str());
	int transId = atoi(origWordId.c_str());


    transModel.add_one_way_link(
        transId,
        0,
        origId
    );

    transModel.add_one_way_link(
        origId,
        0,
        transId
    );

}

/**
 *
 */
void Translations::add_to_word(std::string origWordId) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	int origId = atoi(origWordId.c_str());
    contents::TranslationsAdd c;
    init_content(c);

	contents::WordsHelper whc;
    models::Words wordsModel;
    whc.lang = c.lang;
    
    whc.fetcher = wordsModel.get_word_with_id(origId);
    models::TranslationsMap packedTransWithoutMeaning;
    whc.packedMeaningsTrans = wordsModel.pack_translations(
        whc.fetcher,
        packedTransWithoutMeaning
    ); 
    whc.packedTransWithoutMeaning = packedTransWithoutMeaning;    

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

void Translations::add_to_word_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    //TODO only use form
    forms::AddTranslation addTrans;
    addTrans.load(context());

    if (!addTrans.validate()) {
        go_back_to_previous_page();
        return;
    }


    // TODO : handle if something wrong happen while saving
    std::string transLang = addTrans.wordLang.selected_id();
    std::string transText = addTrans.wordString.value();
    std::string strOrigWordId = addTrans.origWordId.value();
    int origWordId = atoi(strOrigWordId.c_str());
    int transRelId = atoi(addTrans.translationId.value().c_str());
    
    transModel.add_translation_to_word(
            origWordId,
            transRelId,
            transText,
            transLang
    );

    response().set_redirect_header(
        "/" + get_interface_lang() +"/translations/add-to-word/" + strOrigWordId
    );
}

/**
 *
 */
void Translations::remove(std::string transIdStr, std::string origIdStr) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int transId = atoi(transIdStr.c_str());
    int origId = atoi(origIdStr.c_str());

    transModel.remove(transId, origId);
    transModel.remove(origId, transId);

    go_back_to_previous_page();
}


/**
 *
 */
void Translations::add_to_meaning(
    std::string meaningIdStr,
    std::string wordIdStr
) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

	int wordId = atoi(wordIdStr.c_str());
	int meaningId = atoi(meaningIdStr.c_str());

    contents::TranslationsAddToMeaning c;
    init_content(c);
    c.meaningId = meaningId;
    
	contents::WordsHelper whc;
    models::Words wordsModel;

    whc.lang = c.lang;
    whc.fetcher = wordsModel.get_word_with_id(wordId);
    models::TranslationsMap packedTransWithoutMeaning;
    whc.packedMeaningsTrans = wordsModel.pack_translations(
        whc.fetcher,
        packedTransWithoutMeaning
    ); 
    whc.packedTransWithoutMeaning = packedTransWithoutMeaning;    

    c.whc = whc;

    TatoHyperItem* word = whc.fetcher->items[0];
     // if no item with this id

    if (word == NULL) {
        response().set_redirect_header(
            "/" + c.lang +"/words/show-all"
        );
        tato_hyper_item_fetcher_free(whc.fetcher);
        return;
    }

    // fill the form
    std::ostringstream oss;
    oss << wordsModel.get_translation_relation(word);

    c.addTransToMeaning.wordId.value(wordIdStr);
    c.addTransToMeaning.meaningId.value(meaningIdStr);
    
    render("translations_add_to_meaning", c);   

    tato_hyper_item_fetcher_free(whc.fetcher);
}

/**
 *
 */

void Translations::add_to_meaning_treat() {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    //TODO only use form
    forms::AddTransToMeaning addTransToMeaning;
    addTransToMeaning.load(context());

    if (!addTransToMeaning.validate()) {
        go_back_to_previous_page();
        return;
    }


    // TODO : handle if something wrong happen while saving
    std::string transLang = addTransToMeaning.transLang.selected_id();
    std::string transText = addTransToMeaning.transText.value();
    std::string wordIdStr = addTransToMeaning.wordId.value();
    std::string meaningIdStr = addTransToMeaning.meaningId.value();
    int wordId = atoi(wordIdStr.c_str());
    int meaningId = atoi(meaningIdStr.c_str());
    
    transModel.add_translation_to_meaning(
        meaningId,
        wordId,
        transText,
        transLang
    );


    response().set_redirect_header(
        "/" + get_interface_lang() +
        "/translations/add-to-meaning" +
        "/" + meaningIdStr + "/" + wordIdStr
    );
}

/**
 *
 */

void Translations::remove_from_meaning(
    std::string transIdStr,
    std::string meaningIdStr
) {
    CHECK_PERMISSION_OR_GO_TO_LOGIN();

    int transId = atoi(transIdStr.c_str());
    int meaningId = atoi(meaningIdStr.c_str());

    transModel.remove_from_meaning(transId, meaningId);

    go_back_to_previous_page();
}



}

