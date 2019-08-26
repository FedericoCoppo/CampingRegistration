#ifndef CONSOLE_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "string.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;
/*******************************************enumerativo colori console**********************************************/
enum COLOR
 {
     // Text foreground colors
     // Standard text colors
     GRAY_TEXT=8,      BLUE_TEXT,       GREEN_TEXT,
     TEAL_TEXT,        RED_TEXT,        PINK_TEXT,
     YELLOW_TEXT,      WHITE_TEXT,
     // Faded text colors
     BLACK_TEXT=0,     BLUE_FADE_TEXT,  GREEN_FADE_TEXT,
     TEAL_FADE_TEXT,   RED_FADE_TEXT,   PINK_FADE_TEXT,
     YELLOW_FADE_TEXT, WHITE_FADE_TEXT,
     // Standard text background color
     GRAY_BACKGROUND=GRAY_TEXT<<4,     BLUE_BACKGROUND=BLUE_TEXT<<4,
     GREEN_BACKGROUND=GREEN_TEXT<<4,   TEAL_BACKGROUND=TEAL_TEXT<<4,
     RED_BACKGROUND=RED_TEXT<<4,       PINK_BACKGROUND=PINK_TEXT<<4,
     YELLOW_BACKGROUND=YELLOW_TEXT<<4, WHITE_BACKGROUND=WHITE_TEXT<<4,
     // Faded text background color
     BLACK_BACKGROUND=BLACK_TEXT<<4,           BLUE_FADE_BACKGROUND=BLUE_FADE_TEXT<<4,
     GREEN_FADE_BACKGROUND=GREEN_FADE_TEXT<<4, TEAL_FADE_BACKGROUND=TEAL_FADE_TEXT<<4,
     RED_FADE_BACKGROUND=RED_FADE_TEXT<<4,       PINK_FADE_BACKGROUND=PINK_FADE_TEXT<<4,
     YELLOW_FADE_BACKGROUND=YELLOW_FADE_TEXT<<4, WHITE_FADE_BACKGROUND=WHITE_FADE_TEXT<<4
 };
//Routine per modificare colore console .
  BOOL resetTextColor(HANDLE h);
  BOOL setTextColor(HANDLE h, WORD c);

  BOOL resetTextColor(void);
  BOOL SetTextColor(WORD c);

#define CONSOLE_H_INCLUDED
#endif //CONSOLE_H_INCLUDED

