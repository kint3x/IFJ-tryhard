/**
 * Projekt: Implementácia analyzy vyrazov pre jazyk IFJ20
 *
 * @brief Implementácia analyzy vyrazov.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 */
// #TODO -> Najprv si uložiť pri prvom terminále akého typu bude celý výraz, ten potom vraciaš pomocou odkazu na tType
// volanie funkcie bude vyzerať int expr(tType *change,BTreeStackPtr ptr)  na všetky scopy ) použiť funkciu na hladanie
//BTreePtr BTStack_searchbyname(BTreeStackPtr *root,Nstring *s) 
// search= BTStack_searchbyname(&ptr,token.data) -> vráti NULL ak neexistuje, inak vráti odkaz na v strome, search->item_type (T_INT,T_STRING,T_DOUBLE)

//#TODO -> ak spájaš dva výrazy iného typu, vyhodíš chybu 5 ERR_SEMAN_TYPE_COMPATIBILITY
//#TODO -> Delenie nulou ERR_ZERO_DIVIDING    

#include <stdlib.h>
#include "parser.h"
#include "scaner.h"
#include "expr.h"
#include "stack.h"
#include "err.h"
#include "dynstring.h"

extern Token token;
extern Token tokenp;
stack s;

char prec_table[17][17] = {
	//+   -   *   /   (   )   i   <   >  <=  >=   ==  !=  $  int str float
	{'>','>','<','<','<','>','<','>','>','>','>','>','>','>','<','<','<'}, //+
	{'>','>','<','<','<','>','<','>','>','>','>','>','>','>','<','<','<'}, //-
	{'>','>','>','>','<','>','<','>','>','>','>','>','>','>','<','<','<'}, //*
	{'>','>','>','>','<','>','<','>','>','>','>','>','>','>','<','<','<'}, ///
	{'<','<','<','<','<','=','<','<','<','<','<','<','<',' ','<','<','<'}, //(
	{'>','>','>','>',' ','>',' ','>','>','>','>','>','>','>',' ',' ',' '}, //)
	{'>','>','>','>',' ','>',' ','>','>','>','>','>','>','>',' ',' ',' '}, //i
	{'<','<','<','<','<','>','<',' ',' ',' ',' ',' ',' ','>','<','<','<'}, //<
	{'<','<','<','<','<','>','<',' ',' ',' ',' ',' ',' ','>','<','<','<'}, //>
	{'<','<','<','<','<','>','<',' ',' ',' ',' ',' ',' ','>','<','<','<'}, //<=
	{'<','<','<','<','<','>','<',' ',' ',' ',' ',' ',' ','>','<','<','<'}, //>=
	{'<','<','<','<','<','>','<',' ',' ',' ',' ',' ',' ','>','<','<','<'}, //==
	{'<','<','<','<','<','>','<',' ',' ',' ',' ',' ',' ','>','<','<','<'}, //!=
	{'<','<','<','<','<',' ','<','<','<','<','<','<','<',' ','<','<','<'}, //$
	{'>','>','>','>',' ','>',' ','>','>','>','>','>','>','>',' ',' ',' '}, //int
	{'>','>','>','>',' ','>',' ','>','>','>','>','>','>','>',' ',' ',' '}, //str
	{'>','>','>','>',' ','>',' ','>','>','>','>','>','>','>',' ',' ',' '}  //float
};

expr_lexem token_convert() {
	
	expr_lexem l;

	switch (token.type) {
	case T_ID:
		l = L_ID;
		break;
	case T_INT:
		l = L_INT;
		break;
	case T_DOUBLE:
		l = L_FLOAT;
		break;
	case T_STRING:
		l = L_STRING;
		break;
	case T_PLUS:
		l = L_PLUS;
		break;
	case T_MINUS:
		l = L_MINUS;
		break;
	case T_MUL:
		l = L_MUL;
		break;
	case T_DIV:
		l = L_DIV;
		break;
	case T_LEFTBR:
		l = L_LEFT_BR;
		break;
	case T_RIGHTBR:
		l = L_RIGHT_BR;
		break;
	case T_LESS:
		l = L_LESS;
		break;
	case T_MORE:
		l = L_MORE;
		break;
	case T_LESS_EQ:
		l = L_LESS_EQ;
		break;
	case T_MORE_EQ:
		l = L_MORE_EQ;
		break;
	case T_EQ_COMP:               
		l = L_EQUAL;
		break;
	case T_NOTEQUAL:               
		l = L_NOT_EQUAL;
		break;
	default:
		l = L_DOLLAR;
	}
	return l;
}

int items_above_handle() {
	
	int counter = 0;
	item *tmp = s.top;

	while (tmp->handle != true) {
		counter++;
		tmp = tmp->next;
	}
	return counter;
}


