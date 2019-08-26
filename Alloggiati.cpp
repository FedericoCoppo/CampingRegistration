
#include "Alloggiati.h"
#include "Utility.h"

#define CODE_COMUNE "CodeToComune.txt"
#define CODE_NAZIONE "CodeToNazione.txt"
#define CODE_DOCUMENTO "CodeToDocumento.txt"

#define CODE_ITALIA    "100000100"

#define NUM_CARATTERI_RICERCA_NAZIONE 2u

#define CODE_PATENTE         "PATEN"
#define CODE_PASSAPORTO      "PASOR"
#define CODE_CARTA_ID        "IDENT"
#define CODE_CARTA_ID_ELETT  "IDELE"

#define BLANK                "IsBlank"
//EXTERN LABEL
Stream  <string>   StrStream;

// INIZIALIZZAZIONE VARIABILI STATICHE
Alloggiati * Alloggiati::PtAlloggiati[MAX_ALLOGGIATI_NUM] = {NULL};
uint16_t Alloggiati::AllN = 0;
ifstream Alloggiati::f;
bool Alloggiati :: IsOpen = false;

// STRUTTURA RELATIVA ALLA CLASSE ALLOGGIATO
Alloggiati::AlloggiatoEntry AlloggiatoClassStringTable[Alloggiati::MaxAlloggiatoTypeNum] =
{
    { (uint8_t) Alloggiati::OspiteSingolo ,			"OspiteSingolo" },
    { (uint8_t) Alloggiati::CapoFamiglia ,			"CapoFamiglia"  },
    { (uint8_t) Alloggiati::CapoGruppo ,			"CapoGruppo"    },
    { (uint8_t) Alloggiati::Famigliare ,			"Famigliare"    },
    { (uint8_t) Alloggiati::MembroGruppo ,			"MembroGruppo"  },
};

// Inserimento del comune in base al codice ricevuto
bool Alloggiati::SetStringFromCode (string Code, const char * filename, string & String)
{
    CodeStringConverter * pt;
    pt = new CodeStringConverter();

    if(  pt->LoadFile(Code,pt,filename))
    {
        if(pt->IsTrovato())
        {
            String = pt->GetString();
#ifdef DEBUG
            cout << "inserito:" << this->ComuneNascita << endl ;
#endif // DEBUG
        }
        else
        {
#ifdef DEBUG
            SetTextColor(RED_TEXT);
            cout << "CODICE NON TROVATO" << endl ;
            resetTextColor();
#endif
            return false;
        }
    }

    if(pt)
    {
        delete pt;
    }

    return true;
}

// OVERRIDE OPERATORE FLUSH
Alloggiati & operator << ( Alloggiati * a, const string & s)
{
    int cnt = 0;
    char * StrTmp;
    istringstream iss(s);
    string ses, Code, String;
    a->IsCapoFamigliaSingolo = false;

    if(a!=NULL)
    {
        while (iss && cnt<Alloggiati::ParseParamNum)
        {
            switch(cnt)
            {
            case Alloggiati::tipoAlloggiato:
                iss >> StrTmp; // Caricamento in stringa
                a->Tipo = (Alloggiati::TipoAlloggiato)atoi(StrTmp);
                if( a->Tipo > Alloggiati::MaxAlloggiatoTypeCode ||  a->Tipo < Alloggiati::MinAlloggiatoTypeCode  )
                {
                 StrStream.SetError("CODICE TIPO ALLOGGIATO NON VALIDO\n\7");
                }

                if( (a->Tipo == Alloggiati::Famigliare) || (a->Tipo == Alloggiati::MembroGruppo) )
                {
                   a->IsFamiliare = true;
                }
                else
                {
                   a->IsFamiliare = false;
                }
                #ifdef DEBUG
                cout << a->Tipo << endl ;
                #endif
                break;

            case  Alloggiati::nome:
                iss >> a->Name;
                break;

            case  Alloggiati::cognome:
                iss >> a->Surname;
                break;

            case  Alloggiati::sesso:
                iss >> ses;
                if(caseInsensitiveStringCompare(ses, "m"))
                {a->Sex = MASCHIO;}
                else if (caseInsensitiveStringCompare(ses, "f"))
                {a->Sex = FEMMINA;}
                else
                {StrStream.SetError("CODICE TIPO ALLOGGIATO NON VALIDO\n\7");}
                break;

            case Alloggiati::dataNascita:
                iss >> a->DataNascita;
                break;

            case Alloggiati::StaNascita:
                iss >> Code;
                a->StatoNascitaCode = Code;
                if(a->SetStringFromCode(Code,CODE_NAZIONE,String))
                {
                    a->StatoNascita = String;
                    if(!(a->StatoNascitaCode == CODE_ITALIA))  /// usa il  codice italia, la stringa ITALIA non affidabile
                    {
                      a->IsNatoEstero = true;      // Setta se è nato all'estero
                    }
                }
                break;

            case Alloggiati::StaCittadinanza:
                iss >> Code;
                a->StatoCittadinanzaCode = Code;
                if(a->SetStringFromCode(Code,CODE_NAZIONE,String))
                {
                a->StatoCittadinanza = String;
                }
                break;

            case Alloggiati::comuneNascita:
                iss >> Code;
                a->ComuneNascitaCode = Code;
                if(a->SetStringFromCode(Code,CODE_COMUNE,String) && !a->IsNatoEstero)
                {
                a->ComuneNascita = String;
                }
                else
                {
                 a->ComuneNascita = UndefinedString;        ///Comune non trovato o estero
                 a->ComuneNascitaCode = UndefinedString;
                }
                break;

            case Alloggiati::provNascita:
                iss >> a->ProvinciaNascita;
                if (a->IsNatoEstero)
                {
                  a->ProvinciaNascita = UndefinedString; ///Overwrite oxff per province estere
                }
                break;

            case Alloggiati::documentoType:
                if(!a->IsFamiliare)
                {
                iss >> a->DocumentoTypeCode;
                }
                else
                {
                   a->DocumentoTypeCode = BLANK;
                }
                break;

            case Alloggiati::documentoString:
                if(!a->IsFamiliare)
                {
                iss >> a->DocumentoString;
                }
                else
                {
                a->DocumentoString = BLANK;
                }
                break;

            case Alloggiati::documentoLocalita:
                if(!a->IsFamiliare)
                {
                    iss >> Code ;
                    a->LocalitaRilascioDocumentoCode = Code;
                    if(a->SetStringFromCode(Code,CODE_COMUNE,String))
                    {
                        a->Comune_o_StatoRilascioDocumento = String;
                    }
                    else if(a->SetStringFromCode(Code,CODE_NAZIONE,String))
                    {
                        a->Comune_o_StatoRilascioDocumento = String;
                    }
                    else
                    {
                        StrStream.SetError("LOCALITA' RILASCIO DOCUMENTO NON VALIDA\n\7");
                    }
                }
                else
                {
                  a->Comune_o_StatoRilascioDocumento  = BLANK;
                }
                break;

            case Alloggiati::nomeCapo:
                 if(a->IsFamiliare)
                 {
                   iss >> a->NomeCapo;
                 }
                 else
                 {
                     a->NomeCapo = BLANK;
                 }
                 break;

            case Alloggiati::cognomeCapo:
                if(a->IsFamiliare)
                 {
                   iss >> a->CognomeCapo;
                 }
                 else
                 {
                     a->CognomeCapo = BLANK;
                 }
                 break;

            default:
                break;
            }
            cnt++;
        }
    }
    return *a;
}

