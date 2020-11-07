#ifndef _STACK_H
#define _STACK_H

#include "expr.h"

typedef struct {
	item *next;
	expr_lexem lex;
} item;

typedef struct {
	item *top;
} stack;

#endif

void init_stack(stack *s);
void delete_stack(stack *s);
void push(stack *s, expr_lexem l)
void pop(stack *s);

