#ifndef CONTENTS_METAS_H
#define CONTENTS_METAS_H

#include "contents/content.h"
#include "contents/forms/add_meta.h"
#include "contents/forms/edit_meta.h"
#include "contents/helpers/words.h"

namespace contents {

struct MetasAdd : public BaseContent {
    WordsHelper whc;
    forms::AddMeta addMeta;
};

struct MetasEdit : public BaseContent {
    WordsHelper whc;
    forms::EditMeta editMeta;
    std::string key;
};




}

#endif

