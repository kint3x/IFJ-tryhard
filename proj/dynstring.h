/**
 * Projekt: Implementácia prekladača pre jazyk IFJ20
 *
 * @brief Implementácia nafukovacieho stringu.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */

#ifndef _DYNSTRING_H
#define _DYNSTRING_H

#include <stdbool.h>

/**
	@struct Dátový typ pre nafukovací string
*/

typedef struct{
	char *string;
	unsigned int string_size;
	unsigned int allocated_size;
}Nstring;

/**
*	@brief Inicializácia nafukovacieho stringu
*
*	@param s Pointer na štruktúru nafukovacieho stringu
*
*	@result Ak sa nepodarilo, vypíše chybu na stderr
*
*	@return Vráti pointer na štruktúru Nstring alebo NULL ak sa nepodarilo allokovať
*/
Nstring *nstring_init();

/**
*	@brief Uvoľnenie nafukovacieho stringu
*
*	@param s Pointer na štruktúru nafukovacieho stringu
*
*/
void nstring_free(Nstring *s);

/**
*	@brief Zmaže aktuálne uložené znaky zo stringu
*
*	@param s Pointer na štruktúru nafukovacieho stringu
*
*/
void nstring_clear(Nstring *s);

/**
*	@brief Pridá znak na koniec reťazca
*
*	@param s Pointer na štruktúru nafukovacieho stringu
* 	@param c Znak ktorý sa má uložiť
*   @return Vráti true ak sa podarilo, false ak nie
*/
bool nstring_add_char(Nstring *s, char c);


/**
*	@brief Zapíše string do štruktúry
*
*	@param s Pointer na štruktúru nafukovacieho stringu
* 	@param str string ktorý sa zapíše do štruktúry
*	@return Vráti true ak sa podarilo, false ak nie
*/

//#TODO --> Tuto funkciu implementujem ak ju bude treba
bool nstring_add_str(Nstring *s, char *str);

/**
* 	@brief Porovná zadaný string s dynstringom
*
*	@param s Pointer na štruktúru nafukovacieho stringu
* 	@param str string ktorý sa zapíše do štruktúry
*
*	@return Vráti 0 ak sa rovnajú, <0 ak je zadaný string kratší alebo >0 ak je zadaný string dlhší.
*/

int nstring_str_cmp(Nstring *s, const char *str);
/**
* 	@brief vytiskne obsah seznamu "s" na standardní výstup

*
*/
void nstring_print(Nstring *s);

#endif
