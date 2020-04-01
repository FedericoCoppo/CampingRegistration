## Overview
The scope of this prog is manage people accomodation and registration in a camping during holiday as requested by italian regulation: 
[https://alloggiatiweb.poliziadistato.it/PortaleAlloggiati].

## Description
The program has been developed in CodeComposer environment several years ago, just to approach to C++, and it contains several architecture errors and unfortunately has been written in Italian.
However it has been used succesfully for some years to save time during holidaymakers registration. From 2019 the new accomodation portal registration of Polizia di Stato 
allow to memorize data of holidaymakers that has been already registered, so this tool has become unuseful and outdated.   
The scope is only generate a file containing info of holidaymakers that come back in camping every summer as a sort of database.
The data of this holidaymakers will be insert only once in this file, and every time is needed to register this people, 
the prog allow to generate the registration file (that can be sent online instead of manually reinsert all data via web portal). 
For specification about registration file format see

	- "Manuale Utente"[https://alloggiatiweb.poliziadistato.it/PortaleAlloggiati/SupManuali.aspx]
	- "Documento tecnico Creazione File" [https://alloggiatiweb.poliziadistato.it/PortaleAlloggiati/SupManuali.aspx].

Interface example
```
Mon Aug 26 18:53:51 2019

Digita: -'ESC' per uscire dal programma
        -'p' per leggere l'elenco completo degli Alloggiati
        -'s' per leggere dati del singolo Alloggiato
        -'i' per inserire un nuovo Alloggiato
        -'c' per cancellare un Alloggiato
        -'r' per registrare un numero variabile di Alloggiati

Sono Presenti in lista 4 Alloggiati su 1000 disponibili:

Bianchi Filippo
Bianchi Luca
Rossi Paolo
Verdi Mario

Premere un tasto per continuare . . .
```
