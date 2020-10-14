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
*	@return Vráti true ak sa podarilo, inak false
*/
bool nstring_init(Nstring *s);

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
*/
void nstring_add_char(Nstring *s, char c);



#endif