// CARICAMENTO FILE ALLOGGIATI
bool Alloggiati::LoadFile (void)
{
    bool success = true;
    ifstream f("Alloggiati.txt"); 
    string s;

    if(!f)
    {
        StrStream.SetError("Il file non esiste!\n\7");
        success = false;
        return success;
    }

    while(f.good()&&(Alloggiati::AllN<MAX_ALLOGGIATI_NUM)) 
    {
        getline(f, s); // legge tutta la riga dal file e la mette nella variabile s
        char c = s[0u];
        if(!s.empty()  && (c!=0x20)  )
        {
            Alloggiati::PtAlloggiati[Alloggiati::AllN] = new Alloggiati(s);
            Alloggiati::AllN++;
        }
    }

    f.close(); // chiude il file una volta caricata la struttura
    IsOpen = true;
    return success;
}

// COST
Alloggiati::Alloggiati(string s)
{
    IsNatoEstero = false;

    this->Tipo   = Alloggiati::OspiteSingolo;
    this<<s;
    this->SetSuperStream(s);        ///memorizza tutta la string del file
}

Alloggiati::Alloggiati()
{
    IsNatoEstero = false;
    IsFamiliare  = false;
    this->Tipo   = Alloggiati::OspiteSingolo;
    superStream = "SuperStream Vuoto\n";
    NomeCapo = BLANK;
    CognomeCapo = BLANK;
    IsCapoFamigliaSingolo = false;
}

// DIST
Alloggiati:: ~ Alloggiati()
{
    if(this!=NULL && this->IsOpen)
    {

    }
}

/**STAMPA ALLOGGIATI**/
void Alloggiati::StampaAlloggiati()
{
    if(Alloggiati::IsOpen)
    {
        cout << "Sono Presenti in lista " << Alloggiati::AllN <<" Alloggiati su " <<MAX_ALLOGGIATI_NUM<< " disponibili:\n\n";
        // CAMBIA COLORE LISTA
        SetTextColor(PINK_TEXT);
        for(uint16_t i = 0; i< Alloggiati::AllN; i++)
        {
            if(Alloggiati::PtAlloggiati[i]!=NULL)
            {
                Alloggiati::PtAlloggiati[i]->StampaCognome();
                cout << " ";
                Alloggiati::PtAlloggiati[i]->StampaNome();
                cout << "\n";
            }

            if( (i == 100) || (i==225) || (i==360) || (i==480) || (i==600) || (i==800) )
            {
                SetTextColor(GREEN_TEXT);
                system ("pause");
                resetTextColor();
            }
        }

    }
    resetTextColor();
}

/// Stampa le informazioni desiderate di un alloggiato
void Alloggiati::StampaSingoloAlloggiato(void)
{
    int i;
    string String;
    SetTextColor(PINK_TEXT);
    cout << "\n";
    this->StampaCognome();
    cout << " " ;
    this->StampaNome();
    cout << ( (this->Sex == MASCHIO) ? "\nnato il : ":"\nnata il : ");
    this->StampaDataNascita();
    cout << "\na : ";
    this->StampaComuneNascita();
    cout << "\nprovincia : ";
    this->StampaProvinciaNascita();
    cout << "\nstato : ";
    this->StampaStatoNascita();
    cout << "\ncittadinanza : ";
    this->StampaStatoCittadinanza();

    // STAMPA ALLOGGIATO TYPE
    for( i = 0; i<Alloggiati::MaxAlloggiatoTypeNum ; i++)
    {
        if( AlloggiatoClassStringTable[i].AlloggiatoClass == this->Tipo)
        {
            /// Hai trovato l'indice!
            break;
        }
    }
    cout << ( (this->Sex == MASCHIO) ? "\nregistrato come : ":"\nregistrata come : ");
    cout << AlloggiatoClassStringTable[i].ClassString;

    if((this->Tipo != Alloggiati::Famigliare)&&(this->Tipo != Alloggiati::MembroGruppo))
    {
        cout << "\ndocumento : ";

        if(this->SetStringFromCode(this->DocumentoTypeCode,CODE_DOCUMENTO,String))
        {
            cout<< String <<", numero : "<<this->DocumentoString<<", rilasciato in : "<<this->Comune_o_StatoRilascioDocumento<<endl;
        }
        else
        {
            cout<<endl;
        }
    }
    else
    {
        cout << "\ndocumenti non  necessari per gli accompagnatori." << endl;
        cout << "Capofamiglia o Capogruppo di riferimento: " << this->CognomeCapo << " "<< this->NomeCapo<< endl;
    }
    resetTextColor();
}
// RICERCA DI UN ALLOGGIATO
bool Alloggiati::IsAlloggiatoTrovato(string n,string c,  uint16_t & index)
{
    bool success = false;
    if(!(Alloggiati::IsOpen))
    {
        return false;
    }

    for( uint16_t i = 0; i< Alloggiati::AllN; i++)
    {
        if(Alloggiati::PtAlloggiati[i]==NULL)
        {

            StrStream.SetError("Accesso a puntatore NULL!\n\7");
            return success;
        }
        if( (caseInsensitiveStringCompare(n ,Alloggiati::PtAlloggiati[i]->GetNome()))&&caseInsensitiveStringCompare(c,Alloggiati::PtAlloggiati[i]->GetCognome()  ))
        {
            index = i;
            success = true;
            break;
        }
    }
    return success;
}

// RICERCA DI UN ALLOGGIATO SOLO PER COGNOME
bool Alloggiati::IsAlloggiatoTrovato(string c,  uint16_t & index)
{
    bool success = false;
    if(!(Alloggiati::IsOpen))
    {
        return false;
    }

    for( uint16_t i = 0; i< Alloggiati::AllN; i++)
    {
        if(Alloggiati::PtAlloggiati[i]==NULL)
        {
            StrStream.SetError("Accesso a puntatore NULL!\n\7");
            return success;
        }
        if( caseInsensitiveStringCompare(c,Alloggiati::PtAlloggiati[i]->GetCognome()  ))
        {
            index = i;
            success = true;
            break;
        }
    }
    return success;
}

// DISALLOCAZIONE DINAMICA ALLOGGIATO 
void Alloggiati::AlloggiatoDelete (uint16_t i)
{
   uint16_t j;
   if(PtAlloggiati[i]!=NULL)
        {
            Alloggiati * ptTmp = PtAlloggiati[i];
            if(ptTmp!=NULL)
            {
                cout <<"\nALLOGGIATO "<< ptTmp->Surname << " " << ptTmp->Name << " CANCELLATO!\n";
                delete ptTmp;
            }
            ptTmp = NULL;
            PtAlloggiati[i] = NULL;

            for(j=i; j< (Alloggiati::AllN) - 1u; j++)
            {
                PtAlloggiati[j] = PtAlloggiati[j+1];
            }

            PtAlloggiati[Alloggiati::AllN - 1u] = NULL;
            Alloggiati::AllN--;
        }
}

// CERCA ALLOGGIATO
void Alloggiati::CercaAlloggiatoMenu   (string name , string surname)
{
  uint16_t j;
    if(Alloggiati::IsAlloggiatoTrovato(name,surname,j))
    {
        Alloggiati::PtAlloggiati[j]->StampaSingoloAlloggiato();
    }
    else
    {
        SetTextColor(RED_TEXT);
        cout <<"\nALLOGGIATO " << surname << " " << name << " NON TROVATO !" << endl;
        resetTextColor();
        if(Alloggiati::IsAlloggiatoTrovato(surname,j))
        {
            cout <<"Con il cognome ''" << surname << "'' e' stato trovato il seguente alloggiato:" << endl;
            Alloggiati::PtAlloggiati[j]->StampaSingoloAlloggiato();
        }
    }
}
// Stampa tutti gli alloggiati legati a CapoFamiglia/CapoGruppo
void Alloggiati::PrintMembriFamiliari (void)
{
  SetTextColor(PINK_TEXT);
   for(uint8_t i; i< pt_AllFigliCnt; i++)
   {
       if((this->pt_AllFigli[i]!=NULL)  && (this->pt_AllFigli[i]->pt_capo != NULL) && (this->pt_AllFigli[i]->pt_capo==this))
       {
        cout << this->pt_AllFigli[i]->Surname << " " << this->pt_AllFigli[i]->Name <<endl;
       }
   }
   resetTextColor();
}
// Cancella tutti i familiari/membrigruppo di  un alloggiato .
void Alloggiati::AlloggiatoDeleteFigli (void)
{
   uint16_t k;
   uint8_t l;
   for( l=0; l< this->pt_AllFigliCnt; l++)
   {
       if((this->pt_AllFigli[l]!=NULL)  && (this->pt_AllFigli[l]->pt_capo != NULL) && (this->pt_AllFigli[l]->pt_capo==this))
       {
         // DEVI CANCELLARE l'INDICE
         for ( k = 0; k< Alloggiati::AllN; k++)
         {
           if(Alloggiati::PtAlloggiati[k]==this->pt_AllFigli[l])
           {
            break;
           }
         }
         this->pt_AllFigli[l]->AlloggiatoDelete(k);
       }
   }
}

