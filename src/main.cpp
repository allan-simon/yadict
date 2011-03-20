#include <shanghainesedict.h>

#include <iostream>

#include <cppcms/service.h>
#include <cppcms/json.h>
#include <cppcms/applications_pool.h>
#include <cppdb/frontend.h>

#include "models/TatoHyperDB.h"
#include "models/SearchEngine.h"
#include "generics/Languages.h"
#include "generics/ActionId.h"
#include "contents/Config.h"

using namespace std;
using namespace cppcms;

int main(int argc,char ** argv)
{
    try {

        service app(argc,argv);

        // :TODO: maybe it's not the better place 
        // but for the moment it's the only way I've found to initialize tatodb 
        // in order to have the pointer shared by every thread
        string dictPath = app.settings().get<string>("shanghainesedict.dictxml");
        TatoHyperDB::get_instance(dictPath);
        Languages::get_instance();
        Languages::get_instance()->init(
            app.settings().at("shanghainesedict.languages").object()
        );
        SearchEngine::get_instance();
        
        SearchEngine::get_instance()->init_indexed_metas(
            app.settings().at("shanghainesedict.indexedMetas").object()
        );
        cout << "[NOTICE] database loaded" << endl;
        TatoHyperDB::get_instance("")->feed_search_engine();
        cout << "[NOTICE] search engine indexed" << endl;
        singletons::ActionId::get_instance();
        Config *conf = Config::get_instance();

	    conf->cssPath = app.settings().get<string>("shanghainesedict.css");
	    conf->imgPath = app.settings().get<string>("shanghainesedict.img");
        conf->webPath = app.settings().get<string>("shanghainesedict.web");


        booster::intrusive_ptr<apps::Shanghainesedict> shdictapp = 
            new apps::Shanghainesedict(app);

        app.applications_pool().mount(
            shdictapp
        );
        app.run();
        // :TODO: remember to free tatoDB
        // We dump the data once the server is going to close
        // TODO add the dump path in the config.js
        //
        cout << "[NOTICE] going to dump the database" << endl;
        //TatoHyperDB::get_instance("")->dump("dump.xml");
        cout << "[NOTICE] save into sql" << endl;
        singletons::ActionId::kill();
        SearchEngine::kill();
        TatoHyperDB::kill();
        Config::kill();
        Languages::kill();

    } catch(std::exception const &e) {
        cerr<<e.what()<<endl;
    }
}

