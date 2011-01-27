#ifndef SHDICT_MODELS_TATOHYPERDB_H
#define SHDICT_MODELS_TATOHYPERDB_H

#include <iostream>
#include "generics/Singleton.h"

extern "C" {
#include "tato/hyperdb.h"
}


class TatoHyperDB : public Singleton<TatoHyperDB>{
    friend class Singleton<TatoHyperDB>;

    private:
        TatoHyperDb *tatoHyperDb;
        TatoHyperDB(std::string data_path);
        ~TatoHyperDB();

    public:
        TatoHyperDb *get_database_pointer();
        void dump(std::string path);
        

};


#endif
