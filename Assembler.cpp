#include "Assembler.hpp"

const map<string, regex> Assembler::tokenMapa{
    {"SEKCIJA", regex("^\\.section$")},
    {"SIMBOL", regex("^[a-zA-Z_][a-zA-Z0-9_]*$")},
    {"LABELA", regex("^([a-zA-Z_][a-zA-Z0-9_]*):$")},
    {"INSTRUKCIJA", regex("^(halt|int|iret|call|ret|jmp|jeq|jne|jgt|push|pop|xchg|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr|ldr|str)$")},
    {"KRAJ", regex("^\\.end$")},
    {"DIREKTIVA", regex("^\\.(global|extern|word|skip|equ)$")},
    {"LITERAL", regex("^([0-9]+)|\"([a-zA-Z!@#$\\\\]+)\"|(0x[0-9A-F]+)$")},
    {"$LITERAL", regex("^(\\$[0-9]+)|\\$\"([a-zA-Z!@#$%^&\\\\]+)\"|(\\$0x[0-9A-F]+)$")},
    {"$SIMBOL", regex("^\\$[a-zA-Z_][a-zA-Z0-9_]*$")},
    {"REGISTAR", regex("^r[0-7]$")},
    {"[REGISTAR]", regex("^\\[r[0-7]\\]$")},
    {"%SIMBOL", regex("^%[a-zA-Z_][a-zA-Z0-9_]*$")},
    {"*LITERAL", regex("^(\\*[0-9]+)|\\*\"([a-zA-Z@!#$\\\\]+)\"|(\\*0x[0-9A-F]+) $")},
    {"*SIMBOL", regex("^\\*[a-zA-Z_][a-zA-Z0-9_]*$")},
    {"*REGISTAR", regex("^\\*r[0-7]$")},
    {"*[REGISTAR]", regex("^\\*\\[r[0-7]\\]$")},
    {"KOMENTAR", regex("^#[0-9a-zA-Z!@#$%^&*]*$")}

};

const map<InstructionCode, string> Assembler::mapa_kodova{
    {HALT, "halt"},
    {INT, "int"},
    {IRET, "iret"},
    {CALL, "call"},
    {RET, "ret"},
    {JMP, "jmp"},
    {JEQ, "jeq"},
    {JNE, "jne"},
    {JGT, "jgt"},
    {XCHG, "xchg"},
    {ADD, "add"},
    {SUB, "sub"},
    {MUL, "mul"},
    {DIV, "div"},
    {CMP, "cmp"},
    {NOT, "not"},
    {AND, "and"},
    {OR, "or"},
    {XOR, "xor"},
    {TEST, "test"},
    {SHL, "shl"},
    {SHR, "shr"},
    {LDR, "ldr"},
    {STR, "str"}

};

Assembler::Assembler(string ulaz, string izlaz)
{

    this->ulazni_fajl = ulaz;
    this->izlazni_fajl = izlaz;
}

InstructionCode Assembler::get_kod_instrukcije(string instr)
{

    for (auto &par : mapa_kodova)
        if (par.second == instr)
            return par.first;

    cout << "Ne postoji instrukcijski kod za instrukciju: " << instr << endl;
    exit(17);
}

string Assembler::tip_tokena(string token)
{

    string tip = "GRESKA";
    for (auto &par : tokenMapa)
        if (regex_match(token, par.second))
        {
            tip = par.first;
            break;
        }
    return tip;
}

bool Assembler::sekcija_u_tabeli(string sekcija)
{

    for (int i = 0; i < tabela_simbola.size(); i++)
        if (tabela_simbola.at(i).naziv == sekcija)
            return true;

    return false;
}

bool Assembler::sekcija_sintaksa(string sekcija)
{
    regex r("^.[a-zA-Z_][a-zA-Z_0-9]*$");
    if (regex_match(sekcija, r))
        return true;
    else
        return false;
}

void Assembler::dodaj_simbol(string simbol, string sekcija, int location)
{

    for (int i = 0; i < tabela_simbola.size(); i++)
        if (tabela_simbola.at(i).naziv == simbol)
        {

            cout << "Simbol: " << simbol << "vec postoji u tabeli!";
            exit(4);
        }

    Symbol s(tabela_simbola.size(), simbol, sekcija, location);
    if (sekcija == "?")
        s.is_global = true;

    tabela_simbola.push_back(s);
}
bool Assembler::dva_registra(string instrukcija)
{

    if (instrukcija == "add" || instrukcija == "sub" || instrukcija == "mul" || instrukcija == "div" || instrukcija == "xchg" || instrukcija == "cmp" || instrukcija == "and" || instrukcija == "or" || instrukcija == "xor" || instrukcija == "test" || instrukcija == "shr" || instrukcija == "shl")
        return true;

    return false;
}

bool Assembler::simbol_u_tabeli(string simbol)
{

    for (int i = 0; i < tabela_simbola.size(); i++)
        if (tabela_simbola.at(i).naziv == simbol)
            return true;

    return false;
}

bool Assembler::da_li_je_registar(string reg)
{
    regex r("^r[0-7]$");
    if (regex_match(reg, r))
        return true;

    return false;
}

int Assembler::nadji_velicinu(string instrukcija)
{
    int b;

    if (instrukcija == "add" || instrukcija == "sub" || instrukcija == "div" || instrukcija == "mul" || instrukcija == "cmp" || instrukcija == "not" || instrukcija == "and" || instrukcija == "or" || instrukcija == "xor" || instrukcija == "test" || instrukcija == "shl" || instrukcija == "shr" || instrukcija == "ldr" || instrukcija == "str")
        b = 2;
    else if (instrukcija == "push" || instrukcija == "pop")
        b = 3;
    else
        b = 1;

    return b;
}
bool Assembler::jedan_registar(string instrukcija)
{
    if (instrukcija == "int" || instrukcija == "push" || instrukcija == "pop" || instrukcija == "not")
        return true;
    else
        return false;
}
bool Assembler::nula_adresna(string instrukcija)
{
    if (instrukcija == "halt" || instrukcija == "iret" || instrukcija == "ret")
        return true;
    else
        return false;
}
bool Assembler::instrukcija_skoka(string instrukcija)
{

    if (instrukcija == "jmp" || instrukcija == "jeq" || instrukcija == "jne" || instrukcija == "call" || instrukcija == "jgt")
        return true;

    return false;
}