// CANCELLA ALLOGGIATO DALLA STRUTTURA INTERNA
void Alloggiati::CancellaAlloggiatoMenu (string n,string c)
{
    uint16_t i;
    string confirmation;
    if(!(Alloggiati::IsOpen))
    {
        return;
    }

    if(Alloggiati::IsAlloggiatoTrovato(n,c,i))
    {
        if( ((PtAlloggiati[i]->Tipo != Alloggiati::CapoFamiglia) &&
                (PtAlloggiati[i]->Tipo != Alloggiati::CapoGruppo))  || ((PtAlloggiati[i]->pt_AllFigliCnt==0)) )
        {
            SetTextColor(RED_TEXT);
            Alloggiati::PtAlloggiati[i]->AlloggiatoDelete(i);
        }
        else
        {
            cout << "L'alloggiato che si vuole cancellare e' un CapoFamiglia o CapoGruppo.\n";
            cout << "Con lui verranno cancellati i seguenti Membri:\n";
            PtAlloggiati[i]->PrintMembriFamiliari();
            cout <<"\nPREMERE '";
            cout <<"Y";
            cout <<"' PER CONFERMARE CANCELLAZIONE, QUALUNQUE ALTRO TASTO PER ANNULLARE: ";
            StrStream.Acq(confirmation);
            SetTextColor(RED_TEXT);
            if (confirmation == "y" || confirmation == "Y")
            {
                Alloggiati::PtAlloggiati[i]->AlloggiatoDeleteFigli();
                // Poichè hai cancellato un o più elementi il problema è che nella posizione -iesima
                // potrebbe non esserci piu quello che volevi cancellare: devi ricercarlo!
                if(Alloggiati::IsAlloggiatoTrovato(n,c,i))
                {
                    Alloggiati::PtAlloggiati[i]->AlloggiatoDelete(i);
                }
                else
                {
                    StrStream.SetError("\nErrore Memoria! CapoFamiglia o MembroGruppo non trovato dopo cancellazione del SottoMembro");
                }
            }
            else
            {
                cout <<"\nCANCELLAZIONE ANNULLATA" << endl;
            }
        }

    }
    else
    {
        SetTextColor(RED_TEXT);
        cout <<"\nALLOGGIATO " <<  c << " " << n << " NON TROVATO !" << endl;
        cout <<"\nCANCELLAZIONE ANNULLATA" << endl;
    }
    resetTextColor();
}

