#include<stdio.h>
#include <iostream>
#include <string>
#include "Assembler.hpp"
#include "Linker.hpp"

using namespace std;

int main(int argc, char** argv){



if(argc!=5 && ((string)argv[1])=="asembler"){
    cout<<"Format asemblera nije dobar!";
    exit(1);
}
if(((string)argv[1])=="asembler" && (((string)argv[4])=="-o")){
    cout<<"Format asemblera nije dobar!";
    exit(1);
}

if(argc==5 && ((string)argv[1])=="asembler" ){
char* inputfajl;
char* outputfajl;

if(((string)argv[2])=="-o"){
    outputfajl=argv[3];
    inputfajl=argv[4];
}else{
    outputfajl=argv[4];
    inputfajl=argv[2];
}

Assembler asembler(inputfajl,outputfajl);



asembler.prvi_prolaz();

asembler.drugi_prolaz();

asembler.ispisi();

}
//deo za linker -linkable
vector<string> ulazni_fajlovi;

if(((string)argv[1])=="linker" && ((string)argv[2])=="-linkable"){
    
    if(argc<7){
        cout<<"Linkeru se moraju bar 2 fajla proslediti!"<<endl;
        exit(1);
    }
    if((string)(argv[argc-1])=="-o"){
        cout<<"Los format za linker"<<endl;
        exit(1);
    }

    char* outputfajl;
    int index=3;
    for(int i=0;i<argc-3;i++){
    
    if((string)(argv[index+i])=="-o"){
        i++;
        outputfajl=argv[index+i];
    }else if(((string)(argv[index+i])).at(0)=='-' && ((string)(argv[index+i])).at(1)!='o') //onva je -place
    {
        //ne radimo nista
    }else
    ulazni_fajlovi.push_back((string)(argv[index+i]));
    
    }
    Linker l(&ulazni_fajlovi,outputfajl);
    //l.test();
    l.generisi();
    l.azuriraj_RB_tabela_relok();
    l.obradi_relokacije();
    l.ispis();

}
if(((string)argv[1])=="linker" && ((string)argv[2])=="-hex"){
    vector<sekcija_adresa> lista_sekcija_adresa;
    char*output;
    for(int i=3;i<argc;i++){
        string token=(string)(argv[i]);
        if(token.at(0)=='-' && token.at(1)!='o'){ //onda je -place
            string sekcijaIadresa=token.substr(7,token.size());
            int pozicija=sekcijaIadresa.find('@');//index @

            string sekcija=sekcijaIadresa.substr(0,pozicija);
            
            string adresa=sekcijaIadresa.substr(pozicija+1,sekcijaIadresa.size());
            sekcija_adresa sa(sekcija,adresa);
            lista_sekcija_adresa.push_back(sa);

        }else if(token=="-o"){
             //izlazni fajl
             i++;
            output=argv[i];
            
        }else{
            ulazni_fajlovi.push_back(token);
        }  
    }

    Linker l(&ulazni_fajlovi,output,&lista_sekcija_adresa);
    l.generisi();
    l.obradi_relokacije();
    l.obradi_hex_format();
    l.ispisi_hex_format();


}


return 0;
}