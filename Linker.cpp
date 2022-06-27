#include "Linker.hpp"

Linker::Linker(vector<string>* fajlovi,string izlaz){

this->ulazni_fajlovi=fajlovi;
this->lista_sekcija.push_back(Section(".ABS"));
this->izlazni_fajl=izlaz;

}
Linker:: Linker(vector<string>* fajlovi,string izlaz,vector<sekcija_adresa>*lista){
this->ulazni_fajlovi=fajlovi;
this->izlazni_fajl=izlaz;
this->lista_sekcija.push_back(Section(".ABS"));
this->lista_sekcija_adresa=lista;
}



string obradi_vrednost2(int vrednost){

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

string obradi_literal2(string literal){
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




Symbol* Linker:: dohvati_simbol(string naziv){

    Symbol*s=nullptr;
    for(int i=0;i<tabela_simbola.size();i++)
    if(tabela_simbola.at(i).naziv==naziv)
    s=&tabela_simbola.at(i);

    return s;

}
int Linker::dohvati_RB_sekcije(string naziv){

    for(int i=lista_sekcija.size()-1; i>=0 ; i--)
    if(lista_sekcija.at(i).naziv==naziv)
    return i;
    cout<<"Sekcija: "<<naziv<<"nije pronadjena u listi sekcija?";
    exit(2);
}
int Linker::velicina_prethodnih(string naziv, int RB){

    int size=0;
    for(int i=0;i<lista_sekcija.size();i++)
    if(lista_sekcija.at(i).naziv==naziv && i!=RB)
    size+=lista_sekcija.at(i).size;

    return size;
}

bool Linker:: sekcija_u_listi(string naziv,vector<string>*lista){

    for(int i=0;i<lista->size();i++)
    if(lista->at(i)==naziv)
    return true;

return false;
}


void Linker::soritraj_listu_kodova(){

    vector<masinski_kod> lista;
    vector<string> iskoriscene_sekcije;
    for(int i=0;i<lista_sekcija.size();i++){
       
        if(lista_sekcija.at(i).naziv!=".ABS" && !sekcija_u_listi(lista_sekcija.at(i).naziv,&iskoriscene_sekcije)){

            string sekcija=lista_sekcija.at(i).naziv;
            string s="#";
            s.append(sekcija);
            iskoriscene_sekcije.push_back(sekcija);
            lista.push_back(masinski_kod(s));
            for(int j=0;j<lista_kodova.size();j++){
                
                if(lista_kodova.at(j).sekcija==sekcija){
                
                lista.push_back(lista_kodova.at(j));
                
                }

            }







        }




    }

    lista_kodova=lista;
    
}




void Linker::generisi(){
    
    for(int i=0;i<ulazni_fajlovi->size();i++){

        ifstream ifs(ulazni_fajlovi->at(i));
        if(!ifs || !ifs.is_open()){
        cout<<"Greska prilikom otvranja fajla!"<<endl;
        exit(1);
        }
        string linija;
        string token;


       

            while(getline(ifs,linija)){

                    istringstream tok(linija);
                while(tok>>token){

                if(token=="POMOCNA")
                    break;

                if(token=="TABELA") //Tabela simbola
                break;
                string ime_sekcije=token;
                string velicina_sekcije;
                tok>>velicina_sekcije;    
                Section s(ime_sekcije);
                s.size=stoi(velicina_sekcije);
                
                lista_sekcija.push_back(s);
                }

                if(token=="TABELA") //ucitali smo podatke iz pomocne tabele
                break;

            }
            if(i==0){ // prvi fajl prepisujem takav kakav je
            //Tabela simbola
            while(getline(ifs,linija)){
                istringstream tok(linija);
                while(tok>>token){
                     
                     if(token=="RB")
                     break;  
                     if(token=="TABELA")
                     break;

                    string RB;
                    string Naziv;
                    string Sekcija;
                    string Offset;
                    string Global;
                    RB=token;
                    tok>>Naziv;
                    
                    tok>>Sekcija;
                    tok>>Offset;
                    tok>>Global;
                    int offset=Offset=="?" ? -1:stoi(Offset);

                    Symbol sym(stoi(RB),Naziv,Sekcija,offset);
                    if(Global=="global")
                    sym.is_global=true;
                    tabela_simbola.push_back(sym);
                }
                
                if(token=="TABELA")
                break;


            }
            //Tabela sekcija
            string sekcija;
           while(getline(ifs,linija)){
                istringstream tok(linija);
                
                string kod="";
                while(tok>>token){

                    if(token.at(0)=='#'){
                   lista_kodova.push_back(masinski_kod(token));    
                    sekcija=token.substr(1,token.size());
                    }
                    else if(token=="TABELA")
                    break;
                    else{
                        kod.append(token);
                        kod.append(" ");
                    }
        

                }
                 if(token=="TABELA")
                    break;
                if(kod!=""){    
                masinski_kod m(sekcija,kod);
                lista_kodova.push_back(m);
                }
           }     

           //Tabela relokacija
           getline(ifs,linija); //preskakanje 1 linije

            while(getline(ifs,linija)){
                 istringstream tok(linija);
                   if(tok>>token){
                       string sek;
                       string offset;
                       string tip_rel;
                       string RB;
                       string simbol;
                       sek=token;
                       tok>>offset;
                       tok>>tip_rel;
                       tok>>RB;
                       tok>>simbol;
                    Relocation r(sek,stoi(offset),tip_rel,stoi(RB),simbol);
                        lista_relokacija.push_back(r);

                   } 

            }







        }else{ //ostali fajlovi

             //Tabela simbola
            while(getline(ifs,linija)){
                istringstream tok(linija);
                while(tok>>token){
                     
                     if(token=="RB")
                     break;  
                     if(token=="TABELA")
                     break;

                    string RB;
                    string Naziv;
                    string Sekcija;
                    string Offset;
                    string Global;
                    RB=token;
                    tok>>Naziv;
                    tok>>Sekcija;
                    tok>>Offset;
                    tok>>Global;
                    int offset=Offset=="?" ? -1:stoi(Offset);




                    Symbol*sym=dohvati_simbol(Naziv);


                    
                    if(sym==nullptr){//nema simbola u tabeli simbola
                        
                        if(Sekcija=="?"){
                        Symbol s(tabela_simbola.size(),Naziv,Sekcija,offset);
                          if(Global=="global")
                          s.is_global=true;
                          tabela_simbola.push_back(s);
                        }
                        else{
                            int RB_sekcije=dohvati_RB_sekcije(Sekcija);  
                            int size=velicina_prethodnih(Sekcija,RB_sekcije);
                        Symbol s(tabela_simbola.size(),Naziv,Sekcija,offset+size);
                            if(Global=="global")
                          s.is_global=true;
                          tabela_simbola.push_back(s);
                        }
                      
                    }else{
                        if(sym->offset!=-1 && Offset!="?" && Naziv!=Sekcija){
                            
                            cout<<"Duplo definisanje simbola: "<<Naziv<<endl;
                            exit(3);
                        }else if(sym->offset==-1 && Offset!="?"){
                            int RB_sekcije=dohvati_RB_sekcije(Sekcija);  
                            int size=velicina_prethodnih(Sekcija,RB_sekcije);
                            sym->sekcija=Sekcija;
                            sym->offset=size+stoi(Offset);
                            
                        }else if(sym->offset==-1 && Offset=="?"){
                            /*Ne radimo nista*/
                        }else if(sym->offset!=-1 && Offset=="?"){
                            /*Ne radimo nista*/
                        }




                    }

                  
                }
                
                if(token=="TABELA")
                break;


            }
             //Tabela sekcija
               string sekcija;
           while(getline(ifs,linija)){
                istringstream tok(linija);
                
                string kod="";
                while(tok>>token){

                    if(token.at(0)=='#'){
                   lista_kodova.push_back(masinski_kod(token));    
                    sekcija=token.substr(1,token.size());
                    }
                    else if(token=="TABELA")
                    break;
                    else{
                        kod.append(token);
                        kod.append(" ");
                    }
        

                }
                 if(token=="TABELA")
                    break;
                if(kod!=""){    
                masinski_kod m(sekcija,kod);
                lista_kodova.push_back(m);
                }
           }     
            //Tabela relokacija
            getline(ifs,linija); //preskakanje 1 linije

            while(getline(ifs,linija)){
                 istringstream tok(linija);
                   if(tok>>token){
                       string sek;
                       string offset;
                       string tip_rel;
                       string RB;
                       string simbol;
                       sek=token;
                       tok>>offset;
                       tok>>tip_rel;
                       tok>>RB;
                        tok>>simbol;


                    int novi_rb=dohvati_simbol(simbol)->RB;
                  
                        

                    int RB_sekcije=dohvati_RB_sekcije(sek);  
                    int size=velicina_prethodnih(sek,RB_sekcije);
                    int novi_offset=size+stoi(offset);
                    
                    Relocation r(sek,novi_offset,tip_rel,novi_rb,simbol);
                        lista_relokacija.push_back(r);

                   } 

            }



        }





    }

    soritraj_listu_kodova();



}


int get_broj_bajtova(string instr){
    int size=0;
    istringstream tok(instr);
    string s;
    while(tok>>s){
        size++;
    }

    return size;
}

string promeni_masinski_kod(string kod,int bajt,string novo){
    string novi_kod="";
    istringstream tok(kod);
    string token;
    while(tok>>token && bajt>0){

        novi_kod.append(token);
        novi_kod.append(" ");
        bajt--;

    }
    novi_kod.append(novo.substr(0,2));
    novi_kod.append(" ");
    novi_kod.append(novo.substr(2,4));

    return novi_kod;

}

void Linker::azuriraj_RB_tabela_relok(){

    for(int i=0;i<lista_relokacija.size();i++){

        string simbol=lista_relokacija.at(i).simbol;
        string sekcija=lista_relokacija.at(i).sekcija;
        int RB_sekcije=dohvati_simbol(sekcija)->RB;
        if(lista_relokacija.at(i).RB!=RB_sekcije)
        lista_relokacija.at(i).RB=RB_sekcije;
    }


}

void Linker::obradi_relokacije(){

    for(int i=0;i<lista_relokacija.size();i++){
        
        string sekcija= lista_relokacija.at(i).sekcija;
        string tip = lista_relokacija.at(i).tip;
        int offset=lista_relokacija.at(i).offset;
        int cnt=0;
        for(int j=0;j<lista_kodova.size();j++){

                if(lista_kodova.at(j).sekcija==sekcija && lista_kodova.at(j).kod!=""){
                    int velicina_instr=get_broj_bajtova(lista_kodova.at(j).kod);
                    
                    if(cnt+velicina_instr<=offset){
                    cnt+=velicina_instr;

                    }else{ //onda je to taj masinski kod
                    int bajt=offset-cnt;

                    int vrednost=dohvati_simbol(lista_relokacija.at(i).simbol)->offset;
                    bool globalnost=dohvati_simbol(lista_relokacija.at(i).simbol)->is_global;   

                    if(globalnost)
                    vrednost=0;

                    if(tip=="R_X86_64_16PC" && globalnost==false)
                    vrednost-=2;
                    else if(tip=="R_X86_64_16PC" && globalnost==true)
                    vrednost=-2;
                    string novo=obradi_literal2(obradi_vrednost2(vrednost));
                    string high=novo.substr(2,4);
                    string low=novo.substr(0,2);
                    novo=high.append(low);
                   

                   string novi_masinski_kod=promeni_masinski_kod(lista_kodova.at(j).kod,bajt,novo);
                   lista_kodova.at(j).kod=novi_masinski_kod;      
                   break;

                    }
                    

                }


        }




    }
    
    for(int i=0;i<lista_relokacija.size();i++){

        string simbol=lista_relokacija.at(i).simbol;

        string sekcija="";
        if(dohvati_simbol(simbol)!=nullptr){
        sekcija=dohvati_simbol(simbol)->sekcija;
        //cout<<sekcija<<endl;
        }
        int RB;
        if(sekcija!=".ABS")
         RB=dohvati_simbol(sekcija)->RB;

         if(dohvati_simbol(simbol)->is_global ) 
         RB=dohvati_simbol(simbol)->RB;
         lista_relokacija.at(i).RB=RB;
    }

}




void Linker::ispis(){

        ofstream ofs(izlazni_fajl);   
        if(!ofs || !ofs.is_open()){
        cout<<"Greska prilikom otvranja fajla!"<<endl;
        exit(1);
        }
        ofs<<"POMOCNA TABELA"<<endl;
        for(int i=0;i<lista_sekcija.size();i++)
        ofs<<lista_sekcija.at(i).naziv<<"\t"<<lista_sekcija.at(i).size<<endl;
        ofs<<endl;

    ofs<<"TABELA SIMBOLA"<<endl;
    ofs<<"RB"<<"\t"<<"Naziv"<<"\t\t\t"<<"Sekcija "<<"\t\t\t"<<"offset"<<"\t\t"<<"GLOBAL/LOCAL"<<endl; 
    for(int i=0;i<tabela_simbola.size();i++)
    ofs<<tabela_simbola.at(i)<<endl;

     ofs<<endl<<endl;
     ofs<<"TABELA SEKCIJA"<<endl;

    
    for(int i=0;i<lista_kodova.size();i++){
        if(lista_kodova.at(i).kod=="")
        ofs<<endl<<lista_kodova.at(i).sekcija<<endl;
        else
        ofs<<lista_kodova.at(i).kod<<endl;

    }



        ofs<<endl;
        ofs<<"TABELA RELOKACIJA"<<endl;
        ofs<<"#Sekcija"<<"\t\t"<<"#OFFSET"<<"\t\t"<<"#TIP REL."<<"\t\t\t"<<"#RB"<<"\t\t"<<"#SIMBOL"<<endl;
        for(int i=0;i<lista_relokacija.size();i++)
        ofs<<lista_relokacija.at(i);

}

/*************************************************************************************************************/
//deo za Hex


int Linker::velicina_instrukcje (string masinski_kod){

    int size=0;
    istringstream iss(masinski_kod);
    string bajt;
   
    while(iss>>bajt){
        size++;    
    }
    return size;
}


int Linker::velicina_sekcije(string sekcija){
    int size=0;
    for(int i=0;i<lista_sekcija.size();i++)
    if(lista_sekcija.at(i).naziv==sekcija)
    size+=lista_sekcija.at(i).size;

   return size; 
}


int Linker::nadji_adresu_sekcije(string sekcija){

    // for(int i=0;i<lista_sekcija_adresa->size();i++)
    // if(lista_sekcija_adresa->at(i).sekcija==sekcija){
    //     string adresa=lista_sekcija_adresa->at(i).adresa;
    //     adresa=adresa.substr(2,adresa.size());
    //     return stoul(adresa,nullptr,16);
    // }

    // //nema zadatu pocetnu adresu
    // int pozicija_sekcije;
    // for(int i=0;i<lista_sekcija.size();i++)
    // if(lista_sekcija.at(i).naziv==sekcija){
    // pozicija_sekcije=i;   
    // break;
    // }
    // int adresa=0;
    // int size=0;
    // bool nadjena=false;
    // for(int i=pozicija_sekcije-1;i>=0;i--){
    //     string sekcija=lista_sekcija.at(i).naziv;
    //     for(int j=0;j<lista_sekcija_adresa->size();j++)
    //     if(lista_sekcija_adresa->at(j).sekcija==sekcija && nadjena==false){
    //     string adr=lista_sekcija_adresa->at(j).adresa;
    //     adr=adr.substr(2,adr.size());

    //         adresa=stoul(adr,nullptr,16);
    //         nadjena=true;
    //         break;
    //     }
    //     if(nadjena==false)
    //     size+=lista_sekcija.at(i).size;
        
    //     if(nadjena){
    //     size+=velicina_sekcije(sekcija);
    //     break;
    //     }
    // }

    for(int i=0;i<lista_sekcija_adresa->size();i++)
    if(lista_sekcija_adresa->at(i).sekcija==sekcija){
        string adresa=lista_sekcija_adresa->at(i).adresa;
        adresa=adresa.substr(2,adresa.size());
        int vrednost=stoul(adresa,nullptr,16);
        return vrednost;
    }

    cout<<"Ne postoji u listi sekcija: "<<sekcija<<endl;
    exit(16);

}

bool Linker::nalazi_se_u_sekcija_adresi(string sekcija){

    for(int i=0;i<lista_sekcija_adresa->size();i++)
    if(lista_sekcija_adresa->at(i).sekcija==sekcija)
    return true;

    return false;

}


int Linker::nadji_max_adresu(){
    int max=0;

    for(int i=0;i<lista_sekcija_adresa->size();i++){
        string adr=lista_sekcija_adresa->at(i).adresa;
        adr=adr.substr(2,adr.size());
        int vrednost=stoul(adr,nullptr,16);
        if(max<vrednost)
        max=vrednost;

    }
    return max;
    

}
string Linker:: nadji_sekciju(string adresa){

    for(int i=0;i<lista_sekcija_adresa->size();i++)
    if(lista_sekcija_adresa->at(i).adresa==adresa)
    return lista_sekcija_adresa->at(i).sekcija;

    return "";

}

void Linker::dodaj_ostale_adrese(){
    
    for(int i=0;i<lista_sekcija.size();i++)
        if(!nalazi_se_u_sekcija_adresi(lista_sekcija.at(i).naziv) && lista_sekcija.at(i).naziv!=".ABS"){
            // int adresa=nadji_adresu_sekcije(lista_sekcija.at(i).naziv);
            // string vrednost=obradi_vrednost2(adresa);
            // string adr=obradi_literal2(vrednost);
            // string adressa="0x";
            // adressa.append(adr);    
            // sekcija_adresa sa(lista_sekcija.at(i).naziv,adressa);
            // lista_sekcija_adresa->push_back(sa);
            int adresa=nadji_max_adresu();
            
            string vrednost=obradi_vrednost2(adresa);
            string adr="0x";
            adr.append(obradi_literal2(vrednost));
            string sekcija=nadji_sekciju(adr); //vracamo sekciju sa max adresom
            int size=0;
            if(sekcija!="")
             size=velicina_sekcije(sekcija);//velicina max sekcije
            int nova_adresa=adresa+size;
            vrednost=obradi_vrednost2(nova_adresa);
            adr="0x";
            adr.append(obradi_literal2(vrednost));
            sekcija_adresa sa(lista_sekcija.at(i).naziv,adr);
            lista_sekcija_adresa->push_back(sa);

        }


    
}

void Linker::obradi_hex_format(){
        
            dodaj_ostale_adrese();
           
           

           for(int i=0;i<lista_relokacija.size();i++){
               
               string simbol=lista_relokacija.at(i).simbol;
               string sekcija1=dohvati_simbol(simbol)->sekcija;
               int adresa_sekcije;
               if(sekcija1!=".ABS")
                adresa_sekcije=nadji_adresu_sekcije(sekcija1); 
               
               int offset_simbola=dohvati_simbol(simbol)->offset;
               int nova_vrednost=adresa_sekcije+offset_simbola;
               
               string vrednost=obradi_vrednost2(nova_vrednost);
               string masinski_kod=obradi_literal2(vrednost);
               
               string sekcija2=lista_relokacija.at(i).sekcija;


               int offset=lista_relokacija.at(i).offset;
               int cnt=0;
               for(int j=0;j<lista_kodova.size();j++){

                if(lista_kodova.at(j).sekcija==sekcija2 && lista_kodova.at(j).kod!=""){
                    int velicina_instr=get_broj_bajtova(lista_kodova.at(j).kod);
                    
                    if(cnt+velicina_instr<=offset){
                    cnt+=velicina_instr;

                    }else{ //onda je to taj masinski kod
                    int bajt=offset-cnt;
 

                    
                   
                    string high=masinski_kod.substr(2,4);
                    string low=masinski_kod.substr(0,2);
                    masinski_kod=high.append(low);
                   

                   string novi_masinski_kod=promeni_masinski_kod(lista_kodova.at(j).kod,bajt,masinski_kod);
                   lista_kodova.at(j).kod=novi_masinski_kod;      
                   break;

                    }
                    

                }


            }




           }
            


          

       
                  



}

void Linker::sortiraj_po_adresi(){

    
    for(int i=0;i<lista_sekcija_adresa->size()-1;i++)
    for(int j=i+1;j<lista_sekcija_adresa->size();j++){
    string adri=lista_sekcija_adresa->at(i).adresa;
    adri=adri.substr(2,adri.size());
    int vali=stoul(adri,nullptr,16);

    string adrj=lista_sekcija_adresa->at(j).adresa;
    adrj=adrj.substr(2,adrj.size());
    int valj=stoul(adrj,nullptr,16);
    if(valj<vali){
        sekcija_adresa pom=lista_sekcija_adresa->at(i);
        lista_sekcija_adresa->at(i)=lista_sekcija_adresa->at(j);
        lista_sekcija_adresa->at(j)=pom;
    }


    }
    


}
bool proveri_da_li_je_ispisana_adresa(string adr,vector<string>*lista){
    for(int i=0;i<lista->size();i++)
    if(lista->at(i)==adr)
    return true;

return false;
}

void Linker::ispisi_hex_format(){

    sortiraj_po_adresi();

    vector<string>lista_ispisanih_adresa;    

    ofstream ofs(izlazni_fajl);   
        if(!ofs || !ofs.is_open()){
        cout<<"Greska prilikom otvranja fajla!"<<endl;
        exit(1);
        }
        int cnt=0;
    //  for(int i=0;lista_sekcija_adresa->size();i++)
    //  cout<< lista_sekcija_adresa->at(i).sekcija<<": "<<lista_sekcija_adresa->at(i).adresa<<endl;

    for(int i=0;i<lista_sekcija_adresa->size();i++){
        
        string adresa=lista_sekcija_adresa->at(i).adresa;
        int vrednost_adrese=stoul(adresa.substr(2,adresa.size()),nullptr,16);
        if(vrednost_adrese%8!=0){//vrednost adrese nije deljiva sa 8
            int adresapom=vrednost_adrese;
            while(adresapom%8!=0)
            adresapom--;
            string v1=obradi_vrednost2(adresapom);
            string adr1="0x";
            bool smer=false;
            

            adr1.append(obradi_literal2(v1));
            if(proveri_da_li_je_ispisana_adresa(adr1,&lista_ispisanih_adresa)){
                    adresapom=vrednost_adrese;
                    while(adresapom%8!=0)
                    adresapom++;
                    smer=true;
                    adr1="0x";
                    v1=obradi_vrednost2(adresapom);
                    adr1.append(obradi_literal2(v1));

            }
            ofs<<adr1<<": ";
            while(adresapom!=vrednost_adrese){
            ofs<<"00"<<" ";
            cnt++;
            if(!smer)
            adresapom++;
            else
            adresapom--;

            }

        }    

        for(int j=0;j<lista_kodova.size();j++){
            
        if(lista_kodova.at(j).sekcija==lista_sekcija_adresa->at(i).sekcija){

                if(lista_kodova.at(j).kod!=""){
                string kod=lista_kodova.at(j).kod;
                
                istringstream iss(kod);
                string token;
                while(iss>>token){
                        
                          if(cnt==0 ){
                              string v2=obradi_vrednost2(vrednost_adrese);
                              string adr2="0x";
                              adr2.append(obradi_literal2(v2));
                              lista_ispisanih_adresa.push_back(adr2);
                              ofs<<adr2<<": ";
                              
                          }
                           
                          ofs<<token<<" ";
                          if(cnt==7)
                          ofs<<endl;
                          cnt=(cnt+1)%8;
                          vrednost_adrese++;
                      


                }
                }

        }
    }
     while(cnt!=0){
         ofs<<"00"<<" ";
         cnt=(cnt+1)%8;
         vrednost_adrese++;
         if(cnt==0)
         ofs<<endl;
     }   
    

    }
    while(cnt!=8 ){
        if(cnt==0)
        break;
        ofs<<"00"<<" ";
        cnt=cnt+1;
    }

}



