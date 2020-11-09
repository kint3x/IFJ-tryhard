#include "stack.h"


void init_stack(stack *s) {

	s->top = NULL;
}

void delete_stack(stack *s) {
	
	item *tmp;
	while (s->top != NULL) {
		tmp = s->top;
		s->top = s->top->next;
		free(tmp);
	}
}

void push(stack *s, expr_lexem l) {
	
	item *i = malloc(sizeof(item));
	if (i == NULL) {
		//error
		return;
	}

	i->lex = l;
	i->next = s->top;
	s->top = i;
}

void pop(stack *s) {
	if (s->top == NULL) {
		return;
	}
	item *tmp;
	tmp = s->top;
	s->top = s->top->next;
	free(tmp);
}