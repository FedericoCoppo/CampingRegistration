#include "Utility.h"
#include "Datas.h"

// COMPARA STRIGHE CASE INSENSITIVE
bool caseInsensitiveStringCompare(const string& str1, const string& str2)
{
    if (str1.size() != str2.size())
    {
        return false;
    }
    for (string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2)
    {
        if (tolower(*c1) != tolower(*c2))
        {
            return false;
        }
    }
    return true;
}

// COMPARA STRIGHE CASE SENSITIVE
bool caseSensitiveStringCompare(const string& str1, const string& str2)
{
    if (str1.size() != str2.size())
    {
        return false;
    }
    for (string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2)
    {
        if ((*c1) != (*c2))
        {
            return false;
        }
    }
    return true;
}

string intToString(int value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

// STAMPA LA DATA
void StampaData (void)
{
    SetTextColor(TEAL_TEXT);
    // GESTIONE DATA CORRENTE
    Datas * ptData = new Datas();
    ptData->Open();
    ptData->PrintLocalData();
    delete ptData;
    resetTextColor();

}
// STAMPA IL MENU'
void StampaMenu (void)
{
    cout<< "\n**********************************************************************\n";
    cout << "Digita: -'ESC' per uscire dal programma\n\t-'p' per leggere l'elenco completo degli " ;
    cout << "Alloggiati\n\t-'s' per leggere dati del singolo Alloggiato";
    cout << "\n\t-'i' per inserire un nuovo Alloggiato\n\t-'c' per cancellare un Alloggiato";
    cout<< "\n\t-'r' per registrare un numero variabile di Alloggiati" <<endl ;
    cout<< "**********************************************************************\n";
}
string AllNation[500] ;

static bool PrintAllNation (void)
{
    bool success = true;
    ifstream f  ("CodeToNazione.txt");
    string CodeTmp, s;
    uint16_t cnt = 0;
    COORD Pos;
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(!f)
    {
        cout <<"Il file non esiste!\n";
        success = false;
        return success;
    }

    int rigaCurs = 2;

    while(f.good()) // fino a quando c'è qualcosa da leggere ..
    {
        getline(f, s); // legge tutta la riga dal file e la mette nella variabile s

        char c = s[0u];
        if(!s.empty() && (c!=0x20) )
        {
            istringstream iss(s);
            iss >> CodeTmp;      // Estrazione codice
            int index = s.find_first_not_of (CodeTmp) + 1u ;  // Indice del primo elemento della stringa che non contiene il codice
            s = &s[index];  // ti punti sul nome dello stato purchè sia spaziato di un carattere dal codice

            AllNation[cnt++] = s;
        }
    }
    f.close();

    // Ordinamento
    qsort (AllNation,cnt,sizeof(char *),
           (int (*)(const void *, const void *)) cmp);

    for(int i=0; i< cnt; i++)
    {
        // Implementazione per la versione di rilascio (scarrucolano i nomi) .
        cout << " " << AllNation[i] << endl;

        if( (i == 100) || (i==200) || (i==300) || (i==400) )
        {
        SetTextColor(GREEN_TEXT);
        system ("pause");
        resetTextColor();
        }

        #if 0
        if( !(i%2) )  // Indice pari
        {
            cout << " " << AllNation[i];
        }
        else
        {
            Pos = { 50 , rigaCurs };
            SetConsoleCursorPosition(hConsole,Pos);
            cout  << AllNation[i];
            cout << endl;
            rigaCurs++;
        }
        #endif
    }
    cout << endl;
    return success;
}
bool RicercaNazione (string treCifre, int n, string  & nation, string & nationCode)
{
    bool success = true;
    ifstream f  ("CodeToNazione.txt");
    string Nazione,CodeTmp, s;
    const int  maxStringArray = 100;
    string Naz[maxStringArray];
    string Cod[maxStringArray];
    int i,NazIndex;
    int BlockCnt=0;
    string NazIndexString;

    if(!f)
    {
        cout <<"Il file non esiste!\n";
        success = false;
        return success;
    }

    while(f.good())// fino a quando c'è qualcosa da leggere ..
    {
        getline(f, s); // legge tutta la riga dal file e la mette nella variabile s

        char c = s[0u];
        if(!s.empty() && (c!=0x20) )
        {
            istringstream iss(s);
            iss >> CodeTmp;      // Estrazione codice
           // iss >> Nazione;

            int index = s.find_first_not_of (CodeTmp) + 1u ;  // Indice del primo elemento della stringa che non contiene il codice

            s = &s[index];  // ti punti sul nome dello stato purchè sia spaziato di un carattere dal codice

            Nazione = s;

            int uguale= true;
            for( i=0;i<n;i++)
            {
              if(tolower(treCifre[i]) != tolower(Nazione[i]))
                 {
                    uguale= false;
                    break;
                 }
            }
            if(uguale)
            {
               Naz[BlockCnt] = Nazione;
               Cod[BlockCnt] = CodeTmp;
               BlockCnt++;
            }
        }
    }
    f.close();
    if(!BlockCnt)
    {
        StrStream.SetError("Corrispondenze non rilevate!\n");
        SetTextColor(GREEN_TEXT);
        system ("pause");
        resetTextColor();
        if (system("CLS")) system("clear");
        cout<< "NOTA PER FUTURI INSERIMENTI.\nLe nazioni disponibili nel database sono:\n";
        SetTextColor(GREEN_TEXT);
        system ("pause");
        resetTextColor();
        PrintAllNation();
        return false;
    }
    cout<< "Rilevate le seguenti nazioni:\n ";
    for( i = 0; i<BlockCnt; i++)
    {
        cout << "\t" << (i+1) << " " << Naz[i] << endl;
    }
    cout<< "Inserire numero corrispondente alla nazione ( es. 1,2,3, ... ): ";
    StrStream.Acq(NazIndexString);
    NazIndex= atoi(NazIndexString.c_str());
    if((!NazIndex)||NazIndex>BlockCnt)
    {
        StrStream.SetError("Il numero inserito e' fuori range!\n\7");
        return false;
    }
    nation = Naz[NazIndex -1];
    nationCode = Cod[NazIndex -1];
    cout << "Hai scelto: "; SetTextColor(PINK_TEXT);
    cout << Naz[NazIndex -1 ] << endl;
    resetTextColor();
    return success;
}

static const int  maxStringArray = 1500;
static string Pro[maxStringArray];
static string Cod[maxStringArray];
static string Com[maxStringArray];

bool RicercaProvincia (string dueCifre, int n, string  & prov, string & comu,  string & comuCode)
{
    bool success = true;
    ifstream f  ("CodeToProvinciaToComune.txt");
    string Provincia,CodeTmp, s, Comune;
    int i,ComIndex;
    string ComIndexString;
    int BlockCnt=0;
    COORD Pos;
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if(!f)
    {
        StrStream.SetError("Il file non esiste!\n\7");
        success = false;
        return success;
    }

    while(f.good())    // fino a quando c'è qualcosa da leggere
    {
        getline(f, s); // legge tutta la riga dal file e la mette nella variabile s

        char c = s[0u];
        if(!s.empty() && (c!=0x20))
        {
            istringstream iss(s);
            iss >> Provincia;
            iss >> CodeTmp;      // Estrazione codice

            int index = s.find_first_not_of (Provincia) + 1u ;  // Indice del primo elemento della stringa che non contiene il codice
            s = &s[index];  // ti punti sul nome dello stato purchè sia spaziato di un carattere dal codice
            index = s.find_first_not_of (CodeTmp) + 1u ;
            s = &s[index];
            Comune = s;

            int uguale= true;
            for( i=0; i<n; i++)
            {
                if(tolower(dueCifre[i]) != tolower(Provincia[i]))
                {
                    uguale= false;
                    break;
                }
            }
            if(uguale)
            {
                Pro[BlockCnt] = Provincia;
                Cod[BlockCnt] = CodeTmp;
                Com[BlockCnt] = Comune;
                BlockCnt++;
            }
        }
    }
    f.close();

    if(!BlockCnt)
    {
        StrStream.SetError("Provincia inserita non corretta!\n\7");
        return false;
    }

    if (system("CLS")) system("clear");
    cout<< "In provincia di ";
    StrStream.SetInfo(dueCifre);
    cout << " sono presenti i seguenti comuni:\n";
    int rigaCurs = 1;

   bool inordine = false;
   string ComTempo;
   string CodeTempo;

   while(!inordine)
   {
        inordine = true;
        for (uint32_t ordIdx = 0u; ordIdx < BlockCnt -1 ; ordIdx++)
        {
            if ( Com[ordIdx] > Com[ordIdx +1] )
            {
                inordine = false;

                // scambio  elementi
                ComTempo = Com[ordIdx];
                Com[ordIdx] = Com[ordIdx +1];
                Com[ordIdx + 1] = ComTempo;

                CodeTempo = Cod[ordIdx];
                Cod[ordIdx] = Cod[ordIdx +1];
                Cod[ordIdx + 1] = CodeTempo;

           }
       }
   }

    for( i = 0; i<  BlockCnt; i++)
    {
        cout << (i+1) << " " << Com[i] << endl;
        if( (i == 100) || (i==200) || (i==300) || (i==400))
        {
        SetTextColor(GREEN_TEXT);
        system ("pause");
        resetTextColor();
        }

        #if 0
        // Indice pari
        if( !(i%2) )
        {
           cout << (i+1) << " " << Com[i];
        }
        else
        {
         Pos = { 50 , rigaCurs };
         SetConsoleCursorPosition(hConsole,Pos);
         cout << (i+1) << " " << Com[i];
         cout << endl;
         rigaCurs++;
        }
        if( i==500 )  // Limite di comuni visualizzabile su una singla pagina di console .
        {
            cout << endl;
            SetTextColor(GREEN_TEXT);
            system ("pause");
            resetTextColor();
        }
        #endif
    }
    cout<< "\nInserire numero corrispondente al comune ( es. 1,2,3, ...): ";
    StrStream.Acq(ComIndexString);
    ComIndex= atoi(ComIndexString.c_str());
    if((!ComIndex)||ComIndex>BlockCnt)
    {
        StrStream.SetError("Il numero inserito e' fuori range!\n");
        return false;
    }

    comu     = Com[ComIndex -1];
    comuCode = Cod[ComIndex -1];
    prov    =  Pro[ComIndex -1];

    cout << "Hai scelto: ";
    SetTextColor(PINK_TEXT);
    cout << comu << endl;
    resetTextColor();
    SetTextColor(GREEN_TEXT);
    system ("pause");
    resetTextColor();
    return success;
}


// Ordinamento
int cmp (char **str1, char **str2)
{
  return strcmp(*str1,*str2);
}

void cleanbuffer (char * pt, uint8_t s)
{
 uint8_t i;
 for(i=0;i<s;i++)
 {
    pt[i] = 0x20;
 }
}

// Converte la data sostituendo gli zeri .
void convertDatawithZero (string & s)
{
    string tmp = s;
    string supp;

///   1/12/1985
///   1/1/1985
///   01/1/1985

// CONTROLA PRIMA IL GIORNO .
    if(tmp[1] == '/')
    {
        // Giorno con una cifra mancante
        tmp = "0" + s;
        s = tmp;
    }

//   01/1/1985
// CONTROLA IL MESE .

    if(tmp[4] == '/')
    {
        // Mese con una cifra mancante
        supp = tmp + tmp[8];

       for(int i = 0; i< 3; i++ )  // gg/
       {
           supp[i] = tmp[i];
       }

       supp[3] = '0';              // gg/0

       for(int i = 4; i < 10; i++ )
       {
        supp[i] = tmp[i -1];
       }

       s = supp;
    }
}
