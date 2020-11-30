/**
 * Projekt: Implementace překladače imperativního jazyka IFJ20.
 *
 * @brief Implementácia stacku.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 */

#ifndef _STACK_H
#define _STACK_H

#include "expr.h"
#include <stdbool.h>

typedef struct item {
	struct item *next;
	expr_lexem lex;
	bool handle;
	//pridane
	expr_lexem type;
	double val;

}item;

typedef struct stack {
	item *top;
} stack;


void init_stack();
void delete_stack();
void push(expr_lexem l,expr_lexem past_type,double val);
void pop(int count);
item *top_terminal();

#endif
