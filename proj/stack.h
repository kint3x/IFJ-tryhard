#ifndef _STACK_H
#define _STACK_H

#include "expr.h"

typedef struct item{
	struct item *next;
	expr_lexem lex;
}item;

typedef struct stack{
	item *top;
} stack;


void init_stack(stack *s);
void delete_stack(stack *s);
void push(stack *s, expr_lexem l);
void pop(stack *s);

#endif