#ifndef ALL_H

#include <iostream>
#include <windows.h>
#include "wincon.h"
#include <fstream>
#include <ctime>
#include "Console.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include <conio.h>
#include "StringToCodeConv.h"
#include "Datas.h"

using namespace std;

// #define DEBUG

#define MASCHIO 0
#define FEMMINA 1

#define UndefinedString "Estero/NonDefinito"

// Const
const uint16_t MAX_ALLOGGIATI_NUM = 1000;
const uint8_t  MAX_CONSOLE_CHAR   = 100;
const uint8_t  MAX_DISCENDENTI    = 30;

// main class
class Alloggiati
{
public:

    enum TipoAlloggiato : uint8_t
    {
        MinAlloggiatoTypeCode = 16,
        OspiteSingolo = 16,
        CapoFamiglia  = 17,
        CapoGruppo    = 18,
        Famigliare    = 19,
        MembroGruppo  = 20,
        MaxAlloggiatoTypeCode = 20,

        MaxAlloggiatoTypeNum = 5,
    };

    struct AlloggiatoEntry
    {
        /// Classe dell'alloggiato .
         uint8_t AlloggiatoClass;

        /// Stringa associata alla classe .
        const char *ClassString;
    };

    enum ParseParam
    {
        tipoAlloggiato,
        nome      ,
        cognome   ,
        sesso     ,
        dataNascita,
        StaNascita,
        StaCittadinanza,
        comuneNascita,
        provNascita,
        documentoType,
        documentoString,
        documentoLocalita,
        nomeCapo,
        cognomeCapo,
        ParseParamNum,
    };

    //Cost.
    Alloggiati(string s);
    Alloggiati();

    //Dist .
    ~Alloggiati();

	//ARRAY DI PUNTATORI AGLI STREAM DEL FILE
    static  Alloggiati * PtAlloggiati[MAX_ALLOGGIATI_NUM];

    static  ifstream f;
	// CARICA IL FILE Alloggiati.txt nella struttura interna .
    static  bool LoadFile (void);
	// NUMERO TOTALE DI ALLOGGIATI
    static  uint16_t AllN;

    friend  Alloggiati & operator <<( Alloggiati * a, const string & s);

    static void StampaAlloggiati (void);
    static void CancellaAlloggiatoMenu (string n , string c);
    static void CercaAlloggiatoMenu   (string name , string surname);

    //RICERCA DI UN ALLOGGIATO
    static bool IsAlloggiatoTrovato(string n,string c,  uint16_t & index);
    //RICERCA DI UN ALLOGGIATO SOLO PER COGNOME
    static bool IsAlloggiatoTrovato(string c,  uint16_t & index);

    static bool IsOpen;

    void StampaSingoloAlloggiato(void);
    void StampaNome(){cout<<this->Name;}
    void StampaCognome(){cout<<this->Surname;}
    void StampaDataNascita() {cout<<this->DataNascita;}

    void StampaComuneNascita() {cout<<this->ComuneNascita;}
    void StampaProvinciaNascita() {cout<<this->ProvinciaNascita;}
    void StampaStatoNascita() {cout<<this->StatoNascita;}
    void StampaStatoCittadinanza() {cout<<this->StatoCittadinanza;}
    void StampaSesso() {}

    string GetNome (void){return this->Name;}
    string GetCognome(void){return this->Surname;}

    // Inserimento del comune in base al codice ricevuto
    bool SetStringFromCode (string Code, const char * filename, string & String);

    void SetGiorniPermanenza(int g){GiorniPermanenza = g;}
    int GetGiorniPermanenza(){return GiorniPermanenza;}

    // FUNZIONI DI INSERIMENTO
    static bool InserisciAlloggiato (Alloggiati * AlloggPt);
    bool InserisciDataNascita ();
    bool InserisciNominativo ();
    bool InserisciSesso ();
    bool InserisciNazione (string & nazione,string & nazioneCode, const char * destination);
    bool InserisciProvincia ();
    bool InserisciTipoAlloggiato ();
    bool InserisciTipoDocumentoAlloggiato ();
    bool InserisciLocalitaRilascioDoc ();

    static bool FileAlloggiatiUpdate (void);
    bool InserimentoOrdinatoAlloggiati ( void);

    string GetSuperStream (void) { return superStream; }
    void SetSuperStream (string s) { superStream = s;}
    bool SuperStreamUpdate (void);
    bool LinkaIlCapoAlloggiato();

    static bool LinkPuntatoriPadri (void);
    void PrintMembriFamiliari (void);

    // Registrazione .
    static bool RegistraNominativo (string & n ,string & c, uint16_t number);
    static bool RegistraAlloggiatiMenu();
    static bool AlloggiatoPoolInsertionManagement(uint16_t Index );
    static bool RichiediDatiRegistrazione ();
    static bool AlloggiatiRegistraStream(string f );

private:

    // Stream del file Alloggiati
    string superStream;

    // Flag Indicante se è nato all'estero
    bool IsNatoEstero;

    // Flag che indica se è un familiare o membro gruppo
    bool IsFamiliare;

    // Punatore al capofamiglia/capogruppo
     Alloggiati * pt_capo;
     Alloggiati * pt_AllFigli[MAX_DISCENDENTI];
     uint8_t     pt_AllFigliCnt;

    // DISALLOCAZIONE DINAMICA ALLOGGIATO .
    void AlloggiatoDelete (uint16_t i);
    void AlloggiatoDeleteFigli (void);

    string         Name;                  
    string         Surname;                
    bool           Sex;                    
    TipoAlloggiato Tipo;
    string         DataNascita;            

    string         ComuneNascita;
    string         ProvinciaNascita;
    string         StatoNascita;
    string         StatoCittadinanza;

    string         ComuneNascitaCode;
    string         StatoNascitaCode;
    string         StatoCittadinanzaCode;

    string         DocumentoTypeCode;
    string         DocumentoString;
    string         Comune_o_StatoRilascioDocumento;
    string         LocalitaRilascioDocumentoCode;

    int  GiorniPermanenza;

    bool IsCapoFamigliaSingolo;

    string         NomeCapo;
    string         CognomeCapo;

};

#define ALL_H
#endif // ALL_H


