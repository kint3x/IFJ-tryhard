#ifndef _EXPR_H
#define _EXPR_H



typedef enum {
	PLUS,			// +
	MINUS,			// -
	MUL,			// *
	DIV,			// /
	LEFT_BR,		// (
	RIGHT_BR,		// )
	ID,				// i
	LESS,			// <
	MORE,			// >
	LESS_EQ,		// <=
	MORE_EQ,		// >=
	EQUAL,			// ==
	NOT_EQUAL,		// !=
	DOLLAR,			// $
	INT,			// int
	STRING,			// str
	FLOAT,			// float
	NON_TERMINAL,	// E
	ERR				// pre T_ERR

} expr_lexem;

#endif