void Assembler::prvi_prolaz()
{

    int location_counter = 0;
    string line;
    string token;
    string tip;
    ifstream ifs(this->ulazni_fajl);

    Section trenutna_sekcija("null");

    if (!ifs || !ifs.is_open())
    {
        cout << "Greska prilikom otvranja fajla!" << endl;
        exit(1);
    }
    while (getline(ifs, line))
    {

        istringstream tok(line);

        while (tok >> token)
        {
            tip = tip_tokena(token);

             

               

            if (tip == "SEKCIJA")
            {

                string ime_sekcije;
                tok >> ime_sekcije;
                if (!sekcija_sintaksa(ime_sekcije))
                {
                    cout << __LINE__<<" : Sekcija ne sme tako da se naziva!" << endl;
                    exit(1);
                }

                if (sekcija_u_tabeli(ime_sekcije))
                {
                    cout << __LINE__<< ": Sekcija ne sme da se ponavlja!" << endl;
                    exit(3);
                }

                trenutna_sekcija.naziv = ime_sekcije;
                location_counter = 0;
                dodaj_simbol(ime_sekcije, trenutna_sekcija.naziv, location_counter);
            }else if (tip == "LABELA")
            {

                if (trenutna_sekcija.naziv == "null")
                {
                    cout <<__LINE__<< ": Labela mora biti unutar sekcije: "<<token << endl;
                    exit(5);
                }
                string naziv = token.substr(0, token.size() - 1); //da bi izbacili :

                dodaj_simbol(naziv, trenutna_sekcija.naziv, location_counter);
            }
            else if (tip == "INSTRUKCIJA")
            {

                if (dva_registra(token))
                {

                    string registar1;
                    string registar2;
                    tok >> registar1;
                    if (registar1 == "" || registar1.back() != ',')
                    {
                        cout << __LINE__<< ": Sintaksna greska!" << endl;
                        exit(6);
                    }
                    registar1 = registar1.substr(0, registar1.size() - 1);
                    tok >> registar2;

                    if (registar2 == "" || registar2.back() == ',')
                    {
                        cout <<__LINE__<< ": Sintaksna greska!" << endl;
                        exit(6);
                    }
                    if (!da_li_je_registar(registar1) || !da_li_je_registar(registar2))
                    {
                        cout <<__LINE__<<" : "<< token <<" instrukcija koristi 2 registra opste namene!(r0-r7)" << endl;
                        exit(7);
                    }

                    location_counter += nadji_velicinu(token);
                }
                else if (jedan_registar(token))
                {
                    string registar;
                    tok >> registar;
                    if (registar == "")
                    {
                        cout << __LINE__<<" :Sintaksna greska!" << endl;
                        exit(8);
                    }
                    if (!da_li_je_registar(registar))
                    {
                        cout << __LINE__<<" :"<<token << " instrukcija koristi jedan registar opste namene(r0-r7)!" << endl;
                        exit(9);
                    }

                    location_counter += nadji_velicinu(token);
                }
                else if (nula_adresna(token))
                {
                    string novo;
                    tok >> novo;
                    if (novo != "")
                    {
                        cout <<__LINE__<<": "<< token << " instrukcija je nula adresna!" << endl;
                        exit(9);
                    }
                    location_counter += nadji_velicinu(token);
                }
                else
                {

                    /*instrukcije sa odredjenim nacinom adresiranja!
                            (ili operand ili operand i registar)
                        */
                    if (!instrukcija_skoka(token))
                    {
                        //ldr i str instrukcije

                        string registar;
                        string operand;
                        tok >> registar;
                        if (!registar.back() == ',')
                        {
                            cout << __LINE__<<" :"<<"Sintaksna greska za instrukciju:" << token << endl;
                            exit(13);
                        }
                        registar = registar.substr(0, registar.size() - 1);
                        if (!da_li_je_registar(registar))
                        {
                            cout << __LINE__<<" :Prvi operand mora biti registar (r0-r7)! za instrukciju" << token << endl;
                            exit(14);
                        }
                        //prvi operand je def. registar

                        tok >> operand;
                        int velicina;
                        if (tip_tokena(operand) == "REGISTAR" || tip_tokena(operand) == "[REGISTAR]")
                        {
                            velicina = 3;
                        }
                        else if (tip_tokena(operand) == "$LITERAL" || tip_tokena(operand) == "$SIMBOL" || tip_tokena(operand) == "LITERAL" || tip_tokena(operand) == "SIMBOL" || tip_tokena(operand) == "%SIMBOL")
                        {
                            velicina = 5;
                        }
                        else
                        {
                            //registar + literal ili registar + simbol
                            if (operand.at(0) == '[')
                            {
                                operand = operand.substr(1, operand.size());
                                if (da_li_je_registar(operand))
                                {
                                    tok >> operand;
                                    if (operand == "+")
                                    {
                                        tok >> operand;
                                        if (operand.back() == ']')
                                        {
                                            operand = operand.substr(0, operand.size() - 1);

                                            if (tip_tokena(operand) == "SIMBOL" || tip_tokena(operand) == "LITERAL")
                                            {
                                                velicina = 5;
                                            }
                                            else
                                            {
                                                cout << __LINE__<<" :Sintaksna greska za instrukciju " << token << endl;
                                                exit(15);
                                            }
                                        }
                                        else
                                        {
                                            cout <<__LINE__<< " :Sintaksna greska za instrukciju " << token << endl;
                                            exit(15);
                                        }
                                    }
                                    else
                                    {
                                        cout << __LINE__<<" :Sintaksna greska za instrukciju " << token << endl;
                                        exit(15);
                                    }
                                }
                                else
                                {
                                    cout << __LINE__<< " :Sintaksna greska za instrukciju " << token << endl;
                                    exit(15);
                                }
                            }
                            else
                            {
                                cout << __LINE__<< " :Sintaksna greska za instrukciju " << token << endl;
                                exit(15);
                            }
                        }
                        location_counter += velicina;
                    }
                    else
                    { //jeste instrukcija skoka!

                        string operand;
                        tok >> operand;
                        int velicina;
                        if (tip_tokena(operand) == "*REGISTAR" || tip_tokena(operand) == "*[REGISTAR]")
                        {
                            velicina = 3;
                        }
                        else if (tip_tokena(operand) == "LITERAL" || tip_tokena(operand) == "SIMBOL" || tip_tokena(operand) == "%SIMBOL" || tip_tokena(operand) == "*LITERAL" || tip_tokena(operand) == "*SIMBOL")
                        {
                            velicina = 5;
                        }
                        else
                        {
                            //onda je verovatno *[reg + literal] ili *[reg + simbol]
                            if (operand.at(0) == '*' && operand.at(1) == '[')
                            {
                                operand = operand.substr(2, operand.size());

                                if (da_li_je_registar(operand))
                                {
                                    tok >> operand;

                                    if (operand == "+")
                                    {
                                        tok >> operand;
                                        if (operand.back() == ']')
                                        {
                                            operand = operand.substr(0, operand.size() - 1);
                                            if (tip_tokena(operand) == "SIMBOL" || tip_tokena(operand) == "LITERAL")
                                            {
                                                velicina = 5;
                                            }
                                            else
                                            {
                                                cout << __LINE__<< " :Sintaksna greska za instrukciju " << token << endl;
                                                exit(15);
                                            }
                                        }
                                        else
                                        {
                                            cout << __LINE__<<" :Sintaksna greska za instrukciju " << token << endl;
                                            exit(15);
                                        }
                                    }
                                    else
                                    {
                                        cout << __LINE__<<" :Sintaksna greska za instrukciju " << token << endl;
                                        exit(15);
                                    }
                                }
                                else
                                {
                                    cout << __LINE__<<" :Sintaksna greska za instrukciju " << token << endl;
                                    exit(15);
                                }
                            }
                            else
                            {
                                cout << __LINE__<<" :Sintaksna greska za instrukciju " << token << endl;
                                exit(15);
                            }
                        }

                        location_counter += velicina;
                    }
                }
            }
            else if (tip == "KRAJ")
            {

                return;
            }
            else if (tip == "DIREKTIVA")
            {

                if (token == ".word")
                {

                    string vrednost;
                    tok >> vrednost;
                    int broj_simbola = 1;
                    while (vrednost.back() == ',')
                    {
                        vrednost = vrednost.substr(0, vrednost.size() - 1);
                        if (tip_tokena(vrednost) != "LITERAL" && tip_tokena(vrednost) != "SIMBOL")
                        {
                            cout << __LINE__<<" :"<<token << " zahteva ili literal ili simbol!" << endl;
                            exit(10);
                        }
                        broj_simbola++;
                        tok >> vrednost;
                    }
                    if (tip_tokena(vrednost) != "LITERAL" && tip_tokena(vrednost) != "SIMBOL")
                    {
                        cout << __LINE__<<" :"<<token << " zahteva ili literal ili simbol!" << endl;
                        exit(10);
                    }
                    location_counter += broj_simbola * 2;
                }
                else if (token == ".skip")
                {

                    string vrednost;
                    int bytes;

                    tok >> vrednost;
                  
                    if (tip_tokena(vrednost) != "LITERAL")
                    {
                        cout <<__LINE__<< " :.skip radi samo sa jednim literalom!" << endl;
                        exit(11);
                    }
                    if (isdigit(vrednost.at(0))) // provera da li je prvi znak "
                        bytes = stoi(vrednost);
                    else
                    {
                        vrednost = vrednost.substr(1, vrednost.size() - 1);

                        bytes = vrednost.size();
                    }
                    location_counter += bytes;
                }
                else if (token == ".equ")
                {

                    string novi_simbol;
                    string literal;
                    tok >> novi_simbol;
                    if (novi_simbol.back() == ',')
                        novi_simbol = novi_simbol.substr(0, novi_simbol.size() - 1);
                    else
                    {
                        cout << __LINE__<<" :"<<token << " sintaksna greska!" << endl;
                        exit(10);
                    }
                    if (tip_tokena(novi_simbol) != "SIMBOL")
                    {
                        cout << __LINE__<< " :.equ zahteva simbol i literal!" << endl;
                        exit(11);
                    }
                    tok >> literal;
                    if (tip_tokena(literal) != "LITERAL")
                    {
                        cout << __LINE__<<" :.equ zahteva simbol i literal!" << endl;
                        exit(11);
                    }
                    int vrednost_literala;

                    if (literal.at(0) == '0' && literal.at(1) == 'x')
                    { // provera za hexa vrednosti
                        vrednost_literala = stoul(literal, nullptr, 16);
                    }
                    else
                    {
                        if (isdigit(literal.at(0)))
                            vrednost_literala = stoi(literal);
                        else
                        {
                            literal = literal.substr(1, literal.size() - 1);
                            vrednost_literala = literal.size();
                            if (literal.find('\\'))
                            {
                                vrednost_literala -= 1;
                            }
                        }
                    }
                    // if (trenutna_sekcija.naziv == "null")
                    // {
                    //     cout <<__LINE__<< " :direktiva .equ mora biti u okviru sekcije!" << endl;
                    //     exit(11);
                    // }

                    dodaj_simbol(novi_simbol, ".ABS", vrednost_literala);
                    // if(!sekcija_u_tabeli_sekcija("ABS"))
                    //   tabela_sekcija.push_back(Section("ABS"));
                }
                else if (token == ".extern")
                {
                    string simbol;

                    tok >> simbol;

                    while (simbol.back() == ',')
                    {

                        simbol = simbol.substr(0, simbol.size() - 1);
                        if (tip_tokena(simbol) != "SIMBOL")
                        {
                            cout << __LINE__<< " :.extern radi samo sa simbolima!" << endl;
                            exit(12);
                        }
                        if (!simbol_u_tabeli(simbol))
                            dodaj_simbol(simbol, "?", -1);
                        tok >> simbol;
                    }

                    if (tip_tokena(simbol) != "SIMBOL")
                    {
                        cout << __LINE__<<" :.extern radi samo sa simbolima!" << endl;
                        exit(12);
                    }
                    if (!simbol_u_tabeli(simbol))
                        dodaj_simbol(simbol, "?", -1);
                }
                else
                { // .global

                    string nesto_novo;
                    string novo;
                    tok >> nesto_novo;
                    while (nesto_novo.back() == ',')
                    {
                        tok >> nesto_novo;
                    }

                    tok >> novo;
                    if(tip_tokena(novo)=="KOMENTAR")
                    break;

                    if (novo != "")
                    {
                        cout << __LINE__<<" :Samo jedna naredba je dozvoljena po liniji!" << endl;
                        exit(16);
                    }

                    break;
                }
            }
            else if (tip == "KRAJ")
            {

                return;
            }
            else if (tip == "KOMENTAR")
            {
                string random;
                tok>>random;
                
                break;
            }else{
                        cout<<__LINE__<<": nepoznat token: "<<token<<endl;
                        exit(16);
            }

            if(tip!="LABELA"){ //moze i u produzetku da se nesto napise    
           

            string jos_nesto;
            tok >> jos_nesto;

            
            

            if (jos_nesto != "" && jos_nesto.at(0) != '#')
            {   
                if(tip_tokena(jos_nesto)!="INSTRUKCIJA" && tip_tokena(jos_nesto)!="LABELA" 
                && tip_tokena(jos_nesto)!="DIREKTIVA" && tip_tokena(jos_nesto)!="SEKCIJA" ){
                    cout<<__LINE__<<": nepoznat token: "<<jos_nesto<<endl;
                    exit(16);
                }    
                
                cout <<__LINE__<< " :Samo jedna naredba je dozvoljena po liniji!" << endl;
                exit(16);
            }else if(jos_nesto=="#"){
                break;
            }
            

            }
        }
    }
}

