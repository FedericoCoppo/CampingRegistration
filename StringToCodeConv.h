#ifndef COMUNI_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
#include "Utility.h"

using namespace std;

class CodeStringConverter
{
public:
    // Cost.
    CodeStringConverter(string s);
    CodeStringConverter ();
    // Dist.

    static  bool LoadFile (string code, CodeStringConverter * pt, const char * filename);
    static bool IsOpen;

    string GetString (void) {return this->String;}
    bool IsTrovato(void)     {return this->Trovato;}
    void SetTrovato (bool trovato) {this->Trovato =trovato;}

private:
    string String;
    void SetString (string s) {this->String = s;}
    bool Trovato;
};


#define COMUNI_H_INCLUDED
#endif // COMUNI_H_INCLUDED


