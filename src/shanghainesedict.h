#ifndef SHDICT_H
#define SHDICT_H

#include <cppcms/application.h>
#include <cppdb/frontend.h>


#include <contents/content.h>

#include "controllers/Pages.h"
#include "controllers/Words.h"
#include "controllers/Translations.h"
#include "controllers/Searches.h"
//#include "controllers/Users.h"



namespace content {
	class homepage;
}

namespace apps {


class Shanghainesedict : public cppcms::application {

private:
	controllers::Pages pc;
	controllers::Words wc;
	controllers::Translations tc;
	controllers::Searches sc;
	//controllers::Users uc;
    std::map<std::string,std::string> lang_map;

public:
	Shanghainesedict(cppcms::service &w);
	void main(std::string url);
};
}
#endif
