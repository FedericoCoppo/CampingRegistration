#include "Alloggiati.h"
#include "Console.h"
#include "string.h"
#include <iostream>
#include "Windows.h"
#include "wincon.h"
#include <fstream>
#include <ctime>
#include <conio.h>
#include "Utility.h"
#include "Datas.h"

#define INIT_BEEP

using namespace std;

int main()
{

	// GESTIONE CONSOLE ED INIZIALIZZAZIONE VERDE ACQUA
    HANDLE hConsole;
    WORD   wAttr;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    wAttr = TEAL_TEXT;
    SetConsoleTextAttribute(hConsole, wAttr);
    SetConsoleTitle("*****CAMPING MOMBARONE*****");

#ifdef DEBUG
    ptData->PrintStringData();
#endif

    resetTextColor(hConsole);

	// CARICAMENTO DEL FILE NELLA STRUTTURA DATI .
    if(!Alloggiati::LoadFile())
    {
        return 0;
    }

	// GESTIONE DEL MENU PRINCIPALE
    bool loop = true;
    char r ;
    int test =60;
    string name;
    string surname;
    Alloggiati * AlloggPt;

    SetTextColor(BLUE_TEXT);
    cout<<"\n\tBENVENUTO/A!\n\n";

	// BEEP!
#ifdef INIT_BEEP
    Beep(1568, 200);
    Beep(1568, 200);
    Beep(1568, 200);
    Beep(1245, 1000);
    Beep(1397, 200);
    Beep(1397, 200);
    Beep(1397, 200);
    Beep(1175, 1000);
#endif

    // Link puntatori .
    Alloggiati::LinkPuntatoriPadri();

    while (loop)
    {
        cout << "\n";
        SetTextColor(GREEN_TEXT);
        system ("pause");
        resetTextColor();
        // Clear The screen.
        if (system("CLS")) system("clear");

        StampaData ();
        StampaMenu();    // Menu
        Sleep(5);
        r=getch();

        if (r== ESC)
        {
            loop = false;
        }
        else {}

        switch(r)
        {

        case 'p' : case 'P' :

            Alloggiati::StampaAlloggiati();
            break;

        case 'c': case 'C':
            Alloggiati::LinkPuntatoriPadri();
            cout << "Inserire Cognome alloggiato da cancellare: " ;
            StrStream.Acq(surname);
            cout << "Inserire Nome alloggiato da cancellare   : " ;
            StrStream.Acq(name);
            Alloggiati::CancellaAlloggiatoMenu(name,surname );
            Alloggiati::FileAlloggiatiUpdate();
            Alloggiati::LinkPuntatoriPadri();
            break;
        case 's':  case 'S':
            cout << "Inserire Cognome alloggiato da visualizzare: ";
            StrStream.Acq(surname);
            cout << "Inserire Nome alloggiato da visualizzare   : ";
            StrStream.Acq(name);

            Alloggiati::CercaAlloggiatoMenu(name,surname );
            break;

        case 'i': case 'I':
            AlloggPt = new (Alloggiati);
            if(Alloggiati::InserisciAlloggiato(AlloggPt))
            {
                ///Inserimento con successo .
                if (!AlloggPt->InserimentoOrdinatoAlloggiati())
                {
                   StrStream.SetError("Inserimento non effettuato\n");
                   if(AlloggPt!=NULL)
                   {
                     delete(AlloggPt);
                     AlloggPt = NULL;
                   }
                }
                else
                {
                  StrStream.SetError("Inserimento effettuato !\n");
                  // Aggiorna tutti i puntatori .
                  Alloggiati::LinkPuntatoriPadri();
                  // Aggiorna il File .
                  Alloggiati::FileAlloggiatiUpdate();
                }
            }
            else
            {
               StrStream.SetError("Inserimento non effettuato\n");
                if(AlloggPt!=NULL)
                {
                    delete(AlloggPt);
                    AlloggPt = NULL;
                }
            }
            break;

       case 'r': case 'R':
           if(Alloggiati::RegistraAlloggiatiMenu())
           {

           }
           else
           {
             StrStream.SetError("\nRegistrazione Fallita!\n");
           }
           break;

        default:
            break;
        }
    }

    StrStream.SetInfo("Il programma e' terminato con successo, PREMI UN TASTO E CHIUDI.\n");
    SetTextColor(GREEN_TEXT);
    system ("pause");
    resetTextColor();
    return 0;
} // END OF MAIN

