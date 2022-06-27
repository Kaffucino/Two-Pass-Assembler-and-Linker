#include "Symbol.hpp"

Symbol::Symbol(int RB,string naziv,string sekcija,int offset){

    this->RB=RB;
    this->naziv=naziv;
    this->sekcija=sekcija;
    this->offset=offset;

}

ostream & operator<< (ostream& os, const Symbol& sym){

    os<<sym.RB<<"\t"<<sym.naziv;
    
    if(sym.naziv.size()<=3)
    os<<"\t"<<"\t"<<"\t"<<"\t"<<"\t";



    if(sym.naziv.size()>3 && sym.naziv.size()<=7)
    os<<"\t"<<"\t"<<"\t"<<"\t";
    if(sym.naziv.size()>7 && sym.naziv.size()<=11)
    os<<"\t"<<"\t"<<"\t";
    
    if(sym.naziv.size()>11 && sym.naziv.size()<=15)
    os<<"\t"<<"\t";

    if(sym.naziv.size()>16 && sym.naziv.size() <= 20)
    os<<"\t";
    if(sym.naziv.size() > 20)
    os<<"";

    os<<sym.sekcija;

    if(sym.sekcija.size()<=3)
    os<<"\t"<<"\t"<<"\t"<<"\t"<<"\t";

    if(sym.sekcija.size()>3 && sym.sekcija.size()<=7)
    os<<"\t"<<"\t"<<"\t"<<"\t";
    if(sym.sekcija.size()>7 && sym.sekcija.size()<=11)
    os<<"\t"<<"\t"<<"\t";
    
    if(sym.sekcija.size()>11 && sym.sekcija.size()<=15)
    os<<"\t"<<"\t";

    if(sym.sekcija.size()>16 && sym.sekcija.size() <= 20)
    os<<"\t";
    if(sym.sekcija.size() > 20)
    os<<"";


    if(sym.offset==-1)
    os<<"?";
     else
    os<<sym.offset;

 

    os<<"\t"<<"\t"<<"\t";

    if(sym.is_global)
    os<<"global";
    else
    os<<"local";

    return os;

}
