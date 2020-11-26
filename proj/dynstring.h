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

bool Nstring_onlyinit(Nstring *s);
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
* 	@brief Porovná dva dynstringy
*
*	@param s Pointer na štruktúru nafukovacieho stringu
* 	@param d Pointer na štruktúru nafukovacieho stringu
*
*	@return Vráti 0 ak sa rovnajú, <0 ak je zadaný string kratší alebo >0 ak je zadaný string dlhší.
*/
int nstring_cmp(Nstring *s, Nstring *d);
/**
* 	@brief vytiskne obsah seznamu "s" na standardní výstup

*
*/
void nstring_print(Nstring *s);

/**
* 	@brief Vymaže znak z nafukovacého stringu

*
*/
void nstring_char_remove(Nstring *s);


/**
* @brief Vráti či je string prázdny
* @return bool
**/

bool nstring_is_clear(Nstring *s);

/**
* @brief Porovná návratové hodnoty v dynstringoch pre semanticku analyzu
* @return bool
**/
bool nstring_ret_cmp(Nstring *left,Nstring *right);

/**
* @brief Kopíruje dynstring a do dynstringu b
* @return bool
**/
bool nstring_cpy(Nstring *a, Nstring *b);

/**
* @brief Vráti dĺžku dynstringu
* @return dĺžka
**/
int nstring_len(Nstring *s);

/**
* @brief Vráti obsah nstringu v int
* @return int
**/
int nstring_2int(Nstring *s);

/**
* @brief Vráti obsah nstringu vo floate
* @return float
**/
double nstring_3float(Nstring *s);
/**
* @brief Koonvertuje string na escape string
* @return Vráti či sa podarilo
**/
bool nstring_string_to_escape(Nstring *s);


bool nstring_get_and_delete(Nstring *source,Nstring *new);
#endif
