/*
* autor:  Lubomír Závodský <xzavod14>	                *
* rok vytvoření: 2020		                            	*
* projekt ifj 2020/2021                             *
*/
//hlavicka pro lexikalni analyzator
#pragma once
#include <stdio.h>
#include "err.h"

#include "dynstring.h"
//jednotlive typy
typedef enum
{
  T_ERR, // token pre parser  o tom že nastala chyba v lexikalke
  T_UNKNOWN, //Výchozí typ
  T_ID,      //identifikator
  T_INT,  // čislo typu int
  T_DOUBLE, // číslo typu double
  T_COMENT, // //
  //Keyword
  T_WELSE,    // Keyword - ELSE
  T_WFLOAT64,      // Keyword - FLOAT64
  T_WFOR,    // Keyword - FOR
  T_WFUNC,    // Keyword FUNC
  T_WIF,  //Keyword IF
  T_WINT,   //Keyword - INT
  T_WPACKAGE,   //Keyword -PACKAGE
  T_WRETURN,   //Keyword -RETURN
  T_WSTRING,   //Keyword -STRING
  //operatory
  T_PLUS,   //+
  T_MINUS,  //-
  T_MUL,    // *
  T_DIV,    // /
  T_STRING, // retazec

  // logic operators
  T_NOT,  //logic not
  T_AND,  // logic and
  T_OR, // logic or
  T_EQ_COMP, // ==
  // others char
  T_SEMI, //;
  T_LEFTBRACET, //{
  T_RIGHTBRACET, // }
  T_LEFTBR,   // (
  T_RIGHTBR,  // )
  T_COMMA,    //.
  T_ASSIGN,   // =
  T_NOTEQUAL, // !=
  T_SPACE,    // medzera
  T_DOUBLEDOT, //:
  //porovnavanie
  T_LESS, //<
  T_LESS_EQ, // <=
  T_MORE, //>
  T_MORE_EQ,// >=

  //
  T_EOL, //koniec riadku
  T_END_OF_FILE // koniec vstupneho suboru
} tType;
//stavy automatu
typedef enum
{
  EOLINE,   // END OF line
  START,    // počiatočny stav ak nebolo odsadenie
  ID,       //identifikator
  KEYW,     // klučove slovo
  TINT,      // číslo
  DOUBLE,   // desatine čislo
  STRING,   // stav kontrolujuci retazec
  HEX,      //escape sekvencia
  HEX_CON,  //escape con
  ESC,      //escape sekvencia
  MINUS,    //-
  PLUS,     // +
  MULTIPLE, // *
  TESTDIVIDE,   // / delenie kontroluje či neni komentar
  SPACE,    //space
  NOT,// !
  AND,
  OR,
  LEFTBR,    //(
  RIGHTBR,   // )
  COMMA,     //,
  ASSIGN,    //=
  LESS,      //<
  MORE,      //>
  DOUBLEDOT, //:
  BLOCKCOMENT, // /* komentar blokovy
  COMENT,   // komentar riadkovy
  LEFTBRACET, // {
  RIGHTBRACET, //}
  SEMI, // ;
  END        // koncový stav
  } T_state;
typedef

    Nstring *
        tData;

// definice structury tokenu
typedef struct Token
{
  tType type;
 tData data;

} Token;

Token *T_init();

FILE *source;                // promenna pro ulozeni vstupniho souboru
//funkce scaneru
void setSourceFile(FILE *f); // funkce pre ulo6enie suboru na nacitanie
int main();
Token *getNextToken();  // hlavna funkcia
void print_token(Token *token); //testovanie
