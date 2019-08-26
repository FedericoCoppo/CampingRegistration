#include "StringToCodeConv.h"

bool CodeStringConverter :: IsOpen = false;

// COST
CodeStringConverter::CodeStringConverter(string s)
{
    this->String = s;
    IsOpen = false;
    Trovato = false;
}

CodeStringConverter::CodeStringConverter()
{
    IsOpen = false;
    Trovato = false;
}

// LETTURA
bool CodeStringConverter::LoadFile (string code, CodeStringConverter * pt,const char * filename)
{
    bool success = true;

    ifstream f(filename); // nome del file da aprire, si può mettere anche il percorso (es C:\\file.txt)
    string s, CodeTmp,StringTmp;

    int cnt = 0;

    if(!f)
    {
        StrStream.SetError("Il file non esiste!\n\7");
        success = false;
        return success;
    }

    while(f.good() && (!pt->IsTrovato()))// fino a quando c'è qualcosa da leggere ..
    {
        getline(f, s); // legge tutta la riga dal file e la mette nella variabile s

        char c = s[0u];
        if(!s.empty() && (c!=0x20))
        {
            istringstream iss(s);
            iss >> CodeTmp;      // Estrazione codice

            int index = s.find_first_not_of (CodeTmp) + 1u ;  // Indice del primo elemento della stringa che non contiene il codice

            if(CodeTmp==code)
            {
                s = &s[index];  // ti punti sul nome dello stato, comune o documento, purchè sia spaziato di un carattere dal codice

                pt->SetTrovato(true);

                pt->SetString(s);
            }
        }
    }
    IsOpen = true;
    f.close(); // chiude il file una volta caricata la struttura
    IsOpen = false;
    return success;
}

