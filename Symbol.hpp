#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;



class Symbol{

public:

int RB;
string naziv;
string sekcija;
int offset;
bool is_global=false;


Symbol(int RB,string naziv,string sekcija,int offset);

~Symbol(){}


friend ostream & operator<< (ostream& os, const Symbol& sym);


};