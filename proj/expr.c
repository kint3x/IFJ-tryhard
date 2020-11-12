/**
 * Projekt: Implementácia analyzy vyrazov pre jazyk IFJ20
 *
 * @brief Implementácia analyzy vyrazov.
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


int reduce_stack(bool *rel_flag) {
	
	expr_lexem op1, op2, op3;

	if (items_above_handle() == 1) {
		switch (s.top->lex) {
		case L_ID:
		case L_INT:
		case L_FLOAT:
		case L_STRING:
			pop(1);
			s.top->handle = false;
			push(L_NON_TERMINAL);
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
			pop(3);
			s.top->handle = false;
			push(L_NON_TERMINAL);
			return ERR_RIGHT;
		}
		
		if (op1 == L_NON_TERMINAL && op3 == L_NON_TERMINAL) {
			switch (op2) {
			case L_PLUS:
			case L_MINUS:
			case L_MUL:
			case L_DIV:
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL);
				return ERR_RIGHT;
				break;
			case L_LESS:
			case L_MORE:
			case L_LESS_EQ:
			case L_MORE_EQ:
			case L_EQUAL:
			case L_NOT_EQUAL:

				if (*rel_flag == true) {
					return ERR_SYNAN;
				}
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL);
				*rel_flag = true;
				return ERR_RIGHT;
				break;
			default:
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


int expression() {

	int ret_value = ERR_SYNAN;

	bool relation_op_flag = false;
	init_stack();
	item *top_term = top_terminal();

	while (token_convert() != L_DOLLAR || top_term->lex != L_DOLLAR) {
		switch (prec_table[top_term->lex][token_convert()]) {
		case '<':
			top_term->handle = true;
			push(token_convert());
			GET_TOKEN();
			break;
		case '>':
			ret_value = reduce_stack(&relation_op_flag);
			VALUE_CHECK();
			break;
		case '=':
			push(token_convert());
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

	delete_stack();
	return ret_value;
}