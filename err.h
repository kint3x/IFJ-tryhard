/**
 * autor: Ľubomír Závodský <xzavod14>		*
 * rok vytvoření: 2020		    	*

*/
#pragma once
#define ERR_RIGHT                           0   // všetko v poriadku.
#define ERR_LEXSCAN                         1   // chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
#define ERR_SYNAN                           2   // chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, neo-čekávané odřádkování).
#define ERR_SEMAN_NOT_DEFINED               3   // sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefi-nici funkce/proměnné, atp
#define ERR_SEMAN_NEW_DEFINED_VARIABLE      4   //  sémantická chyba při odvozování datového typu nově definované proměnné.
#define ERR_SEMAN_TYPE_COMPATIBILITY        5   //sémantická chyba typové kompatibility v aritmetických, řetězcových a relačníchvýrazech
#define ERR_SEMAN_PARAMETERS                6   //  sémantická chyba v programu – špatný počet/typ parametrů či návratových hodnotu volání funkce či návratu z funkce.
#define ERR_SEMAN_OTHERS                    7   // ostatní sémantické chyby
#define ERR_ZERO_DIVIDING                   9   // běhová chyba dělení nulou.
#define ERR_INTERNAL                        99  // interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti, atd.).