Symbol *Assembler::pronadji_simbol(string name)
{

    for (int i = 0; i < tabela_simbola.size(); i++)
        if (tabela_simbola.at(i).naziv == name)
            return &tabela_simbola.at(i);

    return nullptr;
}

bool Assembler::sekcija_u_tabeli_sekcija(string naziv)
{

    for (int i = 0; i < tabela_sekcija.size(); i++)
        if (tabela_sekcija.at(i).naziv == naziv)
            return true;

    return false;
}
Section *Assembler::dohvati_sekciju(string naziv)
{

    for (int i = 0; i < tabela_sekcija.size(); i++)
        if (tabela_sekcija.at(i).naziv == naziv)
            return &tabela_sekcija.at(i);

    cout << __LINE__<<" :Fatalna greska: sekcija nije u tabeli?" << endl;
    exit(16);
}

void Assembler::drugi_prolaz()
{

    int location_counter = 0;
    string line;
    string token;
    string tip;
    ifstream ifs(this->ulazni_fajl);
    string ime_sekcije;
    if (!ifs || !ifs.is_open())
    {
        cout << __LINE__<< ": Greska prilikom otvranja fajla!" << endl;
        exit(1);
    }
    while (getline(ifs, line))
    {

        istringstream tok(line);
        while (tok >> token)
        {

            tip = tip_tokena(token);
            if (tip == "DIREKTIVA")
            {

                if (token == ".global")
                {

                    string simbol;
                    tok >> simbol;

                    while (simbol.back() == ',')
                    {
                        simbol = simbol.substr(0, simbol.size() - 1);

                        if (tip_tokena(simbol) != "SIMBOL")
                        {
                            cout <<__LINE__<< " :.global zahteva navodjenje simbola ili liste simbola!" << endl;
                            exit(16);
                        }

                        Symbol *sim = pronadji_simbol(simbol);
                        if (sim == nullptr)
                        {
                            cout << __LINE__<< ": Simbol" << simbol << " ne postoji u tabeli simbola!" << endl;
                            exit(17);
                        }

                        sim->is_global = true;
                        tok >> simbol;
                    }

                    if (tip_tokena(simbol) != "SIMBOL")
                    {
                        cout <<__LINE__<< " :.global zahteva navodjenje simbola ili liste simbola!" << endl;
                        exit(16);
                    }

                    Symbol *sim = pronadji_simbol(simbol);
                    if (sim == nullptr)
                    {
                        cout << __LINE__<<" :Simbol " << simbol << " ne postoji u tabeli simbola!" << endl;
                        exit(17);
                    }

                    sim->is_global = true;
                }
                else if (token == ".word")
                {

                    Section *s = dohvati_sekciju(ime_sekcije);
                    int broj = 1;
                    string podatak;
                    tok >> podatak;

                    while (podatak.back() == ',')
                    {
                        podatak = podatak.substr(0, podatak.size() - 1);

                        if (tip_tokena(podatak) == "LITERAL")
                        {
                            int vrednost;
                            if (podatak.at(0) == '0' && podatak.at(1) == 'x')
                            { // hexa vrednost
                                vrednost = stoul(podatak, nullptr, 16);
                            }
                            else
                                vrednost = stoi(podatak);

                            s->dodaj_element(vrednost, "word_LITERAL");
                        }
                        else
                        { //SIMBOL
                            /*Apsolutna relokacija!*/
                            int offset = location_counter + (broj - 1) * 2;
                            int vrednost = -1;
                            int RB;

                            for (int i = 0; i < tabela_simbola.size(); i++)
                                if (tabela_simbola.at(i).naziv == podatak)
                                {
                                    vrednost = tabela_simbola.at(i).offset;
                                    RB = tabela_simbola.at(i).RB;
                                }
                            if (vrednost == -1)
                            {
                                RB = tabela_simbola.size();
                                if (!simbol_u_tabeli(podatak))
                                    dodaj_simbol(podatak, "?", -1);
                            }
                            else
                            {
                                // for(int i=0;i<tabela_simbola.size();i++)
                                //         if(tabela_simbola.at(i).naziv==s->naziv)
                                //         RB=tabela_simbola.at(i).RB;

                                if(pronadji_simbol(podatak)->is_global){
                                RB=pronadji_simbol(podatak)->RB;
                                vrednost=0;
                                }
                                else{
                                string sekcija = pronadji_simbol(podatak)->sekcija;
                                if (sekcija != ".ABS")
                                    RB = pronadji_simbol(sekcija)->RB;
                                }
                            }
                            if (pronadji_simbol(podatak)->sekcija != ".ABS")
                            {
                                Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, podatak);
                                tabela_relokacija.push_back(r);
                            }
                            s->dodaj_element(vrednost, "word_SIMBOL");
                        }

                        tok >> podatak;

                        broj++;
                    }
                    if (tip_tokena(podatak) == "LITERAL")
                    {
                        int vrednost;

                        if (podatak.at(0) == '0' && podatak.size() != 1)
                        { // hexa vrednost
                            vrednost = stoul(podatak, nullptr, 16);
                        }
                        else
                            vrednost = stoi(podatak);

                        s->dodaj_element(vrednost, "word_LITERAL");
                    }
                    else
                    {   //SIMBOL
                        /*Apsolutna relokacija!*/
                        int offset = location_counter + (broj - 1) * 2;
                        int vrednost = -1;
                        int RB;
                        for (int i = 0; i < tabela_simbola.size(); i++)
                            if (tabela_simbola.at(i).naziv == podatak)
                            {
                                vrednost = tabela_simbola.at(i).offset;
                                RB = tabela_simbola.at(i).RB;
                            }
                        if (vrednost == -1)
                        {
                            RB = tabela_simbola.size();
                            if (!simbol_u_tabeli(podatak))
                                dodaj_simbol(podatak, "?", -1);
                        }
                        else
                        {
                            //  for(int i=0;i<tabela_simbola.size();i++)
                            //         if(tabela_simbola.at(i).naziv==s->naziv)
                            //         RB=tabela_simbola.at(i).RB;

                            if(pronadji_simbol(podatak)->is_global){
                            RB=pronadji_simbol(podatak)->RB;
                            vrednost=0;
                            }
                            else{
                            string sekcija = pronadji_simbol(podatak)->sekcija;
                            if (sekcija != ".ABS")
                                RB = pronadji_simbol(sekcija)->RB;
                            }
                        }
                        if (pronadji_simbol(podatak)->sekcija != ".ABS")
                        {
                            Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, podatak);
                            tabela_relokacija.push_back(r);
                        }
                        s->dodaj_element(vrednost, "word_SIMBOL");
                    }

                    location_counter += broj * 2;
                }
                else if (token == ".skip")
                {

                    Section *s = dohvati_sekciju(ime_sekcije);
                    int vrednost;
                    string podatak;
                    tok >> podatak;
                    if (podatak.at(0) == '0' && podatak.size() != 1)
                    {
                        podatak = podatak.substr(2, podatak.size());
                        vrednost = stoul(podatak, nullptr, 16);
                    }
                    else
                        vrednost = stoi(podatak);

                    location_counter += vrednost;
                    s->dodaj_element(vrednost, ".skip");
                }
            }
            else if (tip == "SEKCIJA")
            {

                tok >> ime_sekcije;
                if (!sekcija_u_tabeli_sekcija(ime_sekcije))
                    tabela_sekcija.push_back(Section(ime_sekcije));
                location_counter = 0;
            }
            else if (tip == "INSTRUKCIJA")
            {
                InstructionCode instr_kod;
                if (token != "push" && token != "pop")
                    instr_kod = get_kod_instrukcije(token);
                if (token == "push")
                    instr_kod = get_kod_instrukcije("str");
                if (token == "pop")
                    instr_kod = get_kod_instrukcije("ldr");

                Section *s = dohvati_sekciju(ime_sekcije);

                if (dva_registra(token))
                {
                    s->dodaj_element(instr_kod, "instr_dva_registra");
                    string prvi_registar;
                    string drugi_registar;
                    tok >> prvi_registar;
                    prvi_registar = prvi_registar.substr(1, prvi_registar.size() - 1); // da bi skinuo , i r
                    tok >> drugi_registar;
                    drugi_registar = drugi_registar.substr(1, drugi_registar.size()); // da bi skinuo r

                    s->dodaj_element(stoi(prvi_registar), "registar");
                    s->dodaj_element(stoi(drugi_registar), "registar");

                    location_counter += 2;
                }
                else if (jedan_registar(token))
                { //za push i pop nemamo operacioni kod?

                    /*Deo za push i pop*/

                    if (token == "push" || token == "pop")
                    {
                        s->dodaj_element(instr_kod, token); // push ili pop
                        string reg;
                        tok >> reg;
                        reg = reg.substr(1, reg.size());
                        s->dodaj_element(stoi(reg), "registar");
                        location_counter += 3;
                    }
                    else
                    {

                        s->dodaj_element(instr_kod, "instr_jedan_registar");

                        string registar;
                        tok >> registar;
                        registar = registar.substr(1, registar.size());
                        s->dodaj_element(stoi(registar), "registar");
                        location_counter += 2;
                    }
                }
                else if (nula_adresna(token))
                { //halt ret iret

                    s->dodaj_element(instr_kod, "nula-adresna");
                    location_counter += 1;
                }
                else
                { //instrukcije koje koriste operande

                    if (!instrukcija_skoka(token))
                    { //ldr ili str
                        InstructionCode instr_kod = get_kod_instrukcije(token);
                        string registar;

                        tok >> registar;
                        registar = registar.substr(1, registar.size() - 2); //tj redni broj registra

                        string operand;
                        tok >> operand;

                        if (tip_tokena(operand) == "$LITERAL" || tip_tokena(operand) == "$SIMBOL")
                        { // neposredno adresiranje

                            operand = operand.substr(1, operand.size()); // da bi skinuli $
                            if (tip_tokena(operand) == "LITERAL")
                            {
                                int vrednost;
                                if (operand.at(0) == '0' && operand.at(1) == 'x')
                                { // hexa vrednost
                                    operand = operand.substr(2, operand.size());
                                    vrednost = stoul(operand, nullptr, 16);
                                }
                                else
                                { // decimalna
                                    vrednost = stoi(operand);
                                }
                                s->dodaj_element(instr_kod, "ldr_str_immediate");

                                s->dodaj_element(stoi(registar), "registar");
                                s->dodaj_element(vrednost, "literal");
                            }
                            else
                            { // onda je simbol
                                int vrednost = -1;
                                for (int i = 0; i < tabela_simbola.size(); i++)
                                    if (tabela_simbola.at(i).naziv == operand)
                                        vrednost = tabela_simbola.at(i).offset;
                                if (vrednost == -1)
                                {
                                    /*Deo za relokaciju jer nema simbola u tabeli*/

                                    int offset = location_counter + 3;
                                    int RB;

                                    if (!pronadji_simbol(operand))
                                    {
                                        dodaj_simbol(operand, "?", -1);
                                        RB = tabela_simbola.size() - 1;
                                    }
                                    else
                                    {
                                        Symbol *sym = pronadji_simbol(operand);
                                        RB = sym->RB;
                                    }
                                    Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, operand);
                                    tabela_relokacija.push_back(r);
                                }
                                else
                                { //Ima simbola al svakako apsoultna relokacija!
                                    Symbol *sym = pronadji_simbol(operand);
                                    string sekcija = sym->sekcija;
                                    int offset = location_counter + 3;
                                    int RB;
                                    if (sekcija != ".ABS")
                                        RB = pronadji_simbol(sekcija)->RB;

                                    if (pronadji_simbol(operand)->sekcija != ".ABS")
                                    {
                                        Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, operand);
                                        tabela_relokacija.push_back(r);
                                    }
                                }

                                s->dodaj_element(instr_kod, "ldr_str_immediate");

                                s->dodaj_element(stoi(registar), "registar");
                                s->dodaj_element(vrednost, "literal");
                            }
                            location_counter += 5;
                        }
                        else if (tip_tokena(operand) == "LITERAL" || tip_tokena(operand) == "SIMBOL")
                        { // memorijsko adresiranje

                            if (tip_tokena(operand) == "LITERAL")
                            {
                                int vrednost;
                                if (operand.at(0) == '0' && operand.at(1) == 'x')
                                { // hexa vrednost
                                    operand = operand.substr(2, operand.size());
                                    vrednost = stoul(operand, nullptr, 16);
                                }
                                else
                                { // decimalna
                                    vrednost = stoi(operand);
                                }

                                s->dodaj_element(instr_kod, "ldr_str_mem");

                                s->dodaj_element(stoi(registar), "registar");
                                s->dodaj_element(vrednost, "literal");
                            }
                            else
                            { // SIMBOL
                                int vrednost = -1;
                                for (int i = 0; i < tabela_simbola.size(); i++)
                                    if (tabela_simbola.at(i).naziv == operand)
                                        vrednost = tabela_simbola.at(i).offset;
                                int offset = location_counter + 3;
                                int RB;
                                if (vrednost == -1)
                                {
                                    /*Deo za relokaciju jer nema simbola u tabeli*/

                                    if (!pronadji_simbol(operand))
                                    {
                                        dodaj_simbol(operand, "?", -1);
                                        RB = tabela_simbola.size() - 1;
                                    }
                                    else
                                    {
                                        Symbol *sym = pronadji_simbol(operand);
                                        RB = sym->RB;
                                    }
                                }
                                else
                                {
                                    Symbol *sym2 = pronadji_simbol(operand);
                                    string sekcija = sym2->sekcija;
                                    if (sekcija != ".ABS")
                                        RB = pronadji_simbol(sekcija)->RB;
                                    if (sym2->is_global)
                                        RB = sym2->RB;
                                }

                                if (pronadji_simbol(operand)->sekcija != ".ABS")
                                {
                                    Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, operand);
                                    tabela_relokacija.push_back(r);
                                }

                                //else{ //Ima simbola al svakako apsoultna relokacija!
                                //     Symbol*sym=pronadji_simbol(operand);
                                //     string sekcija=sym->sekcija;
                                //     int offset=location_counter+3;
                                //     int RB=pronadji_simbol(sekcija)->RB;
                                //     Relocation r(ime_sekcije,offset,"R_X86_64_16",RB,operand);
                                //     tabela_relokacija.push_back(r);

                                // }

                                s->dodaj_element(instr_kod, "ldr_str_mem");

                                s->dodaj_element(stoi(registar), "registar");
                                s->dodaj_element(vrednost, "literal");
                            }
                            location_counter += 5;
                        }
                        else if (tip_tokena(operand) == "REGISTAR" || tip_tokena(operand) == "[REGISTAR]")
                        { //registarsko dir i indir.

                            if (tip_tokena(operand) == "REGISTAR")
                                s->dodaj_element(instr_kod, "ldr_str_regdir");
                            else
                                s->dodaj_element(instr_kod, "ldr_str_regindir");

                            if (tip_tokena(operand) == "REGISTAR")
                                operand = operand.substr(1, operand.size());     // da skinemo r
                            else                                                 // [REGISTAR]
                                operand = operand.substr(2, operand.size() - 1); // da skinemo [r]

                            s->dodaj_element(stoi(registar), "registar");
                            s->dodaj_element(stoi(operand), "registar");

                            location_counter += 3;
                        }
                        else if (tip_tokena(operand) == "%SIMBOL")
                        { //PC relativno

                            operand = operand.substr(1, operand.size()); // da bi skinuo %
                            int vrednost = -1;
                            int RB;
                            for (int i = 0; i < tabela_simbola.size(); i++)
                                if (tabela_simbola.at(i).naziv == operand)
                                {
                                    vrednost = tabela_simbola.at(i).offset;
                                    RB = tabela_simbola.at(i).RB;
                                }
                            /*Relativna relokacija?*/

                            int offset = location_counter + 3;
                            Symbol *sym;
                            if (vrednost == -1)
                            {
                                if (!simbol_u_tabeli(operand))
                                {
                                    dodaj_simbol(operand, "?", -1);
                                    RB = tabela_simbola.size() - 1;
                                }
                                else
                                {

                                    sym = pronadji_simbol(operand);
                                    if (sym->is_global)
                                        RB = sym->RB;
                                    else
                                    {
                                        string sekcija = sym->sekcija;
                                        if (sekcija != ".ABS")
                                            RB = pronadji_simbol(sekcija)->RB;
                                    }
                                }
                            }
                            //  if(vrednost!=-1||sym->is_global){
                            //      for(int i=0;i<tabela_simbola.size();i++)
                            //     if(tabela_simbola.at(i).naziv==s->naziv)
                            //      RB=tabela_simbola.at(i).RB;
                            //  }
                            
                            
                                if (pronadji_simbol(operand)->sekcija != ".ABS")
                                {
                                    Relocation r(ime_sekcije, offset, "R_X86_64_16PC", RB, operand);
                                    tabela_relokacija.push_back(r);
                                }
                            

                            s->dodaj_element(instr_kod, "ldr_str_PC");

                            s->dodaj_element(stoi(registar), "registar");
                            if (vrednost != -1 && !pronadji_simbol(operand)->is_global)
                                s->dodaj_element(vrednost - 2, "simbol");
                            else
                                s->dodaj_element(-2, "simbol");

                            location_counter += 5;
                        }
                        else
                        { // ili registar+ simbol ili registar+ literal

                            string reg = operand;

                            reg = reg.substr(2, reg.size());
                            string plus;
                            tok >> plus;
                            string operand2;
                            tok >> operand2;
                            operand2 = operand2.substr(0, operand2.size() - 1);

                            int vrednost = -1;
                            if (tip_tokena(operand2) == "SIMBOL")
                            {
                                for (int i = 0; i < tabela_simbola.size(); i++)
                                    if (tabela_simbola.at(i).naziv == operand2)
                                        vrednost = tabela_simbola.at(i).offset;
                                if (vrednost == -1)
                                {
                                    //apsoultna relokacija
                                    int offset = location_counter + 3;

                                    dodaj_simbol(operand, "?", -1);

                                    Relocation r(ime_sekcije, offset, "R_X86_64_16", tabela_simbola.size() - 1, operand);
                                    tabela_relokacija.push_back(r);
                                }
                                else
                                { //Ima simbola al svakako apsoultna relokacija!
                                    int RB;
                                    int offset = location_counter + 3;
                                    Symbol *sym = pronadji_simbol(operand);
                                    if (sym->is_global)
                                        RB = sym->RB;
                                    else
                                    {
                                        string sekcija = sym->sekcija;
                                        if (sekcija != ".ABS")
                                            RB = pronadji_simbol(sekcija)->RB;
                                    }
                                    if (sym->sekcija != ".ABS")
                                    {
                                        Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, operand);
                                        tabela_relokacija.push_back(r);
                                    }
                                }
                            }
                            else
                            { // LITERAL
                                if (operand2.at(0) == '0' && operand2.at(1) == 'x')
                                { // hexa vrednost
                                    operand2 = operand2.substr(2, operand2.size());
                                    vrednost = stoul(operand2, nullptr, 16);
                                }
                                else
                                { // decimalna
                                    vrednost = stoi(operand2);
                                }
                            }
                            s->dodaj_element(instr_kod, "ldr_str_pom");
                            s->dodaj_element(stoi(registar), "registar");
                            s->dodaj_element(stoi(reg), "registar");
                            s->dodaj_element(vrednost, "simbol/literal");
                            location_counter += 5;
                        }
                    }
                    else
                    { // JMP instrukcije

                        string operand;
                        tok >> operand;
                        if (tip_tokena(operand) == "LITERAL" || tip_tokena(operand) == "SIMBOL" || tip_tokena(operand) == "*LITERAL" || tip_tokena(operand) == "*SIMBOL")
                        {
                            bool imm = true;

                            if (operand.at(0) == '*')
                            {
                                operand = operand.substr(1, operand.size());
                                imm = false;
                            }
                            int vrednost = -1;
                            if (tip_tokena(operand) == "LITERAL")
                            {

                                if (operand.at(0) == '0' && operand.at(1) == 'x')
                                { //skidam 0 i x
                                    operand = operand.substr(2, operand.size());
                                    vrednost = stoul(operand, nullptr, 16);
                                }
                                else
                                    vrednost = stoi(operand);
                            }
                            else
                            { //SIMBOL
                                int RB;
                                int offset = location_counter + 3;
                                for (int i = 0; i < tabela_simbola.size(); i++)
                                    if (tabela_simbola.at(i).naziv == operand)
                                    {
                                        vrednost = tabela_simbola.at(i).offset;
                                        RB = tabela_simbola.at(i).RB;
                                    }

                                if (!pronadji_simbol(operand))
                                {
                                    dodaj_simbol(operand, "?", -1);
                                    RB = tabela_simbola.size() - 1;
                                }
                                else
                                {

                                    Symbol *sym = pronadji_simbol(operand);
                                    if (sym->is_global)
                                        RB = sym->RB;
                                    else
                                    {
                                        string sekcija = sym->sekcija;
                                        if (sekcija != ".ABS")
                                            RB = pronadji_simbol(sekcija)->RB;
                                    }
                                }
                                // if(vrednost!=-1){
                                // for(int i=0;i<tabela_simbola.size();i++)
                                // if(tabela_simbola.at(i).naziv==s->naziv)
                                // RB=tabela_simbola.at(i).RB;
                                // }
                                if (pronadji_simbol(operand)->sekcija != ".ABS")
                                {
                                    Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, operand);
                                    tabela_relokacija.push_back(r);
                                }
                            }
                            if (imm)
                                s->dodaj_element(instr_kod, "instr_skoka_imm");
                            else
                                s->dodaj_element(instr_kod, "instr_skoka_mem");

                            s->dodaj_element(vrednost, "operand");

                            location_counter += 5;
                        }
                        else if (tip_tokena(operand) == "%SIMBOL")
                        { //PC rel. adresiranje

                            operand = operand.substr(1, operand.size());
                            int RB;
                            int vrednost = -1;
                            int offset = location_counter + 3;

                            for (int i = 0; i < tabela_simbola.size(); i++)
                                if (tabela_simbola.at(i).naziv == operand)
                                {
                                    vrednost = tabela_simbola.at(i).offset;
                                    RB = tabela_simbola.at(i).RB;
                                }

                            if (!pronadji_simbol(operand))
                            {
                                dodaj_simbol(operand, "?", -1);
                                RB = tabela_simbola.size() - 1;
                                offset = -2;
                            }
                            else
                            {
                                Symbol *sym = pronadji_simbol(operand);
                                if (sym->is_global)
                                    RB = sym->RB;
                                else
                                {
                                    string sekcija = sym->sekcija;
                                    if (sekcija != ".ABS")
                                        RB = pronadji_simbol(sekcija)->RB;
                                }
                            }
                            // if(vrednost!=-1){
                            // for(int i=0;i<tabela_simbola.size();i++)
                            // if(tabela_simbola.at(i).naziv==s->naziv)
                            // RB=tabela_simbola.at(i).RB;
                            // }
                            if (pronadji_simbol(operand)->sekcija != ".ABS")
                            {
                                Relocation r(ime_sekcije, offset, "R_X86_64_16PC", RB, operand);
                                tabela_relokacija.push_back(r);
                            }
                            s->dodaj_element(instr_kod, "instr_skoka_PC");
                            if (vrednost != -1 && !pronadji_simbol(operand)->is_global)
                                s->dodaj_element(vrednost - 2, "simbol");
                            else
                                s->dodaj_element(-2, "simbol");

                            location_counter += 5;
                        }
                        else if (tip_tokena(operand) == "*REGISTAR" || tip_tokena(operand) == "*[REGISTAR]")
                        { //reg i regind

                            bool reg = true;
                            if (operand.at(1) == '[')
                            {
                                operand = operand.substr(2, operand.size() - 1);
                                reg = false;
                            }
                            else
                                operand = operand.substr(1, operand.size());

                            operand = operand.substr(1, operand.size()); // broj registra

                            if (reg)
                                s->dodaj_element(instr_kod, "instr_skoka_reg");
                            else
                                s->dodaj_element(instr_kod, "instr_skoka_regind");

                            s->dodaj_element(stoi(operand), "registar");

                            location_counter += 3;
                        }
                        else
                        { // reg+ literal ili reg + simbol

                            string registar = operand.substr(3, operand.size()); // reg
                            string plus;
                            tok >> plus; //+
                            string value;
                            tok >> value;
                            value = value.substr(0, value.size() - 1);
                            int vrednost = -1;
                            if (tip_tokena(value) == "LITERAL")
                            {

                                if (value.at(0) == '0' && value.at(1) == 'x')
                                { //skidam 0 i x
                                    value = value.substr(2, value.size());
                                    vrednost = stoul(value, nullptr, 16);
                                }
                                else
                                    vrednost = stoi(value);
                            }
                            else
                            { //SIMBOL
                                int RB;
                                int offset = location_counter + 3;
                                for (int i = 0; i < tabela_simbola.size(); i++)
                                    if (tabela_simbola.at(i).naziv == value)
                                    {
                                        vrednost = tabela_simbola.at(i).offset;
                                        RB = tabela_simbola.at(i).RB;
                                    }

                                if (!pronadji_simbol(value))
                                {
                                    dodaj_simbol(value, "?", -1);
                                    RB = tabela_simbola.size() - 1;
                                }
                                else
                                {
                                    Symbol *sym = pronadji_simbol(value);
                                    if (sym->is_global)
                                        RB = sym->RB;
                                    else
                                    {
                                        string sekcija = sym->sekcija;
                                        if (sekcija != ".ABS")
                                            RB = pronadji_simbol(sekcija)->RB;
                                    }
                                }
                                // if(vrednost!=-1){
                                // for(int i=0;i<tabela_simbola.size();i++)
                                // if(tabela_simbola.at(i).naziv==s->naziv)
                                // RB=tabela_simbola.at(i).RB;
                                // }
                                if (pronadji_simbol(value)->sekcija != ".ABS")
                                {
                                    Relocation r(ime_sekcije, offset, "R_X86_64_16", RB, value);
                                    tabela_relokacija.push_back(r);
                                }
                            }
                            s->dodaj_element(instr_kod, "instr_skoka_pom");
                            s->dodaj_element(stoi(registar), "registar");
                            s->dodaj_element(vrednost, "literal/simbol");
                            /*Apsolutna relokacija*/
                            location_counter += 5;
                        }
                    }
                }
            }
        }
    }
}

