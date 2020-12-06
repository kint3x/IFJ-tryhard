/**
 * Projekt: Implementace překladače imperativního jazyka IFJ20.
 *
 * @brief Implementácia analyzy vyrazov.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 */
#ifndef _EXPR_H
#define _EXPR_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "symtable.h"

typedef enum {
	L_PLUS,			// +
	L_MINUS,		// -
	L_MUL,			// *
	L_DIV,			// /
	L_LEFT_BR,		// (
	L_RIGHT_BR,		// )
	L_ID,			// i
	L_LESS,			// <
	L_MORE,			// >
	L_LESS_EQ,		// <=
	L_MORE_EQ,		// >=
	L_EQUAL,		// ==
	L_NOT_EQUAL,	// != 
	L_DOLLAR,		// $
	L_INT,			// int
	L_STRING,		// str
	L_FLOAT,		// float
	L_NON_TERMINAL,	// E
	L_ERR,			// pre T_ERR alebo T_UNKNOWN
	L_BOOL			// pre bool

} expr_lexem;

/*
*	@brief Konvertuje token zo scanneru na token pre analyzu
*
*	@return Novy typ tokenu
*/
expr_lexem token_convert();
/*
*	@brief Spočíta itemy nad handle
*
*	@return Vráti počet itemov nad handle
*/
int items_above_handle();

/*
*	@brief Redukuje stack 
*
*	@return Vráti ERR_RIGHT či nejakú z syntaktických alebo semantických chýb
*/
int reduce_stack(bool *rel_flag,tType *change);

/*
*	@brief Hlavná funkcia precedenčnej analýzy
*
*	@return Vráti ERR_RIGHT či nejakú z syntaktických alebo semantických chýb
*/
int expression(tType *change,bool *cond,BTreeStackPtr Local_trees);

/*
*	@brief Kontroluje, či premenné sú správneho typu, semantická kontrola
*
*	@return Vráti ERR_RIGHT či nejakú zo semantických chýb
*/
int sem_check_var(tType *change,BTreeStackPtr Local_trees);
#endif