// Nome e Cognome nuovo alloggiato .
bool Alloggiati::InserisciNominativo ()
{
   bool finito= false;
   string confirmation;
    do
    {
        cout << "Inserire Cognome: " ;
        StrStream.Acq(this->Surname);
        if(this->Surname.size()>50u)
        {
            StrStream.SetError("\nLunghezza cognome non valido (maggiore di 50 caratteri) !\n");
            return false;
        }
        cout << "Inserire Nome   : " ;
        StrStream.Acq(this->Name);
        if(this->Name.size()>30u)
        {
            StrStream.SetError("\nLunghezza nome non valido (maggiore di 30 caratteri) !\n");
            return false;
        }
        SetTextColor(PINK_TEXT);
        cout << this->Surname << " " << this->Name;
        resetTextColor();
        cout << " inserito, premi Y per proseguire, E per uscire, qualunque altro tasto per modificare nominativo: ";
        StrStream.Acq(confirmation);
        if( (confirmation == "Y") || (confirmation == "y")  )
        {
         finito = true;
        }
        else if (confirmation == "E"||confirmation == "e")
        {
            break;
        }
    }
    while(!finito);
    if(!finito)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Inserimento data nascita nuovo allogiato .
bool Alloggiati::InserisciDataNascita ()
{
    typedef struct Data_1
    {
        int aa;
        int mm;
        int gg;
    } Data_1;

    Data_1 n, curr ;
    string bar = "/";
    string s;
    string gg,mm,aa;
    bool ok = false;
    string confirmation;

	// CAPTURE DELLA DATA CORRENTE .
    Datas * ptData = new Datas();
    ptData->Open();
    curr.gg = ptData->GetGiornoDataOdierna() ;
    curr.mm   = ptData->GetMeseDataOdierna();
    curr.aa   = ptData->GetAnnoDataOdierna();
    delete ptData;

    do
    {
        resetTextColor();
        cout << "\nInserire Data di nascita (formato gg/mm/aaaa): " << endl;
        ok = true;  /// Inserimento data .
        cout << "Inserire giorno : " ; StrStream.Acq(gg);
        cout << "Inserire mese   : " ; StrStream.Acq(mm);
        cout << "Inserire anno   : " ; StrStream.Acq(aa);

        n.gg= atoi(gg.c_str());
        n.mm= atoi(mm.c_str());
        n.aa= atoi(aa.c_str());

        if(n.gg== 0 || n.mm==0 || n.aa==0)
        {
        StrStream.SetError("Non hai inserito dei valori numerici accettabili!\n\7 "); ok = false; continue;
        }

        if(n.gg < 1 || n.gg > 31)
        {
          StrStream.SetError("Giorno non valido, inserire valore da 1 a 31: \n\7"); ok = false; continue;
        }

        if(n.mm < 1 || n.mm > 12)
        {
            StrStream.SetError("Mese non valido, inserire valore da 1 a 12: \n\7"); ok = false; continue;
        }
           ///Febbraio con   giorni oppure aprile/giugno/settembre/novembre con 31 gg
        if( (n.gg >29)&&(n.mm ==2))
        {
            StrStream.SetError("Giorno inserito incongruente con il mese!\n\7 "); ok = false; continue;
        }
        //oppure aprile/giugno/settembre/novembre con 31 gg
        else if ( (n.gg > 30)&&  ((n.mm==4)||(n.mm==6)||(n.mm==9)||(n.mm==11)) )
        {
            StrStream.SetError("Giorno inserito incongruente con il mese!\n\7 "); ok = false; continue;
        }

        if(n.aa < curr.aa - 95 )
        {
            if(n.aa < curr.aa - 105 )
            {
             StrStream.SetError("Con la data inserita l'alloggiato dovrebbe essere caput!\n\7 "); ok = false; continue;
            }

            SetTextColor(RED_TEXT); cout << "Alloggiato ha davvero"<< n.aa << "anni?" ;
            resetTextColor();
            cout<<  " Premi Y per confermare eta', E per uscire, qualunque altro tasto per modificare data: " ;
            StrStream.Acq(confirmation);
            if( (confirmation != "Y") && (confirmation != "y")  )
            {
                if(confirmation == "E"||confirmation == "e")
                {
                    return false;
                }
                else
                {
                    ok = false;
                    continue;
                }
            }
        }
        // anno futuro o anno corrente con mese futuro             mese ed anno correnti con giorno  futuro
        if(n.aa>curr.aa||((curr.mm< n.mm) &&(n.aa == curr.aa) ) || ((curr.mm == n.mm) && (n.aa == curr.aa) && (n.gg>=curr.gg )))
        {
            StrStream.SetError("Errore, hai inserito una data futura!\n\7"); ok = false; continue;
        }

        if(n.aa == curr.aa )
        {
            StrStream.SetError("Alloggiato ha meno di un anno!\n\7"); cout<<  "Premi Y per confermare eta', E per uscire, qualunque altro tasto per modificare data: " ;            StrStream.Acq(confirmation);
            if( (confirmation != "Y") && (confirmation != "y")  )
            {
                if(confirmation == "E"||confirmation == "e")
                {
                    return false;
                }
                else
                {
                    ok = false; continue;
                }
            }
        }
        s = intToString  (n.gg) + (string) bar + (string) intToString (n.mm) + (string) bar + (string) intToString (n.aa);
        cout <<"Data inserita: ";SetTextColor(PINK_TEXT);  cout << s ; resetTextColor(); cout<< " , premi Y per confermare eta', E per uscire, qualunque altro tasto per modificare data: " ;
        StrStream.Acq(confirmation);
        if( (confirmation != "Y") && (confirmation != "y")  )
        {
            if(confirmation == "E"||confirmation == "e")
            {
                return false;
            }
            else
            {
                ok = false;
                continue;
            }
        }
    }
    while(!ok);
    this->DataNascita = s;
    return true;
}

// Inserimento Sesso Alloggiato .
bool Alloggiati::InserisciSesso ()
{
    string sesso;

    cout << "Inserire M se maschio F (o altro tasto) se femmina: " ;
    StrStream.Acq(sesso);
    SetTextColor(GREEN_TEXT);
    if(sesso == "m" || sesso== "M" )
    {
        this->Sex = MASCHIO ;
    cout << "                                                       -> MASCHIO" ;
    }
    else
    {
        this->Sex  = FEMMINA ;
        cout << "                                                   -> FEMMINA" ;
    }
    resetTextColor();
    return true;
}
// Nazione nascita/cittadinanza nuovo alloggiato .
bool Alloggiati::InserisciNazione (string & nazione,string & nazioneCode, const char * destination)
{
    string confirmation;
    string statoChar;
    cout << "L'alloggiato e' " << destination <<  " in Italia? Y per confermare qualunque altro tasto se nato all'estero: " ;
    StrStream.Acq(confirmation);
    if( (confirmation != "Y") && (confirmation != "y")  )
    {
        if(NUM_CARATTERI_RICERCA_NAZIONE==1u)
        {
            cout << "Inserire la prima cifra dello stato: " ;
        }
        else
        {
            cout << "Inserire le PRIME ";
            cout << NUM_CARATTERI_RICERCA_NAZIONE;
            cout<< " cifre dello stato: " ;
        }
        StrStream.Acq(statoChar);
        if(!RicercaNazione(statoChar , NUM_CARATTERI_RICERCA_NAZIONE , nazione , nazioneCode))
        {
            return false;
        }

        // if NATO all'estero .
        if(destination=="NATO")
        {
            this->IsNatoEstero = true;
            this->ProvinciaNascita = UndefinedString;
            this->ComuneNascita = UndefinedString;
            this->ComuneNascitaCode = UndefinedString;
        }
    }
    else // ITALIA
    {
        nazione = "ITALIA";
        nazioneCode = CODE_ITALIA;
    }
    return true;
}
// Provincia nascita nuovo alloggiato .
bool Alloggiati::InserisciProvincia ()
{
    string confirmation;
    string statoChar;
    string provincia;

    cout << "Inserisci PROVINCIA DI NASCITA alloggiato (2 lettere): " ;
    StrStream.Acq(provincia);

    // Controlli
    if(RicercaProvincia ( provincia, 2u , this->ProvinciaNascita, this->ComuneNascita,  this->ComuneNascitaCode))
    {
        return true;
    }
    else
    {return false;}
}

// Quando inserisci tipo nuovo alloggiato, quando è Familiare/Membogruppo aggancia il CapoGruppo/CapoFamiglia .
bool Alloggiati::LinkaIlCapoAlloggiato()
{
    string n,c;
    uint16_t j;

    bool success = true;

    cout << "Inserendo Familiare o MembroGruppo e' obbligatorio indicare CapoFamiglia o CapoGruppo.\n";
    cout << "Indicare il COGNOME del Capofamiglia o Capogruppo: ";
    StrStream.Acq(c);
    cout << "Indicare il NOME del CapoFamiglia o CapoGruppo: ";
    StrStream.Acq(n);
    if(Alloggiati::IsAlloggiatoTrovato(n,c,j))
    {
        if(  (Alloggiati::PtAlloggiati[j]->Tipo == Alloggiati::CapoFamiglia && this->Tipo == Alloggiati::Famigliare )  ||
          (Alloggiati::PtAlloggiati[j]->Tipo == Alloggiati::CapoGruppo && this->Tipo == Alloggiati::MembroGruppo ))
        {
            cout << "Corrispondenza trovata con il seguente alloggiato:\n";
            Alloggiati::PtAlloggiati[j]->StampaSingoloAlloggiato();
            this->IsFamiliare = true;

            this->NomeCapo = n;
            this->CognomeCapo = c;
            cout << "Prosegui con i dati di ";
            StrStream.SetInfo(this->Surname ); cout<<" "; StrStream.SetInfo(this->Name );
        }
        else
        {
            StrStream.SetError("\nL'alloggiato esiste, ma e' incoerente nel tipo!");
            StrStream.SetError(" Per es: hai scelto OspiteSingolo o Capogruppo per un tipo Familiare.");
            success = false;
        }
    }
    else
    {
        success = false;
        SetTextColor(RED_TEXT);
        cout <<"\nALLOGGIATO Capofamiglia/Capogruppo " << c << " " << n << " NON TROVATO !" << endl;
        success = false;
        this->IsFamiliare = false;

        this->NomeCapo = BLANK;
        this->CognomeCapo = BLANK;
    }
    return success;
}

// Tipo Nuovo alloggiato .
bool Alloggiati::InserisciTipoAlloggiato (void)
{
    int i;
    string typeString;
    Alloggiati::TipoAlloggiato type;
    bool finito;
    stringstream ss[Alloggiati::MaxAlloggiatoTypeNum];
    string str,confirmation;

    cout << "\nINSERIMENTO TIPOLOGIA ALLOGGIATO:\n\n" ;

    for( i = 0 ; i< (int) Alloggiati::MaxAlloggiatoTypeNum ; i++ )
    {
        ss[i] << (int) AlloggiatoClassStringTable[i].AlloggiatoClass;
        ss[i] >> str;
        cout << str << " per " << AlloggiatoClassStringTable[i].ClassString << endl;
        str.clear();
    }

    cout << "\n";
    do
    {
        finito = true;
        cout << "Inserisci il numero corrispondente: " ;
        StrStream.Acq(typeString);
        type = (Alloggiati::TipoAlloggiato) atoi(typeString.c_str());

        if((type>Alloggiati::MaxAlloggiatoTypeCode)||(type<Alloggiati::MinAlloggiatoTypeCode))
        {
            StrStream.SetError("Tipologia alloggiato inserita fuori range\7\n");
            cout << "Inserire E per uscire ed annullare, altro tasto per re-inserire il tipo alloggiato: ";
            StrStream.Acq(confirmation);
            if (confirmation == "E" ||  confirmation == "e")
            {
                return false;
            }
            else
            {
                finito = false; continue;
            }
        }
        else
        {
            this->Tipo = type;
        }
    }
    while (!finito);

    if( (this->Tipo == Alloggiati::Famigliare) || (this->Tipo == Alloggiati::MembroGruppo) )
    {
      this->IsFamiliare = true;
      if(!this->LinkaIlCapoAlloggiato()) /// cerca di linare il familiare al capfamiglia
      {
         StrStream.SetError("\nNOTA: inserire Capofamiglia o Capogruppo PRIMA dei relativi accompagnatori \n\n");
         return false;
      }
    }
    else
    {
      IsFamiliare = false;
    }
    return true;
}

// Tipo Doc Alloggiato .
bool Alloggiati::InserisciTipoDocumentoAlloggiato (void)
{
    string docString, s,docNumString, confirmation;
    int const maxDocNum = 120;
    string CodeTmp[maxDocNum];
    string DocTmp[maxDocNum];
    int doc;
    ifstream f  ("CodeToDocumento.txt");
    int cnt = 0;
    bool success,finito;

    cout << "\nINSERIMENTO TIPOLOGIA DOCUMENTO ALLOGGIATO:\n\n" ;
    SetTextColor(GREEN_TEXT);
    system ("pause");
    resetTextColor();

    if(!f)
    {
        StrStream.SetError("Il file non esiste!\n\7");
        success = false;
        return success;
    }
    while(f.good()&& cnt<maxDocNum )    // fino a quando c'è qualcosa da leggere
    {
        stringstream ss;
        string str;

        getline(f, s); // legge tutta la riga dal file e la mette nella variabile s

        char c = s[0u];
        if(!s.empty() && (c!=0x20))
        {
            ss << (cnt +1);
            ss >> str;
            istringstream iss(s);
            iss >> CodeTmp[cnt];
            int index = s.find_first_not_of (CodeTmp[cnt]) + 1u ;  // Indice del primo elemento della stringa che non contiene il codice
            s = &s[index];  // ti punti sul nome dello stato purchè sia spaziato di un carattere dal codice
            DocTmp[cnt] = s;
            if( CodeTmp[cnt] == CODE_PATENTE ||  CodeTmp[cnt] == CODE_PASSAPORTO ||
                    CodeTmp[cnt] ==CODE_CARTA_ID ||  CodeTmp[cnt] == CODE_CARTA_ID_ELETT )
            {
                SetTextColor(PINK_TEXT);
                cout << str  << " --> " <<  DocTmp[cnt] << endl;
                resetTextColor();
            }
            else
            {
                cout << str  << " --> " <<  DocTmp[cnt] << endl;
            }
            cnt++;
            str.clear();
        }
    }
    f.close();

    do
    {
        finito = true;
        cout << "\nInserisci il numero corrispondente al documento ( es. 1,2,3, ...):" ;
        StrStream.Acq(docString);
        doc = atoi(docString.c_str());

        if(doc <1||(doc > cnt ))
        {
            StrStream.SetError("Tipologia documento alloggiato inserita fuori range\7\n");
            cout << "Inserire E per uscire ed annullare, altro tasto per ricominciare inserimento documento: ";
            StrStream.Acq(confirmation);
            if (confirmation == "E" ||  confirmation == "e")
            {
                return false;
            }
            else
            {
                finito = false; continue;
            }
        }
        else
        {
            doc--;
            this->DocumentoTypeCode = CodeTmp[doc] ;

            // Lettura numero e località del documento:
            cout << "Inserire numero ";
            SetTextColor(PINK_TEXT);
            cout <<   DocTmp[doc];
            resetTextColor();
            cout << " (max 20 caratteri): ";
            StrStream.Acq(docNumString);
            if(docNumString.size()>20u )
            {
                StrStream.SetError("Cifre documento fuori range\7\n");
                cout << "Inserire E per uscire ed annullare, altro tasto per re-inserire tipo di documento: ";
                StrStream.Acq(confirmation);
                if (confirmation == "E" ||  confirmation == "e")
                {
                    return false;
                }
                else
                {
                    finito = false;
                    continue;
                }
            }
            else
            {
                this->DocumentoString = docNumString;
                if(!this->InserisciLocalitaRilascioDoc())
                {
                return false;
                }
                else{ /*OK*/}
            }
        }
    }
    while (!finito);

    return true;
}

// Inserimento Località Rilascio documento
bool Alloggiati::InserisciLocalitaRilascioDoc (void)
{
    string confirmation;
    string provincia, provinciaDoc, ComuneDoc, statoChar;
    cout << "Documento RILASCIATO in ITALIA? Inserire Y per confermare ITALIA, altro tasto se rilasciato all'ESTERO: ";
    StrStream.Acq(confirmation);
    if (confirmation == "Y" ||  confirmation == "y")
    {
        // ITALIA
        cout << "Hai scelto ITALIA come stato di rilascio documento. Inserire PROVINCIA di rilascio del documento (2 lettere): ";
        StrStream.Acq(provincia);
        ///Controlli
        if(!RicercaProvincia(provincia,2u,provinciaDoc,this->Comune_o_StatoRilascioDocumento,this->LocalitaRilascioDocumentoCode))
        {
            return false;
        }
    }
    else
    {
        // ESTERO
        cout << "Localita' estera rilascio documento. ";
        if(NUM_CARATTERI_RICERCA_NAZIONE==1u)
        {
            cout << "Inserire la prima cifra dello stato: " ;
        }
        else
        {
            cout << "Inserire le PRIME ";
            cout << NUM_CARATTERI_RICERCA_NAZIONE;
            cout<< " cifre dello stato: " ;
        }
        StrStream.Acq(statoChar);
        if(!RicercaNazione(statoChar , NUM_CARATTERI_RICERCA_NAZIONE , this->Comune_o_StatoRilascioDocumento,this->LocalitaRilascioDocumentoCode))
        {
        return false;
        }
    }
    return true;
}

// Inserimento Alloggiati
bool Alloggiati::InserisciAlloggiato (Alloggiati * AlloggPt)
{
    string name, surname, DataN;
    string sesso;
    string nazione,nazioneCode;
    string confirmation;

    if(!AlloggPt->InserisciNominativo())
    {
        return false;
    }

    AlloggPt->InserisciSesso();

    if(!AlloggPt->InserisciDataNascita())
    {
        return false;
    }

    if(!AlloggPt->InserisciTipoAlloggiato())
    {
        return false;
    }

    cout << "\nINSERIMENTO STATO DI NASCITA ALLOGGIATO:\n\n" ;
    if(!AlloggPt->InserisciNazione(AlloggPt->StatoNascita, AlloggPt->StatoNascitaCode,"NATO") )
    {
        return false;
    }

    // SOLO SUGLI ITALIANI
    if(!AlloggPt->IsNatoEstero)
    {
        cout << "Inserimento PROVINCIA di NASCITA Alloggiato\n" ;
        if(!AlloggPt->InserisciProvincia())
        {
            return false;
        }
    }
    else
    {
        AlloggPt->ComuneNascita = UndefinedString;
        AlloggPt->ComuneNascitaCode = UndefinedString;
    }

    cout << "INSERIMENTO STATO DI CITTADINANZA ALLOGGIATO:\n\n" ;
    if(!AlloggPt->InserisciNazione(AlloggPt->StatoCittadinanza, AlloggPt->StatoCittadinanzaCode,"CITTADINO") )
    {
        return false;
    }

    if(!AlloggPt->IsFamiliare)
    {
        if(!AlloggPt->InserisciTipoDocumentoAlloggiato())
        {
            return false;
        }
    }
    else
    {
      AlloggPt->DocumentoTypeCode = BLANK;
      AlloggPt->DocumentoString = BLANK;
      AlloggPt->Comune_o_StatoRilascioDocumento  = BLANK;
    }

    // Conferma
    cout << "Inserito seguente alloggiato:\n";
    AlloggPt->StampaSingoloAlloggiato();
    cout << "\nPremere Y per confermare inserimento, altro tasto per annullare:";

    StrStream.Acq(confirmation);
    if (confirmation != "Y" &&  confirmation != "y")
    {
        return false;
    }
    // Costruisci il super Stream
    AlloggPt->SuperStreamUpdate();

    return true;
}

// Inserimento ordinato .
bool Alloggiati::InserimentoOrdinatoAlloggiati (void)

{
    int i,k;
    bool trovato = false;
    for(i=0; i<Alloggiati::AllN ; i++)
    {
        if(  caseInsensitiveStringCompare(PtAlloggiati[i]->Surname,this->Surname) )
        {
            if(caseInsensitiveStringCompare(PtAlloggiati[i]->Name,this->Name) )
            {
                StrStream.SetError("Alloggiato gia' presente, cancellarlo prima di re-inserirlo\n");
                return false;
            }
        }

        // Crei oggetti
        char * s1 = new char [this->Surname.length()+1];
        char * s2 = new char [PtAlloggiati[i]->Surname.length()+1];
        char * n1 = new char [this->Name.length()+1];
        char * n2 = new char [PtAlloggiati[i]->Name.length()+1];

        // Copy dalle stringhe (necessario perchè con = si modificano le stringhe originali)
        strcpy(s1,this->Surname.c_str() );
        strcpy(s2,PtAlloggiati[i]->Surname.c_str() );
        strcpy(n1,this->Name.c_str() );
        strcpy(n2,PtAlloggiati[i]->Name.c_str() );

        // Converti tutto
        for(uint16_t p = 0; p < strlen(s1); p++)
        {
            s1[p] = toupper(s1[p]);
        }

        for(uint16_t p = 0; p < strlen(s2); p++)
        {
            s2[p] = toupper(s2[p]);
        }
        for(uint16_t p = 0; p < strlen(n1); p++)
        {
            n1[p] = toupper(n1[p]);
        }
        for(uint16_t p = 0; p < strlen(n2); p++)
        {
            n2[p] = toupper(n2[p]);
        }

        if( (strcmp(s1,s2)<0) || ((strcmp(s1,s2)==0)&& (strcmp(n1,n2)<0)) )/// la prima precede la seconda
        {
            Alloggiati::AllN++;
            for(k=Alloggiati::AllN -1 ; k>i ; k--)
            {
                Alloggiati::PtAlloggiati[k] = Alloggiati::PtAlloggiati[k-1] ;
            }
            Alloggiati::PtAlloggiati[i] = this;
            trovato = true;
            break;

        }

    delete [] s1;
    delete [] s2;
    delete [] n1;
    delete [] n2;
    }
    if(!trovato)
    {
        Alloggiati::PtAlloggiati[Alloggiati::AllN ++] = this;
    }
    return true;
}

// Aggiorna il file interno degli alloggiati con il buffer attuale .
bool  Alloggiati:: FileAlloggiatiUpdate (void)
{
    char * filename = "Alloggiati.txt";
    ofstream fout ( filename );

    string super;

    if ( ! fout )
    {
        cerr << " can't open input - " << filename <<
             endl;
        return false;
    }
    for(int i= 0; i<Alloggiati::AllN; i++ )
    {
        super +=  Alloggiati::PtAlloggiati[i]->superStream;
        super += "\n";
    }
    fout << super << endl;
    fout.close();

    return true;
}

// Aggiornamento superstream .
bool  Alloggiati:: SuperStreamUpdate (void)
{
    string sessoString , TipoString;
    string UndefStr = UndefinedString;

    if(Sex ==MASCHIO)
    {
        sessoString = "M";
    }
    else
    {
        sessoString = "F";
    }

    TipoString = intToString((int) this->Tipo);

    superStream = TipoString + " " + Name + " " + Surname + " " + sessoString + " " + DataNascita + " ";
    superStream += StatoNascitaCode + " " + StatoCittadinanzaCode + " " + ComuneNascitaCode + " ";
    superStream += ProvinciaNascita + " " ;
    if(!IsFamiliare)
    {
    superStream +=  DocumentoTypeCode + " " + DocumentoString + " " + LocalitaRilascioDocumentoCode;
    }
    else
    {
     superStream += NomeCapo + " " + CognomeCapo;
    }

    return true;
}

// Aggiorna tutti i puntatori del buffer dei padri con i rispettivi figli se ci sono .
bool Alloggiati::LinkPuntatoriPadri (void)
{

  for( uint16_t i = 0; i< Alloggiati::AllN; i++)
  {
      for( uint16_t j = 0; j<MAX_DISCENDENTI ; j++)
      {
          Alloggiati::PtAlloggiati[i]->pt_AllFigli[j]= NULL;
          Alloggiati::PtAlloggiati[i]-> pt_AllFigliCnt = 0;
      }
      Alloggiati::PtAlloggiati[i]->pt_capo = NULL;
  }

// Puntatori ai figli .
  for( uint16_t i = 0; i< Alloggiati::AllN; i++)
  {
     if(  (Alloggiati::PtAlloggiati[i]->Tipo == Alloggiati::CapoFamiglia) ||
           (Alloggiati::PtAlloggiati[i]->Tipo == Alloggiati::CapoGruppo) )
     {
         for( uint16_t j = 0; j< Alloggiati::AllN; j++)
         {
            if( caseInsensitiveStringCompare( Alloggiati::PtAlloggiati[i]->Surname, Alloggiati::PtAlloggiati[j]->CognomeCapo))
               {
                 if(  caseInsensitiveStringCompare( Alloggiati::PtAlloggiati[i]->Name, Alloggiati::PtAlloggiati[j]->NomeCapo))
                    {
                       Alloggiati::PtAlloggiati[i]->pt_AllFigli[ Alloggiati::PtAlloggiati[i]->pt_AllFigliCnt++] = Alloggiati::PtAlloggiati[j] ;
                    }
               }
         }
     }
  }

    // Puntatori verso i padri .
    for( uint16_t i = 0; i< Alloggiati::AllN; i++)
    {
        if(  (Alloggiati::PtAlloggiati[i]->IsFamiliare ))
        {
            for( uint16_t j = 0; j< Alloggiati::AllN; j++)
            {
                if( caseInsensitiveStringCompare( Alloggiati::PtAlloggiati[i]->CognomeCapo, Alloggiati::PtAlloggiati[j]->Surname))
                {
                    if(  caseInsensitiveStringCompare( Alloggiati::PtAlloggiati[i]->NomeCapo, Alloggiati::PtAlloggiati[j]->Name))
                    {
                        Alloggiati::PtAlloggiati[i]->pt_capo = Alloggiati::PtAlloggiati[j] ;
                    }
                }
            }
        }
    }

    // Debug

#if 0
    for( uint16_t i = 0; i< Alloggiati::AllN; i++)
    {
        for( uint16_t j = 0; j<MAX_DISCENDENTI ; j++)
        {
            if(Alloggiati::PtAlloggiati[i]->pt_AllFigli[j]!=NULL)
            {
                cout << Alloggiati::PtAlloggiati[i]->Surname << " " << Alloggiati::PtAlloggiati[i]->Name;
                cout << " Capi di ";
                cout << Alloggiati::PtAlloggiati[i]->pt_AllFigli[j]->Surname;
                cout << " ";
                cout << Alloggiati::PtAlloggiati[i]->pt_AllFigli[j]->Name << endl;
            }
        }
    }

    for( uint16_t i = 0; i< Alloggiati::AllN; i++)
    {
        if(Alloggiati::PtAlloggiati[i]->pt_capo!=NULL)
        {
            cout << Alloggiati::PtAlloggiati[i]->Surname << " " << Alloggiati::PtAlloggiati[i]->Name;
            cout << " e'  Familiare di ";
            cout << Alloggiati::PtAlloggiati[i]->pt_capo->Surname;
            cout << " ";
            cout << Alloggiati::PtAlloggiati[i]->pt_capo->Name << endl;

        }
    }
#endif

   return true;
}

///******************************************************************************************///
///*************************************PARTE DI REGISTRAZIONE ******************************///
///******************************************************************************************///

const uint16_t RegMax = 250u;
static uint8_t poolCnt = 0;
static Alloggiati * pt_regist[RegMax];
static string dArrivo;
int DayPermanenza = 0;

// Nome e Cognome nuovo alloggiato .
bool Alloggiati::RegistraNominativo (string & n ,string & c, uint16_t number)
{
    cout << "Inserire Cognome Alloggiato " << (number +1) << " da registrare : " ;
    StrStream.Acq(c);
    if(c.size()>50u)
    {
        StrStream.SetError("\nLunghezza cognome non valido (maggiore di 50 caratteri) !\n");
        return false;
    }
    cout << "Inserire Nome Alloggiato " << (number + 1) <<  "  da registrare   : " ;
    StrStream.Acq(n);
    if(n.size()>30u)
    {
        StrStream.SetError("\nLunghezza nome non valido (maggiore di 30 caratteri) !\n");
        return false;
    }

    return true;
}

// Methodo per la gestione del'inserimento dell'alloggiato nel pool di regisrazione .
bool Alloggiati::AlloggiatoPoolInsertionManagement( uint16_t Index )
{
    bool success = true;
    string confirmation;



    if(Alloggiati::PtAlloggiati[Index]==NULL)
    {
        StrStream.SetError("Puntatore non valido!\n");
        return false;
    }

    // non è ancora stat rievato questo caso particolare
    Alloggiati::PtAlloggiati[Index]->IsCapoFamigliaSingolo = false;

    if( Alloggiati::PtAlloggiati[Index]->IsFamiliare == true)
    {
        // Se  è un familiare registri tutta la famiglia .
        cout << "\nHai inserito un alloggiato che fa parte di Famiglia/Gruppo.\n";
        cout << "Verra' registrata/o l'intera/o famiglia/gruppo.\n\n";

        ///  Controlla che il capo non sia già stato inserito singolarmente .
        bool trovato = false;
        for(uint16_t i =0; i<poolCnt; i++ )
        {
            if (caseInsensitiveStringCompare(pt_regist[i]->Surname,PtAlloggiati[Index]->pt_capo->Surname) )
            {
                if (caseInsensitiveStringCompare(pt_regist[i]->Name,PtAlloggiati[Index]->pt_capo->Name))
                {
                    trovato = true;
                }
            }
        }

        if(!trovato)
        {
            // Inserimento capo
            pt_regist[poolCnt] = Alloggiati::PtAlloggiati[Index]->pt_capo ;
            pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);
            pt_regist[poolCnt]->IsCapoFamigliaSingolo = false;
            poolCnt++;
        }
        else
        {
            StrStream.SetError("Inserimento non valido: il relativo capogruppo o capofamiglia gia' inserito!\n");
            return false;
        }

           // Inserimento familiari
        for(uint8_t i = 0; i< Alloggiati::PtAlloggiati[Index]->pt_capo->pt_AllFigliCnt; i++)
        {
          pt_regist[poolCnt] =  Alloggiati::PtAlloggiati[Index]->pt_capo->pt_AllFigli[i];
          pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);
          pt_regist[poolCnt]->IsCapoFamigliaSingolo = false;
          poolCnt++;
        }
    }
    else  // Se non è familiare
    {
        if ((Alloggiati::PtAlloggiati[Index]->Tipo != Alloggiati::CapoFamiglia) && ((Alloggiati::PtAlloggiati[Index]->Tipo != Alloggiati::CapoGruppo)))
        {
            // Se singolo .
            pt_regist[poolCnt] = Alloggiati::PtAlloggiati[Index];
            pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);
            pt_regist[poolCnt]->IsCapoFamigliaSingolo = false;
            poolCnt++;
        }
        else
        {
            // Se è un Capofamiglia avvisa che puoi registrerai solo lui o tutta la famiglia.
            if(Alloggiati::PtAlloggiati[Index]->pt_AllFigliCnt != 0)
            {
                cout << "\nHai inserito un alloggiato Capo di Famiglia o Gruppo. \nInserire Y per confermare registrazione dell'intera gruppo/famiglia, ";
                cout << "altro tasto per eseguire registrazione singola : ";
                StrStream.Acq(confirmation);
                if (confirmation == "Y" ||  confirmation == "y")
                {
                    pt_regist[poolCnt] = Alloggiati::PtAlloggiati[Index];
                    pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);
                    poolCnt++;
                    // registra famiglia .
                    for(uint8_t i = 0; i< Alloggiati::PtAlloggiati[Index]->pt_AllFigliCnt; i++)
                    {
                        pt_regist[poolCnt] =  Alloggiati::PtAlloggiati[Index]->pt_AllFigli[i];
                        pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);
                        poolCnt++;
                    }
                }
                else
                {
                    pt_regist[poolCnt] = Alloggiati::PtAlloggiati[Index];
                    pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);

                    // CASO particolare in cui registrerai un capofmiglia come singolo
                    pt_regist[poolCnt]->IsCapoFamigliaSingolo = true;
                    poolCnt++;

                }
            }
            else
            {
            // CAPO FAMIGLIA da registrare come singolo
            StrStream.SetInfo("Nota: registrazione di capo che attualmente non ha membri o familiari.\n");
            pt_regist[poolCnt] = Alloggiati::PtAlloggiati[Index];
            pt_regist[poolCnt]->SetGiorniPermanenza(DayPermanenza);

            // CASO particolare in cui registrerai un capofmiglia come singolo
            pt_regist[poolCnt]->IsCapoFamigliaSingolo = true;
            poolCnt++;

            }
        }
    }
    return success;
}

