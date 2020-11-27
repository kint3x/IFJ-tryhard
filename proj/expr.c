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
#include "code_generator.h"

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
	
	double d_pomocny;
	expr_lexem t_pomocny;

	expr_lexem op1, op2, op3;

	if (items_above_handle() == 1) {
		switch (s.top->lex) {
		case L_ID:
		case L_INT:
		case L_FLOAT:
		case L_STRING:
			d_pomocny=s.top->val;
			t_pomocny=s.top->type;
			pop(1);
			s.top->handle = false;
			push(L_NON_TERMINAL,t_pomocny,d_pomocny);
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
			d_pomocny=s.top->next->val;
			t_pomocny=s.top->next->type;

			pop(3);
			s.top->handle = false;
			push(L_NON_TERMINAL,t_pomocny,d_pomocny);
			return ERR_RIGHT;
		}
		
		if (op1 == L_NON_TERMINAL && op3 == L_NON_TERMINAL) {
			switch (op2) {
			case L_PLUS:
				if((s.top->next->next->type==L_BOOL) || (s.top->type==L_BOOL)) return ERR_SEMAN_TYPE_COMPATIBILITY; //nemožno redukovat bool
				if((s.top->next->next->type==L_ID) || (s.top->type==L_ID)){ // ak je jedno z nich ID, vysledok bude E typu T_ID
					t_pomocny=L_ID;
					d_pomocny=42; //magicka konštanta aby tam nebola 0
				}
				else{ // ak ani jedno nie je ID tak výpočet uložíme a vysledok bude rovnakeho typu
					t_pomocny=s.top->type;
					d_pomocny = s.top->next->next->val + s.top->val;
				}
				if((*change)==T_STRING){
					G_expr_string_concat();
				}else{
					G_expr_operat('+',(*change));
				}
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,t_pomocny,d_pomocny);
				return ERR_RIGHT;
				break;
			case L_MINUS:
				if((s.top->next->next->type==L_BOOL) || (s.top->type==L_BOOL)) return ERR_SEMAN_TYPE_COMPATIBILITY; //nemožno redukovat bool
				if((s.top->next->next->type==L_ID) || (s.top->type==L_ID)){ // ak je jedno z nich ID, vysledok bude E typu T_ID
					t_pomocny=L_ID;
					d_pomocny=42; //magicka konštanta aby tam nebola 0
				}
				else{ // ak ani jedno nie je ID tak výpočet uložíme a vysledok bude rovnakeho typu
					t_pomocny=s.top->type;
					d_pomocny = s.top->next->next->val - s.top->val;
				}
				G_expr_operat('-',(*change));
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,t_pomocny,d_pomocny);
				return ERR_RIGHT;
				break;
			case L_MUL:
				if((s.top->next->next->type==L_BOOL) || (s.top->type==L_BOOL)) return ERR_SEMAN_TYPE_COMPATIBILITY; //nemožno redukovat bool
				if((s.top->next->next->type==L_ID) || (s.top->type==L_ID)){ // ak je jedno z nich ID, vysledok bude E typu T_ID
					t_pomocny=L_ID;
					d_pomocny=42; //magicka konštanta aby tam nebola 0
				}
				else{ // ak ani jedno nie je ID tak výpočet uložíme a vysledok bude rovnakeho typu
					t_pomocny=s.top->type;
					d_pomocny = s.top->next->next->val * s.top->val;
				}
				G_expr_operat('*',(*change));
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,t_pomocny,d_pomocny);
				return ERR_RIGHT;
				break;
			case L_DIV:
				if((s.top->next->next->type==L_BOOL) || (s.top->type==L_BOOL)) return ERR_SEMAN_TYPE_COMPATIBILITY; //nemožno redukovat bool
				if((s.top->next->next->type==L_ID) || (s.top->type==L_ID)){ // ak je jedno z nich ID, vysledok bude E typu T_ID
					if(s.top->type != L_ID) {
						if(s.top->val == 0) return ERR_ZERO_DIVIDING; // delime nulou zleje , vyhadzuje error
					}
					t_pomocny=L_ID;
					d_pomocny=42; //magicka konštanta aby tam nebola 0
				}
				else{ // ak ani jedno nie je ID tak výpočet uložíme a vysledok bude rovnakeho typu
					if(s.top->val == 0) return ERR_ZERO_DIVIDING; // delime nulou zleje , vyhadzuje error
					t_pomocny=s.top->type;
					d_pomocny = s.top->next->next->val / s.top->val;
				}
				G_expr_operat('/',(*change));
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,t_pomocny,d_pomocny);
				return ERR_RIGHT;
				break;
			case L_LESS://<
				if(op2==L_LESS)G_if_operat('<',*change);
			case L_MORE://>
				if(op2==L_MORE)G_if_operat('>',*change);
			case L_LESS_EQ://M
				if(op2==L_LESS_EQ)G_if_operat('M',*change);
			case L_MORE_EQ://V
				if(op2==L_MORE_EQ)G_if_operat('V',*change);
			case L_EQUAL://=
				if(op2==L_EQUAL)G_if_operat('=',*change);
			case L_NOT_EQUAL://!
				if(op2==L_NOT_EQUAL)G_if_operat('!',*change);

				if (*rel_flag == true) {
					return ERR_SEMAN_TYPE_COMPATIBILITY;
				}
				pop(3);
				s.top->handle = false;
				push(L_NON_TERMINAL,L_BOOL,42);
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
			push(token_convert(),token_convert(),nstring_3float(token.data));
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
			push(token_convert(),token_convert(),nstring_3float(token.data));
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
		if(strcmp(token.data->string,"_")==0) return ERR_SEMAN_OTHERS;
		BTreePtr search = BTStack_searchbyname(&Local_trees,token.data); // vyhlada premennu
		if(search == NULL) return ERR_SEMAN_NOT_DEFINED; // ak nenajde neexistuje
		G_expr_term(token,search->uniq_scope);
		
		if((*change)==T_UNKNOWN){
			(*change)=search->item_type;
		}else{
			if(search->item_type!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	else if(token.type==T_INT){
		G_expr_term(token,0);
		if((*change)==T_UNKNOWN){
			(*change)=T_INT;
		}else{
			if(T_INT!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	else if(token.type==T_STRING){
		G_expr_term(token,0);
		if((*change)==T_UNKNOWN){
			(*change)=T_STRING;
		}else{
			if(T_STRING!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	else if(token.type==T_DOUBLE){
		G_expr_term(token,0);
		if((*change)==T_UNKNOWN){
			(*change)=T_DOUBLE;
		}else{
			if(T_DOUBLE!=(*change)){
				return ERR_SEMAN_TYPE_COMPATIBILITY;
			}
		}
	}
	if((*change)==T_STRING){ // ak pracujeme so stringmi nemôžeme povoliť operácie * / - 
		if(token.type==T_MUL) return ERR_SEMAN_TYPE_COMPATIBILITY;
		if(token.type==T_DIV) return ERR_SEMAN_TYPE_COMPATIBILITY;
		if(token.type==T_MINUS) return ERR_SEMAN_TYPE_COMPATIBILITY;
	}
	return ERR_RIGHT;
}
