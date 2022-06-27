#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

class Relocation{

public:

string sekcija;
int offset;
string tip;
int RB;
string simbol;

Relocation(string sekcija,int offset,string tip,int RB,string simbol);

friend ostream& operator <<(ostream&os, const Relocation&rel);

};