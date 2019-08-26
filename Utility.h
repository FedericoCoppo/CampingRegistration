#ifndef UT_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include "Console.h"

#include <fstream>

using namespace std;

#define ESC 27

// COMPARA 2 STRIGHE "CASE INSENSITIVE"
bool caseInsensitiveStringCompare(const string& str1, const string& str2);
bool caseSensitiveStringCompare(const string& str1, const string& str2);
// STAMPA IL MENU'
void StampaMenu(void);
// CONVERT INTERO IN STRING
string intToString(int value);

void StampaData (void);

void cleanbuffer (char * pt, uint8_t s);

template <class T>
class Stream
{
public:
    void Acq( T & stream )
    {
        SetTextColor(GREEN_TEXT);
        cin >> stream;
        resetTextColor();
    };

    void SetError( const string s )
    {
        SetTextColor(RED_TEXT);
        cout << s ;
        resetTextColor();
    };


    void SetInfo ( const string s )
    {
        SetTextColor(PINK_TEXT);
        cout << s ;
        resetTextColor();
    };
}; // fine classe Vettore

extern Stream  <string> StrStream;


bool RicercaNazione (string treCifre, int n, string  & nation, string & nationCode);
bool RicercaProvincia (string dueCifre, int n, string  & prov, string & comu,  string & comuCode);

int cmp (char **str1, char **str2);

// Converte la data sostituendo gli zeri .
void convertDatawithZero (string & s);

#define UT_H_INCLUDED
#endif // UT_H_INCLUDED

