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
	push(L_DOLLAR,0,NULL);
}

void delete_stack() {

	item *tmp;
	while (s.top != NULL) {
		tmp = s.top;
		s.top = s.top->next;
		nstring_free(tmp->nazov);
		free(tmp);
	}
}

void push(expr_lexem l,double data,Nstring *nazov) {

	item *i = malloc(sizeof(item));
	if (i == NULL) {
		//error
		return;
	}
	if(nazov!=NULL){
		if(!nstring_is_clear(nazov)){
			if(l==L_ID){
				i->nazov=nstring_init();
				nstring_cpy(nazov,i->nazov);
			}
		}
	}else{
		i->nazov=NULL;
	}
	i->cislo=data;
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
		nstring_free(tmp->nazov);
		tmp->nazov=NULL;
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