int reduce_stack(bool *rel_flag,tType *change) {
	Nstring *pomocna=nstring_init();
	double pomoc;
	expr_lexem op1, op2, op3;

	if (items_above_handle() == 1) {
		switch (s.top->lex) {
		case L_ID:
		case L_STRING:
			nstring_cpy(s.top->nazov,pomocna);
			pomoc=s.top->cislo;
			pop(1);
			s.top->handle = false;
			push(L_NON_TERMINAL,pomoc,pomocna);
			nstring_free(pomocna);
			return ERR_RIGHT;
			break;
		case L_INT:
		case L_FLOAT:
			pomoc=s.top->cislo;
			pop(1);
			s.top->handle = false;
			push(L_NON_TERMINAL,pomoc,NULL);
			nstring_free(pomocna);
			return ERR_RIGHT;
			break;
		default:
			return ERR_SYNAN;
			break;
		}
	}
	else if (items_above_handle() == 3) {
		op3 = s.top->lex;
		op2 = s.top->next->lex;
		op1 = s.top->next->next->lex;

		if (op1 == L_LEFT_BR && op2 == L_NON_TERMINAL && op3 == L_RIGHT_BR) {
			nstring_cpy(s.top->nazov,pomocna);
			pomoc=s.top->cislo;
			pop(3);
			s.top->handle = false;
			push(L_NON_TERMINAL,pomoc,pomocna);
			nstring_free(pomocna);
			return ERR_RIGHT;
		}
		
		if (op1 == L_NON_TERMINAL && op3 == L_NON_TERMINAL) {
			switch (op2) {
			case L_PLUS:
				if(((*change)==T_INT)||((*change)==T_DOUBLE)){
					if(nstring_is_clear(s.top->next->next->nazov)&&nstring_is_clear(s.top->nazov)){
						pomoc=(s.top->next->next->cislo)-(s.top->cislo);
					}
				}
				nstring_cpy(s.top->nazov,pomocna);
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,pomoc,pomocna);
				nstring_free(pomocna);
				return ERR_RIGHT;
				break;
			case L_MINUS:
				if(((*change)==T_INT)||((*change)==T_DOUBLE)){
					if(nstring_is_clear(s.top->next->next->nazov)&&nstring_is_clear(s.top->nazov)){
						pomoc=(s.top->next->next->cislo) - (s.top->cislo);
					}
				}
				nstring_cpy(s.top->nazov,pomocna);
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,pomoc,pomocna);
				nstring_free(pomocna);
				return ERR_RIGHT;
				break;
			case L_MUL:
				if(((*change)==T_INT)||((*change)==T_DOUBLE)){
					if(nstring_is_clear(s.top->next->next->nazov)&&nstring_is_clear(s.top->nazov)){
						pomoc=(s.top->next->next->cislo)*(s.top->cislo);
					}
				}
				nstring_cpy(s.top->nazov,pomocna);
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,pomoc,pomocna);
				nstring_free(pomocna);
				return ERR_RIGHT;
				break;
			case L_DIV:
				if(((*change)==T_INT)||((*change)==T_DOUBLE)){
					if(nstring_is_clear(s.top->next->next->nazov)&&nstring_is_clear(s.top->nazov)){
						if(s.top->cislo == 0) return ERR_ZERO_DIVIDING;
						pomoc=(s.top->next->next->cislo)/(s.top->cislo);
					}
				}
				nstring_cpy(s.top->nazov,pomocna);
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,pomoc,pomocna);
				nstring_free(pomocna);
				return ERR_RIGHT;
				break;
			case L_LESS:
			case L_MORE:
			case L_LESS_EQ:
			case L_MORE_EQ:
			case L_EQUAL:
			case L_NOT_EQUAL:

				if (*rel_flag == true) {
					nstring_free(pomocna);
					return ERR_SYNAN;
				}
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,0,NULL);
				*rel_flag = true;
				nstring_free(pomocna);
				return ERR_RIGHT;
				break;
			default:
				nstring_free(pomocna);
				return ERR_SYNAN;
				break;
			}
		}
		return ERR_SYNAN;
	}
	else {
		return ERR_SYNAN;
	}
}


int expression(tType *change,bool *cond,BTreeStackPtr Local_trees) {

	int ret_value = ERR_SYNAN;
	(*change)=T_UNKNOWN;
	bool relation_op_flag = false;
	init_stack();
	item *top_term = top_terminal();

	while (token_convert() != L_DOLLAR || top_term->lex != L_DOLLAR) {
		switch (prec_table[top_term->lex][token_convert()]) {
		case '<':
			top_term->handle = true;
			if((ret_value=sem_check_var(change,Local_trees))!=ERR_RIGHT){
				delete_stack();
				return ret_value;
			}

			push(token_convert(),nstring_3float(token.data),token.data);
			GET_TOKEN();
			break;
		case '>':
			ret_value = reduce_stack(&relation_op_flag,change);
			if(ret_value!=ERR_RIGHT){
				delete_stack();
				return ret_value;
			}
			break;
		case '=':
			if((ret_value=sem_check_var(change,Local_trees))!=ERR_RIGHT){
				delete_stack();
				return ret_value;
			}
			push(token_convert(),nstring_3float(token.data),token.data);
			GET_TOKEN();
			break;
		case ' ':
			return ERR_SYNAN;
			break;
		}
		top_term = top_terminal();
		
	}

	if (s.top->lex == L_NON_TERMINAL && s.top->next->lex == L_DOLLAR) {
		ret_value = ERR_RIGHT;
	}

	(*cond) = relation_op_flag;
	delete_stack();
	return ret_value;
}

int sem_check_var(tType *change,BTreeStackPtr Local_trees){

	if(token.type==T_ID){
		BTreePtr search = BTStack_searchbyname(&Local_trees,token.data); // vyhlada premennu
		if(search == NULL) return ERR_SEMAN_NOT_DEFINED; // ak nenajde neexistuje
		
		if((*change)==T_UNKNOWN){
			(*change)=search->item_type;
		}else{
			if(search->item_type!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	else if(token.type==T_INT){
		if((*change)==T_UNKNOWN){
			(*change)=T_INT;
		}else{
			if(T_INT!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	else if(token.type==T_STRING){
		if((*change)==T_UNKNOWN){
			(*change)=T_STRING;
		}else{
			if(T_STRING!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	else if(token.type==T_DOUBLE){
		if((*change)==T_UNKNOWN){
			(*change)=T_DOUBLE;
		}else{
			if(T_DOUBLE!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	return ERR_RIGHT;
}