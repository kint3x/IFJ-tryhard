/*
* autor:  Lubomír Závodský <xzavod14>	                *
* rok vytvoření: 2020		                            	*
* projekt ifj 2020/2021                             *
*/
//hlavicka pro lexikalni analyzator

#include "err.h"

#include "dynstring.h"
//jednotlive typy
typedef enum
{

  T_UNKNOWN, //Výchozí typ
  T_ID,      //identifikator
             //klicova slova
  T_ELSE,    // Keyword - ELSE
  T_INT,
  T_DOUBLE,
  T_COMENT, // //
  //operatory
  T_PLUS,   //+
  T_MINUS,  //-
  T_MUL,    // *
  T_DIV,    // /
  T_INTDIV, // //
  T_STRING, // retazec

  // logic operators
  T_NOT,
  T_AND,
  T_OR,
  T_EQ_COMP, //
  // others char
  T_LEFTBR,   // (
  T_RIGHTBR,  // )
  T_COMMA,    //.
  T_ASSIGN,   // =
  T_NOTEQUAL, // !=
  T_SPACE,    // medzera
  T_DOUBLEDOT, //:
  //porovnavanie
  T_LESS, //<
  T_LESS_EQ,
  T_MORE, //>
  T_MORE_EQ,
  //odsadenie
  T_INDENT,
  T_DEDENT,
  //
  T_EOL, //koniec riadku
  T_END_OF_FILE // koniec vstupneho suboru

} tType;
//stavy automatu
typedef enum
{
  BEGLINE,  //stav začiatok riadku kontrola odsadenia
  EOLINE,   // END OF line
  START,    // počiatočny stav ak nebolo odsadenie
  ID,       //identifikator
  KEYW,     // klučove slovo
  TINT,      // číslo
  DOUBLE,   // desatine čislo
  STRING,   // stav kontrolujuci retazec
  HEX,      //
  HEX_CON,  //
  ESC,      //escape sekvencia
  MINUS,    //operatory
  PLUS,     // +
  MULTIPLE, // -
  DIVIDE,   // delenie
  SPACE,    //space
  NOT,
  AND,
  OR,
  LEFTBR,    //(
  RIGHTBR,   // )
  COMMA,     //,
  ASSIGN,    //=
  LESS,      //<
  MORE,      //>
  DOUBLEDOT, //:
  END,        // koncový stav
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
