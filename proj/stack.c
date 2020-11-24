/**
 * Projekt: Implementácia stacku pre jazyk IFJ20
 *
 * @brief Implementácia stacku.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 */

#include <stdlib.h>
#include "parser.h"
#include "scaner.h"
#include "expr.h"
#include "stack.h"
#include "err.h"
#include "dynstring.h"

extern stack s;

void init_stack() {

	s.top = NULL;
	push(L_DOLLAR);
}

void delete_stack() {

	item *tmp;
	while (s.top != NULL) {
		tmp = s.top;
		s.top = s.top->next;
		free(tmp);
	}
}

void push(expr_lexem l) {

	item *i = malloc(sizeof(item));
	if (i == NULL) {
		//error
		return;
	}

	i->lex = l;
	i->handle = false;
	i->next = s.top;
	s.top = i;
}

void pop(int count) {
	for (int i = 0; i < count; i++) {
		item *tmp;
		if (s.top == NULL) {
			return;
		}
		tmp = s.top;
		s.top = s.top->next;
		free(tmp);
	}
}

item *top_terminal() {
	
	item *tmp = s.top;
	
	while (tmp->lex == L_NON_TERMINAL) {
		tmp = tmp->next;
	}

	return tmp;
}
