/**
 * Projekt: Implementácia prekladača pre jazyk IFJ20
 *
 * @brief Implementácia err stacku pre volané funkcie pred prečítaním ich hlavičky.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */


#ifndef _ERR_STACK
#define _ERR_STACK

#include "dynstring.h"
#include "symtable.h"
#include "err.h"
#include <stdlib.h>



typedef struct estack{

Nstring *called_name;
Nstring *left;
Nstring *called_arg_types;

struct estack *next;
}*EStackPtr;


/*
*	@brief Pridá volanie na stack problémov a odloží ho k neskoršiemu vyriešeniu
*	
*	@return ERR_INTERNAL | ERR_RIGHT
*/
int EStack_addcall(EStackPtr *root,Nstring *called_name, Nstring *left, Nstring *arg_types);

/*
*	@brief Pridá volanie na stack problémov a odloží ho k neskoršiemu vyriešeniu
*	
*	@return ERR_RIGHT | ERR_SEMAN_PARAMETERS 
*/
int EStack_solveproblems(EStackPtr *root,BTreePtr global_tree);

/*
*	@brief Pridá volanie na stack problémov a odloží ho k neskoršiemu vyriešeniu
*	
*	@Vymaže zoznam a uvoľní pamäť	
*/
void EStack_dispose(EStackPtr *root);



#endif