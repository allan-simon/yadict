#ifndef TATOEBACPP_MODELS_TATOHYPERDB_H
#define TATOEBACPP_MODELS_TATOHYPERDB_H

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
        TatoHyperDb *getDatabasePointer();
        void dump(std::string path);
        

};


#endif