bool Alloggiati::RichiediDatiRegistrazione ()
{
    bool success = true;
    bool finito = true;
    string DayPermanenzaString;

    cout << "\nNOTA: la data di arrivo verra' impostata in data odierna\n";

    do
    {
        cout << "\nInserire giorni di permanenza dell'alloggiato e dei suoi accompagnatori: ";
        StrStream.Acq(DayPermanenzaString);

        DayPermanenza= atoi(DayPermanenzaString.c_str());

        if(DayPermanenza> 0 && DayPermanenza< 31)
        {
         finito = true;
        }
        else
        {
            StrStream.SetError("\nGiorni di permanenza non validi (al massimo sono ammessi 30 gg)!\n");
            finito = false;
        }
    }
    while (!finito);

    return success ;
}

// Menu di registrazione
bool Alloggiati::RegistraAlloggiatiMenu()
{
   string NomeFileRegistrazione;
   string nome, cognome;
   char r;
   bool finito = false;
   uint16_t Index = 0;

   if (system("CLS")) system("clear");
   cout << "MENU' PER LA REGISTRAZIONE DI UNO O PIU' ALLOGGIATI\n";

   // Selezione del nome del file di output .
   Datas * ptData = new Datas();
   ptData->Open();
   NomeFileRegistrazione = "RegistraAlloggiati_" + ptData->GetStringFileData();

   dArrivo = ptData->GetStringData();

   if(ptData!=NULL)
   {
       delete ptData;
       ptData = NULL;
   }


   // Init the pool number .
   poolCnt = 0;
   for(uint16_t i =0; i < RegMax ; i++)
   {
     pt_regist[i] = NULL;
   }

    // Selezione degli alloggiati da inserire nel pool .
    while(!finito && poolCnt < RegMax)
    {
        // Aggiorna con un mini report.
        cout << "\nALLOGGIATI ATTUALMENTE CARICATI PER LA REGISTRAZIONE:\n" ;
        SetTextColor(PINK_TEXT);
        for(uint16_t i =0; i<poolCnt; i++ )
        {
            cout << pt_regist[i]->Surname << " " << pt_regist[i]->Name << endl;
        }
        resetTextColor();

        cout << "\nDigita:";
        cout << "\n- Y per aggiungere Allogg. nella registrazione odierna";
        cout << "\n- R per procede con registrazione Allogg. attualmente inseriti";
        cout << "\n- Qualunque altro tasto per annullare registrazione\n";
        r=getch();

        if (r == 'Y' ||  r == 'y')
        {
            finito = false;
        }
        else if (r == 'R' ||  r == 'r')
        {
            if(poolCnt!=0u)
            {
             finito = true;
            }
            else
            {
            StrStream.SetError("Non ci sono elementi da registrare!\n");
            return false;
            }
        }
        else
        {
            return false;
        }

        if(!finito)
        {
            // Richiedi Nome e Cognome Alloggiato da Inserire.
            if(!RegistraNominativo(nome, cognome, poolCnt))
            {
                finito = false;
                continue;
            }

            // Controlla che non sia già presente nel tuo pool di registrazione
            bool trovato = false;
            for(uint16_t i =0; i<poolCnt; i++ )
            {
                if (caseInsensitiveStringCompare(pt_regist[i]->Surname,cognome) )
                {
                    if (caseInsensitiveStringCompare(pt_regist[i]->Name,nome))
                    {
                        trovato = true;
                    }
                }
            }

            if(trovato)
            {
                StrStream.SetError("Alloggiato gia' inserito!\n");
                finito = false;
                continue;
            }

            // Richiedi Giorni di permanenza e data di arrivo.
            if(!RichiediDatiRegistrazione())
            {
               finito = false; continue;
            }

            // Ricerca
            if(Alloggiati::IsAlloggiatoTrovato (nome, cognome, Index))
            {
            // Gestione dell'inserimento dell'alloggiato .
                 if(!Alloggiati::AlloggiatoPoolInsertionManagement(Index))
                 {
                     finito = false;
                     continue;
                 }
            }
            else
            {
                StrStream.SetError("Alloggiato non trovato!\n");
                finito = false;
                continue;
            }
        }
    }

    // Per tutto il pool devi generare un superstream e aggiornare file .
    if( poolCnt > 0u)
    {
          if(AlloggiatiRegistraStream(NomeFileRegistrazione))
          {

          }
          else
          {
            return false;
          }
    }
    else
    {
      StrStream.SetError("Non hai registrato nessuno!\n"); return false;
    }
	
    //  Resetta la data di arrivo ed i giorni di permamenenza tramite il  pool .

    cout << "Registrazione avvenuta correttamente. \nIl file da utilizzare si chiama: ";
    StrStream.SetInfo(NomeFileRegistrazione);
    return true;
}

