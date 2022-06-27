#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Assembler.hpp"
using namespace std;

struct masinski_kod{
  string kod;
  string sekcija;  
    
    masinski_kod(string s="", string k=""){
        kod=k;
        sekcija=s;
    }


};

struct sekcija_adresa{ //za hex deo
  string sekcija;
  string adresa;
  sekcija_adresa(string s,string a){
    sekcija=s;
    adresa=a;
  }

};


class Linker{

public:
string izlazni_fajl;
vector<string>*ulazni_fajlovi;
vector<sekcija_adresa>*lista_sekcija_adresa;

vector<Symbol> tabela_simbola;
vector<Section> lista_sekcija;
vector<masinski_kod> lista_kodova;


vector<Relocation> lista_relokacija;

Linker(vector<string>* fajlovi,string izlaz);
Linker(vector<string>* fajlovi,string izlaz,vector<sekcija_adresa>*lista);
void generisi();
bool sekcija_u_listi(string naziv,vector<string>*lista);
void ispis();
void obradi_relokacije();
Symbol* dohvati_simbol(string naziv);
int dohvati_RB_sekcije(string naziv);
int velicina_prethodnih(string naziv, int RB);
void soritraj_listu_kodova();
//void test();

//hex deo
int velicina_instrukcje(string masinski_kod);

void azuriraj_RB_tabela_relok(); //ako treba RB okruzujuce sekcije
void obradi_hex_format();
int nadji_adresu_sekcije(string sekcija);

int nadji_max_adresu();

string nadji_sekciju(string adresa);


void sortiraj_po_adresi();
void dodaj_ostale_adrese();
bool nalazi_se_u_sekcija_adresi(string sekcija);
int velicina_sekcije(string sekcija);
void ispisi_hex_format();


};