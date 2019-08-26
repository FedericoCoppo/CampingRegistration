#include "Console.h"

/***********OverLoad Routine per il settaggio colori console *****************/
BOOL SetTextColor(WORD c)
{
    HANDLE hCons;
    hCons = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD   wAttr;
    wAttr = c;
    SetConsoleTextAttribute(hCons, wAttr);

    return true;
}

BOOL resetTextColor(void)
{
    HANDLE hCons;
    hCons = GetStdHandle(STD_OUTPUT_HANDLE);
    return SetConsoleTextAttribute(hCons,WHITE_FADE_TEXT);
}
/***********Routine per il settaggio colori console *****************/
BOOL resetTextColor(HANDLE h)
{
    return SetConsoleTextAttribute(h,WHITE_FADE_TEXT);
}

BOOL setTextColor(HANDLE h, WORD c)
{
    return SetConsoleTextAttribute(h,c);
}

