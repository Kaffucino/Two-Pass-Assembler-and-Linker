#include "Section.hpp"
#include <iomanip>
#include <bits/stdc++.h>

Section::Section(string naziv){
    this->naziv=naziv;
    size=0;
}

void Section:: dodaj_element(int vrednost,string tip,bool linker){

    elementSekcije e(vrednost,tip);
    elementi.push_back(e);
   
    if(linker==false){
    if(tip=="instr_dva_registra" || tip=="instr_jedan_registar" )
    size+=2;
    if(tip=="push" || tip=="pop")
    size+=3;

    if(tip=="nula-adresna")
    size+=1;
    if(tip=="ldr_str_immediate" || tip=="ldr_str_mem" || tip=="ldr_str_PC" || tip=="ldr_str_pom")
    size+=5;

    if(tip=="ldr_str_regdir" || tip=="ldr_str_regindir")
    size+=3;

    if(tip=="instr_skoka_imm" || tip=="instr_skoka_mem" || tip=="instr_skoka_pom" || tip=="instr_skoka_PC")
    size+=5;

    if(tip=="instr_skoka_reg" || tip=="instr_skoka_regind")
    size+=3;

    if(tip=="word_LITERAL" || tip=="word_SIMBOL")
    size+=2;

    if(tip==".skip")
    size+=vrednost;
    }

}


//Procesor je little endian!
string obradi_vrednost(int vrednost){

    stringstream stream;
    stream<<'0'<<hex<<uppercase<<vrednost;
    string tok=stream.str();

    int cnt=0;
    
     for(int i=0;i<tok.size();i++)
     if(isdigit(tok.at(i)) || tok.at(i)=='A' || tok.at(i)=='B' || tok.at(i)=='C'|| tok.at(i)=='D'|| tok.at(i)=='E'|| tok.at(i)=='F')
     cnt++;
    
    if(cnt==3)
    tok=tok.substr(1,tok.size());
    

    return tok;

}

string obradi_literal(string literal){
    string l="00";
    l.append(literal);
   
    int cnt=0;
    string krajnje="";
    for(int i=l.size()-1;i>=0;i--){
        cnt++;
        char c=l.at(i);
        krajnje+=c;
        if(cnt==4){
            break;
        }

    }
    reverse(krajnje.begin(),krajnje.end());
    return krajnje;
}

    string obradi_skip(int vrednost){

        string value="";
        for(int i=0;i<vrednost ; i++){
            value.append("00");
            if(i!=vrednost-1)
            value.append(" ");    
        }
        return value;
    }

    


