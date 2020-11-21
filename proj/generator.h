/*
* autor:  Lubomír Závodský <xzavod14>	                *
* rok vytvoření: 2020		                            	*
* projekt ifj 2020/2021                             *
*/
//hlavicka pro generator


//funkce generatoru
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "stable.h"
/**
 * funkce vygenerjue kod (IFJCODE20) z aktualne načteneho tokenu
 * funkce je volana  v syntaktickej analýze po každom vyhodnotení tokenu
 * @param aktualne načtení token
 * @param table Lokální tabulka proměnných aktuální funkce
*/

void generatecodefromtoken(Token *token ,  tree table)
static int parametrs =  1; //počet parametrov

{
switch (token->type)
  {

  }


}
