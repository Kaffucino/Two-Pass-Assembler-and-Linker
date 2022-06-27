#include "Relocation.hpp"

Relocation:: Relocation(string sekcija,int offset,string tip,int RB,string simbol){
    
    this->sekcija=sekcija;
    this->offset=offset;
    this->tip=tip;
    this->RB=RB;
    this->simbol=simbol;


}

ostream& operator <<(ostream&os, const Relocation&rel){

    os<<rel.sekcija<<"\t\t\t"<<" "<<rel.offset<<"\t\t\t"<<rel.tip<<"\t\t";
    if(rel.tip=="R_X86_64_16")
    os<<"\t";
    
    os<<" "<<rel.RB<<"\t\t"<<rel.simbol<<endl;

    return os;


}