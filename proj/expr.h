#ifndef _EXPR_H
#define _EXPR_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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
	L_ERR			// pre T_ERR alebo T_UNKNOWN

} expr_lexem;

expr_lexem token_convert();
int items_above_handle();
int reduce_stack(bool *rel_flag);
int expression();

#endif
