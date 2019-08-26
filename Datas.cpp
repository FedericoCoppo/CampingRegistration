#include "Datas.h"
#include "Utility.h"
/******************************Classe Data******************************************************/
// Costruttore .
Datas::Datas()
{
    this->DataString = "Data sconosciuta";
    this->currentTime = 0;
    this->gg = 0;
    this->mm = 0;
    this->aa = 0;
    this->IsOpen = false;
}
// Distruttore .
Datas::~Datas()
{
    if(IsOpen)
    {
        //delete this;
    }
}
// Open: cattura data attuale e la converte in stringa riempiendo tutti i vari campi della struttura .
void Datas::Open (void)
{
    this->IsOpen  = true;
    this->GetLocalData();
    this->ConvertDataToString();
}
// Cattura data attuale .
void Datas::GetLocalData(void)
{
    time(&this->currentTime);
    this->localTime = localtime(&this->currentTime);
}
// Print Data attuale formato "lungo" .
void Datas::PrintLocalData()
{
    if(this->IsOpen)
    {
        cout << "\n\t" <<asctime(localtime(&this->currentTime));
    }
}

string Datas::GetStringFileData()
{
    string s;
    s =  intToString(gg) + "_" + intToString(mm) + "_" + intToString(aa);
    return s;
}


// Routine di supporto per convertire interi in stringhe .
string Datas::ConvertNumToString(int num)
{

    string Result;          // string which will contain the result

    ostringstream convert;   // stream used for the conversion

    convert << num;      // insert the textual representation of 'Number' in the characters in the stream

    Result = convert.str(); // set 'Result' to the contents of the stream
    return Result;
}

// Conversione della stringa data finale in formato mm/gg/aa
void Datas::ConvertDataToString(void)
{
    string s;
    this->gg = (this->localTime->tm_mday);
    this->mm = (this->localTime->tm_mon + 1);
    this->aa = (this->localTime->tm_year + 1900);

    s = this->ConvertNumToString(this->gg);
    s += '/';
    s += this->ConvertNumToString(this->mm);
    s += '/';
    s += this->ConvertNumToString(this->aa );

    this->DataString =s;
}
// Stampa della data finale .
void Datas::PrintStringData(void)
{
    if(IsOpen)
    {
        cout << "Data : " << DataString << endl;
    }
}

// GetAnnoDataOdierna .
int Datas::GetAnnoDataOdierna (void)
{
    if(IsOpen)
    {
     return this->aa ;
    }
    else
    {
    return 0;
    }
}

// GetAnnoDataOdierna .
int Datas::GetMeseDataOdierna (void)
{
    if(IsOpen)
    {
     return this->mm ;
    }
    else
    {
    return 0;
    }
}

// GetAnnoDataOdierna .
int Datas::GetGiornoDataOdierna (void)
{
    if(IsOpen)
    {
     return this->gg ;
    }
    else
    {
    return 0;
    }
}