void Assembler::ispisi()
{
    ofstream ofs(izlazni_fajl);

    ofs << "POMOCNA TABELA " << endl;
    for (int i = 0; i < tabela_sekcija.size(); i++)
        ofs << tabela_sekcija.at(i).naziv << "\t" << tabela_sekcija.at(i).size << endl;

    ofs << endl;

    ofs << "TABELA SIMBOLA" << endl;
    ofs << "RB"
        << "\t"
        << "Naziv"
        << "\t\t\t"
        << "Sekcija "
        << "\t\t\t"
        << "offset"
        << "\t\t"
        << "GLOBAL/LOCAL" << endl;
    for (int i = 0; i < tabela_simbola.size(); i++)
       ofs << tabela_simbola.at(i) << endl;

    ofs << endl
        << endl;
    ofs << "TABELA SEKCIJA" << endl;

    for (int i = 0; i < tabela_sekcija.size(); i++)
        ofs << tabela_sekcija.at(i) << endl;

    ofs << endl;
    ofs << "TABELA RELOKACIJA" << endl;
    ofs << "#Sekcija"
        << "\t\t"
        << "#OFFSET"
        << "\t\t"
        << "#TIP REL."
        << "\t\t\t"
        << "#RB"
        << "\t\t"
        << "#SIMBOL" << endl;
    for (int i = 0; i < tabela_relokacija.size(); i++)
        ofs << tabela_relokacija.at(i);
}