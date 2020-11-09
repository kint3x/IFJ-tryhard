#ifndef _EXPR_H
#define _EXPR_H



typedef enum {
	PLUS,			// +
	MINUS,			// -
	MUL,			// *
	DIV,			// /
	LEFT_BR,		// (
	RIGHT_BR,		// )
	LESS,			// <
	MORE,			// >
	LESS_EQ,		// <=
	MORE_EQ,		// >=
	EQUAL,			// ==
	NOT_EQUAL,		// !=
	ID,				// i
	INT,			// int
	STRING,			// str
	FLOAT,			// float
	NON_TERMINAL,	// E
	DOLLAR,			// $
	ERR				// pre T_ERR

} expr_lexem;