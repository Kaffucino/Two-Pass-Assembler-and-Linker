#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;


struct elementSekcije{
    string tip;
   // int offset;
    int vrednost;
    //int size;    

    elementSekcije(int v,string t){
        vrednost=v;
        tip=t;
    }

};

class Section{

public:
vector<elementSekcije> elementi;
string naziv;
int size; //velicina sekcije
Section(string naziv);

void dodaj_element(int vrednost,string tip,bool linker=false);

friend ostream& operator <<(ostream& os, const Section&sec);
friend class Linker;
};