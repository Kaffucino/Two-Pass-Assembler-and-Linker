#include "Symbol.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include "Section.hpp"
#include "Relocation.hpp"
#pragma once
using namespace std;

enum InstructionCode{HALT,INT,IRET,CALL,RET,JMP,JEQ=21,JNE=37,JGT=53,XCHG=6,ADD,SUB=23,MUL=39,DIV=55,CMP=71,NOT=8,AND=24,OR=40,XOR=56,TEST=72,SHL=9,SHR=25,LDR=10,STR};


class Assembler{

public:

string ulazni_fajl;
string izlazni_fajl;
vector<Symbol> tabela_simbola;
vector <Section> tabela_sekcija;
vector<Relocation> tabela_relokacija;

int location_counter=0;

Assembler(string ulaz,string izlaz);

~Assembler(){}

void prvi_prolaz();

void drugi_prolaz();


string tip_tokena(string token);
InstructionCode get_kod_instrukcije(string instr);


bool sekcija_u_tabeli(string sekcija);

bool sekcija_sintaksa(string sekcija);

static const map<string,regex> tokenMapa;

static const map<InstructionCode,string> mapa_kodova;

// static const map<int,regex> sizeInstrMapa; // ne radi?

void dodaj_simbol(string simbol,string sekcija,int location);

bool dva_registra(string instrukcija);
bool da_li_je_registar(string reg);
bool jedan_registar(string instrukcija);
bool nula_adresna(string instrukcija);
int nadji_velicinu(string instrukcija);
bool instrukcija_skoka(string instrukcija);
bool simbol_u_tabeli(string simbol);
void ispisi();
Symbol*pronadji_simbol(string name);
bool sekcija_u_tabeli_sekcija(string naziv);
Section*dohvati_sekciju(string naziv);
};
