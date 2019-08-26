#ifndef DATA_H_INCLUDED

#include "string.h"
#include <sstream>
#include <iostream>
#include <ctime>

using namespace std;

/******************************Classe Data******************************************************/
class Datas
{
public:
    // Costruttore .
    Datas();

    // Dist .
    ~Datas();

    // Legge data corrente da s.o ed inizializza le strutture
    void Open (void);

    // stampa data corrente .
    void PrintLocalData();

    // Converte la data nel formato stringa .
    void ConvertDataToString(void);

    // Stampa data corrente in formato stringa .
    void PrintStringData(void);

    int GetAnnoDataOdierna (void);
    int GetMeseDataOdierna (void);
    int GetGiornoDataOdierna (void);

    // Ritorna la datacorrente in formato stringa .
    string GetStringData(){return this->DataString;}

    // Formato compatibile per rinominare i file .
    string GetStringFileData();

private:
    // Usata dalla Open per leggere dal sistema operativo data attuale .
    void GetLocalData(void);

    // Data  in formato stringa .
    string DataString;

    // struttura perleggere da S.O la data attuale .
    time_t currentTime;

    // Routine di appoggio che converte interi in stringhe .
    string ConvertNumToString(int num);

    //Valori della data utili (inizializzati nella  open) .
    int gg;
    int mm;
    int aa;
    struct tm *localTime;

    // Flag di apertura del driver .
    bool IsOpen;
};

#define DATA_H_INCLUDED
#endif //DATA_H_INCLUDED