bool Alloggiati::AlloggiatiRegistraStream(string f )
 {

    string format = ".txt";
    string s;

    s =  f + format;
    const char * filename = s.c_str();

    ofstream fout ( filename );
    if ( ! fout )
    {
        cerr << " can't open input - " << filename <<  endl;
        StrStream.SetError("Impossibile aprire file di output!\n");
        return false;
    }

     char c[100];
     for(uint16_t i =0; i<poolCnt; i++ )
     {
        // TIPO ALLOGGIATO 2

        cleanbuffer(c,100);
        if(pt_regist[i]->IsCapoFamigliaSingolo ==true)
        {
          s = intToString (Alloggiati::OspiteSingolo);
        }
        else
        {
         s = intToString (pt_regist[i]->Tipo);
        }
        strcpy(c, s.c_str());
        fout.write(c, 2);

        // DATA ARRIVO 10
        cleanbuffer(c,100);
        string data_arrivo = dArrivo;
        convertDatawithZero(data_arrivo);
        strcpy(c, data_arrivo.c_str() );
        fout.write(c, 10);

        // GIORNI PERMANENZA 2
        cleanbuffer(c,100);
        int gPermanenza = pt_regist[i]->GetGiorniPermanenza();
        if(gPermanenza < 10)
        {
          string tmp = intToString (pt_regist[i]->GetGiorniPermanenza());
          s[0] = '0';
          s[1] = tmp[0];
        }
        else
        {
           s = intToString (pt_regist[i]->GetGiorniPermanenza());
        }
        strcpy(c, s.c_str());
        fout.write( c, 2);

        // COGNOME ALLOGGIATO 50
        cleanbuffer(c,100);
        s = pt_regist[i]->Surname; /// Tutti i cognomi Maiuscoli (prima lettera)
        s[0] = toupper(s[0]);
        strcpy(c, s.c_str());
        fout.write( c, 50);

        // NOME ALLOGGIATO 30
        cleanbuffer(c,100);
        s = pt_regist[i]->Name;
        s[0] = toupper(s[0]);      /// Tutti i nomi Maiuscoli (prima lettera)
        strcpy(c, s.c_str());
        fout.write( c, 30);

        // SESSO ALLOGGIATO 1
        cleanbuffer(c,100);
        if (pt_regist[i]->Sex == MASCHIO)
        {
          c[0] = '1';
        }
        else
        {
          c[0] = '2';
        }
        fout.write( c, 1);

        // DATA NASCITA 10
        cleanbuffer(c,100);
        string data_nascita = pt_regist[i]->DataNascita;
        convertDatawithZero(data_nascita);
        strcpy(c, data_nascita.c_str());
        fout.write( c, 10);

        // COMUNE NASCITA 9
        cleanbuffer(c,100);
        if( !(pt_regist[i]->IsNatoEstero))
        {
          strcpy(c, pt_regist[i]->ComuneNascitaCode.c_str());
        }
        else
        {
            // 9 spazi blank
        }
        fout.write( c, 9);

        // PROVINCIA NASCITA 2
        cleanbuffer(c,100);
        if( !(pt_regist[i]->IsNatoEstero))
        {
          strcpy(c, pt_regist[i]->ProvinciaNascita.c_str());
        }
        else
        {
            // 2 spazi blank
        }
        fout.write( c, 2);

        // STATO NASCITA 9 (sia estero che italiano)
        cleanbuffer(c,100);
        strcpy(c, pt_regist[i]->StatoNascitaCode.c_str());
        fout.write( c, 9);

        // STATO CITTADINAZA 9 (sia estero che italiano)
        cleanbuffer(c,100);
        strcpy(c, pt_regist[i]->StatoCittadinanzaCode.c_str());
        fout.write( c, 9);

        // PARTE OPZIONALE
        if(pt_regist[i]->IsFamiliare)
        {
        cleanbuffer(c,100);
        fout.write( c, 34);
        }
        else
        {
            // CODIFICA TIPO DOCUMENTO 5
            cleanbuffer(c,100);
            strcpy(c, pt_regist[i]->DocumentoTypeCode.c_str());
            fout.write( c, 5);

            // NUMERO DOCUMENTO 20
            cleanbuffer(c,100);
            strcpy(c, pt_regist[i]->DocumentoString.c_str());
            fout.write( c, 20);

            // LOCALITà RILASCIO DOCUMENTO 9
            cleanbuffer(c,100);
            strcpy(c, pt_regist[i]->LocalitaRilascioDocumentoCode.c_str());
            fout.write( c, 9);
        }

        // resetta il "caso  particolare".
        pt_regist[i]->IsCapoFamigliaSingolo = false;

        // resetta i giornidi permanenza .
        pt_regist[i]->GiorniPermanenza = 0;

        if (i!= (poolCnt -1))
        {
            c[0] = '\n';
            fout.write( c, 1);
        }
     }
    fout.close();
    return true;
 }