ostream& operator <<(ostream& os, const Section&sec){
    
    os<<"#"<<sec.naziv<<endl;
    
    for(int i=0;i<sec.elementi.size();i++){
        
        if(sec.elementi.at(i).tip=="instr_dva_registra"){
                
        int vrednost=sec.elementi.at(i).vrednost;
        
        string hexa_vrednost=obradi_vrednost(vrednost);
        reverse(hexa_vrednost.begin(),hexa_vrednost.end());
        
        int vrednost_regsrc=sec.elementi.at(i+1).vrednost;
        int vrednost_regdest=sec.elementi.at(i+2).vrednost;
        i+=2;
        string regdest=to_string(vrednost_regdest);
        string regsrc=to_string(vrednost_regsrc);
       
        hexa_vrednost.append(" ");
        hexa_vrednost.append(regsrc);
        hexa_vrednost.append(regdest);

        
        os<<hexa_vrednost<<endl;
        }else if(sec.elementi.at(i).tip=="instr_jedan_registar"){
            int vrednost=sec.elementi.at(i).vrednost;
        
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());

            int vrednost_registra=sec.elementi.at(i+1).vrednost;
            i++;
            string reg=to_string(vrednost_registra);
            hexa_vrednost.append(" ");
            hexa_vrednost.append(reg);
            hexa_vrednost.append("F");
            os<<hexa_vrednost<<endl;
        }else if(sec.elementi.at(i).tip=="nula-adresna"){

            int vrednost=sec.elementi.at(i).vrednost;
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());
            os<<hexa_vrednost<<endl;

        }else if(sec.elementi.at(i).tip=="ldr_str_immediate" || sec.elementi.at(i).tip=="ldr_str_mem" || sec.elementi.at(i).tip=="ldr_str_PC"){
            int vrednost=sec.elementi.at(i).vrednost;
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());
            int registar=sec.elementi.at(i+1).vrednost;
            hexa_vrednost.append(" ");
            
            hexa_vrednost.append(to_string(registar));
            if(sec.elementi.at(i).tip!="ldr_str_PC")
            hexa_vrednost.append("F");
            else
            hexa_vrednost.append("7");

            hexa_vrednost.append(" ");
            
           
            hexa_vrednost.append("0");
            if(sec.elementi.at(i).tip=="ldr_str_immediate")
            hexa_vrednost.append("0");
            else if(sec.elementi.at(i).tip=="ldr_str_mem")
            hexa_vrednost.append("4");
            else
            hexa_vrednost.append("3"); //PC rel

            hexa_vrednost.append(" ");
            int literal=sec.elementi.at(i+2).vrednost;
            i+=2;
            //string low="??";
            //string high="??";
            string low="00";
            string high="00";
            if(literal!=-1){
            string literal_hexa=obradi_vrednost(literal);
            
            string lit=obradi_literal(literal_hexa);
             
             high=lit.substr(0,2);
             low=lit.substr(2,lit.size());
            // reverse(high.begin(),high.end());
            // reverse(low.begin(),low.end());
            }
            hexa_vrednost.append(low);
            hexa_vrednost.append(" ");

            hexa_vrednost.append(high);
            os<<hexa_vrednost<<endl;

        }else if(sec.elementi.at(i).tip=="ldr_str_regdir" ||sec.elementi.at(i).tip=="ldr_str_regindir"){
        int vrednost=sec.elementi.at(i).vrednost;
        
        string hexa_vrednost=obradi_vrednost(vrednost);
        reverse(hexa_vrednost.begin(),hexa_vrednost.end());
        
        int vrednost_regsrc=sec.elementi.at(i+1).vrednost;
        int vrednost_regdest=sec.elementi.at(i+2).vrednost;
        
        string regdest=to_string(vrednost_regdest);
        string regsrc=to_string(vrednost_regsrc);
       
        hexa_vrednost.append(" ");
        hexa_vrednost.append(regsrc);
        hexa_vrednost.append(regdest);
        hexa_vrednost.append(" ");
        hexa_vrednost.append("0");
        if(sec.elementi.at(i).tip=="ldr_str_regdir")
        hexa_vrednost.append("1");
       else
        hexa_vrednost.append("2");
        i+=2;
        os<<hexa_vrednost<<endl;


         }else if(sec.elementi.at(i).tip=="ldr_str_pom"){ // reg indi s pomerajem
        int vrednost=sec.elementi.at(i).vrednost;        
        string hexa_vrednost=obradi_vrednost(vrednost);
        reverse(hexa_vrednost.begin(),hexa_vrednost.end());
        int vrednost_regdest=sec.elementi.at(i+1).vrednost;
        int vrednost_regsrc=sec.elementi.at(i+2).vrednost;
        int literal=sec.elementi.at(i+3).vrednost;
        hexa_vrednost.append(" ");
        hexa_vrednost.append(to_string(vrednost_regdest));
        hexa_vrednost.append(to_string(vrednost_regsrc));
        hexa_vrednost.append(" ");
        hexa_vrednost.append("0");
        hexa_vrednost.append("3");
        hexa_vrednost.append(" ");

            //string low="??";
            //string high="??";
            string low="00";
            string high="00";
            if(literal!=-1){
            string literal_hexa=obradi_vrednost(literal);
            string lit=obradi_literal(literal_hexa);
              high=lit.substr(0,2);
              low=lit.substr(2,lit.size());
             
            }
            hexa_vrednost.append(low);
            hexa_vrednost.append(" ");

            hexa_vrednost.append(high);
            i+=3;
            os<<hexa_vrednost<<endl;
        }else if(sec.elementi.at(i).tip=="word_LITERAL" || sec.elementi.at(i).tip=="word_SIMBOL"){
                string ispis="";
                
                while(1){
                if(sec.elementi.at(i).tip=="word_LITERAL"){    
                string vrednost=obradi_vrednost(sec.elementi.at(i).vrednost);
                string literal=obradi_literal(vrednost);
                string low=literal.substr(0,2);
                string high=literal.substr(2,literal.size());

                ispis.append(high);
                ispis.append(" ");
                ispis.append(low);



                 }else{ //word_SIMBOL
                    int vrednost=sec.elementi.at(i).vrednost;
                    string low;
                    string high;
                    string value;
                    string lit;
                    if(vrednost==-1){
                        high="00";
                        low="00";
                        
                    }else{
                        value=obradi_vrednost(vrednost);
                        lit=obradi_literal(value);    
                        low=lit.substr(0,2);
                        high=lit.substr(2,lit.size());

                    }
                        
                ispis.append(high);
                ispis.append(" ");
                ispis.append(low);

                }

                if((sec.elementi.size()-1 > i) &&( sec.elementi.at(i+1).tip=="word_LITERAL" || sec.elementi.at(i+1).tip=="word_SIMBOL")){
                    ispis.append("\n");
                    i++;
                }else
                break;


                }
                
                os<<ispis<<endl;

        }else if(sec.elementi.at(i).tip=="push" || sec.elementi.at(i).tip=="pop"){

            int vrednost=sec.elementi.at(i).vrednost;
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());
            int registar=sec.elementi.at(i+1).vrednost;
            hexa_vrednost.append(" ");
            hexa_vrednost.append(to_string(registar));  
            hexa_vrednost.append("6");//SP
            hexa_vrednost.append(" ");
            if(sec.elementi.at(i).tip=="push")
            hexa_vrednost.append("1");
            else
            hexa_vrednost.append("4");

            hexa_vrednost.append("2");

            os<<hexa_vrednost<<endl;

        }else if(sec.elementi.at(i).tip=="instr_skoka_imm" ||sec.elementi.at(i).tip=="instr_skoka_mem" || sec.elementi.at(i).tip=="instr_skoka_PC" ){
            
            int vrednost=sec.elementi.at(i).vrednost;
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());
            hexa_vrednost.append(" ");
            hexa_vrednost.append("F");
            if(sec.elementi.at(i).tip!="instr_skoka_PC")
            hexa_vrednost.append("F");
            else
            hexa_vrednost.append("7");
            
            hexa_vrednost.append(" ");
            hexa_vrednost.append("0");
            if(sec.elementi.at(i).tip=="instr_skoka_imm")
            hexa_vrednost.append("0"); //imm
            else if(sec.elementi.at(i).tip=="instr_skoka_mem")
            hexa_vrednost.append("4"); //mem
            else
            hexa_vrednost.append("3");//PC rel

            hexa_vrednost.append(" ");
           // string high="??";
            //string low="??";
              string high="00";
              string low="00";

            int operand=sec.elementi.at(i+1).vrednost;
            
            if(operand!=-1){
            string hexa_operand=obradi_vrednost(operand);
            
            string lit=obradi_literal(hexa_operand);
           
            low=lit.substr(0,2);
            high=lit.substr(2,lit.size());
            }
            hexa_vrednost.append(high);
            hexa_vrednost.append(" ");
            hexa_vrednost.append(low);
            i++;
            os<<hexa_vrednost<<endl;

        }else if(sec.elementi.at(i).tip=="instr_skoka_reg" ||sec.elementi.at(i).tip=="instr_skoka_regind"){
            
            int vrednost=sec.elementi.at(i).vrednost;
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());
            hexa_vrednost.append(" ");
            hexa_vrednost.append("F");
            hexa_vrednost.append(to_string( sec.elementi.at(i+1).vrednost)); //reg
            hexa_vrednost.append(" ");
            hexa_vrednost.append("0");
            if(sec.elementi.at(i).tip=="instr_skoka_reg")
            hexa_vrednost.append("1");
            else
            hexa_vrednost.append("2");
            i++;
            os<<hexa_vrednost<<endl;

        }else if(sec.elementi.at(i).tip=="instr_skoka_pom"){
            
            int vrednost=sec.elementi.at(i).vrednost;
            string hexa_vrednost=obradi_vrednost(vrednost);
            reverse(hexa_vrednost.begin(),hexa_vrednost.end());
            hexa_vrednost.append(" ");
            hexa_vrednost.append("F");
            hexa_vrednost.append(to_string( sec.elementi.at(i+1).vrednost));//reg
            hexa_vrednost.append(" ");
            hexa_vrednost.append("0");
            hexa_vrednost.append("3");//reg ind pom
            hexa_vrednost.append(" ");
            
            // string high="??";
            //string low="??";
              string high="00";
              string low="00";

            int operand=sec.elementi.at(i+2).vrednost;
            
            if(operand!=-1){
            string hexa_operand=obradi_vrednost(operand);
            
            string lit=obradi_literal(hexa_operand);
           
            low=lit.substr(0,2);
            high=lit.substr(2,lit.size());
            }
            hexa_vrednost.append(high);
            hexa_vrednost.append(" ");
            hexa_vrednost.append(low);
            i+=2;
            os<<hexa_vrednost<<endl;

        }else if(sec.elementi.at(i).tip==".skip"){
            int vrednost=sec.elementi.at(i).vrednost;
            
            string broj_nula=obradi_skip(vrednost);
            os<<broj_nula<<endl;
        }

    }
    
    return os;

}