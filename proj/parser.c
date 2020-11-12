
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "scaner.h"
#include "expr.h"
#include "stack.h"
#include "err.h"
#include "dynstring.h"

Token token; // GLOBALNA PREMENNA S AKTUALNYM TOKENOM
Token tokenp; // pomocny token

void peek_nexttoken() {
	Token *t = getNextToken();   // NACITAME NOVY TOKEN
	print_token(t);
	tokenp.type = t->type;
	tokenp.data = t->data;
	free(t);					// UVOLNIME STRUKTURU 
	t = NULL;
}

void p_getnexttoken() {
	if (token.type == T_END_OF_FILE) {  //ROBILO TO MEMORY LEAK AK TU TATO PODMIENKA NEBOLA, Z NEJAKOHO DOVODU
		nstring_free(token.data);       // SI TO PYTALO DALSI TOKEN PO EOF
		return;
	}

	if (tokenp.type != -1) {
		nstring_free(token.data);
		token.type = tokenp.type;
		token.data = tokenp.data;
		tokenp.type = -1;
		tokenp.data = NULL;
	}
	else {
		nstring_free(token.data);  // VYMAZE SA POVODY NSTRING
		token.data = NULL;

		Token *t = getNextToken();   // NACITAME NOVY TOKEN
		print_token(t);
		token.type = t->type;
		token.data = t->data;
		free(t);					// UVOLNIME STRUKTURU 
		t = NULL;
	}
}


int p_prog() {
	//printf("SET token.data to null\n");
	token.type = T_UNKNOWN; // Inicializacia 
	token.data = NULL; // Nastavi dynstring na NULL
	tokenp.data = NULL;
	tokenp.type = -1;

	int ret_value = ERR_SYNAN;
	GET_TOKEN();

	switch (token.type) {
	case T_WPACKAGE:
	case T_EOL:
		
		ret_value = p_opteol();
		VALUE_CHECK();

		if (token.type == T_WPACKAGE) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}
		if (token.type == T_ID) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else if(token.type== T_END_OF_FILE){
			ret_value = ERR_SEMAN_NOT_DEFINED;
			VALUE_CHECK();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		ret_value = p_opteol();
		VALUE_CHECK();
		ret_value = p_funclist();
		VALUE_CHECK();

		break;
	default:
		break;
	}
	return ret_value;
}

int p_funclist() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_WFUNC:
		ret_value = p_func();
		VALUE_CHECK();
		ret_value = p_opteol();
		VALUE_CHECK();
		ret_value = p_funclist();
		VALUE_CHECK();
		break;

	case T_END_OF_FILE:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_func() {

	int ret_value = ERR_SYNAN;

	if (token.type == T_WFUNC) {
		GET_TOKEN();

		if (token.type == T_ID) {
			GET_TOKEN();

			if (token.type == T_LEFTBR) {
				GET_TOKEN();

				ret_value = p_paramlist();
				VALUE_CHECK();
				ret_value = p_datatypelist();
				VALUE_CHECK();
				ret_value = p_statlist();
				VALUE_CHECK();

				if (token.type == T_EOL) {
					GET_TOKEN();
				}
				else if (token.type == T_END_OF_FILE) {
					ret_value = ERR_RIGHT;
				}
				else {
					ret_value = ERR_SYNAN;
					VALUE_CHECK();
				}
			}
		}
	}

	return ret_value;
}

int p_paramlist() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_ID:
		GET_TOKEN();

		ret_value = p_datatype();
		VALUE_CHECK();
		ret_value = p_paramnext();
		VALUE_CHECK();
		break;

	case T_RIGHTBR:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_paramnext() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_COMMA:
		GET_TOKEN();

		if (token.type == T_ID) {
			GET_TOKEN();
		}
		else break;

		ret_value = p_datatype();
		VALUE_CHECK();
		ret_value = p_paramnext();
		VALUE_CHECK();
		break;

	case T_RIGHTBR:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_datatypelist() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_LEFTBR:
		GET_TOKEN();

		ret_value = p_datatype();
		VALUE_CHECK();
		ret_value = p_datatypenext();
		VALUE_CHECK();
		break;

	case T_LEFTBRACET:
		GET_TOKEN();

		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else break;

		ret_value = p_opteol();
		VALUE_CHECK();
		break;
	default:
		break;
	}

	return ret_value;
}

int p_datatypenext() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_COMMA:
		GET_TOKEN();

		ret_value = p_datatype();
		VALUE_CHECK();
		ret_value = p_datatypenext();
		VALUE_CHECK();
		break;

	case T_RIGHTBR:
		GET_TOKEN();

		if (token.type == T_LEFTBRACET) {
			GET_TOKEN();
		}
		else break;

		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else break;

		ret_value = p_opteol();
		VALUE_CHECK();
		break;
	default:
		break;
	}

	return ret_value;
}

int p_datatype() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_WINT:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_WFLOAT64:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_WSTRING:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_statlist() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_WIF:
	case T_WFOR:
	case T_WRETURN:
	case T_ID:
		ret_value = p_stat();
		VALUE_CHECK();
		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		ret_value = p_opteol();
		VALUE_CHECK();
		ret_value = p_statlist();
		VALUE_CHECK();
		break;

	case T_RIGHTBRACET:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_stat() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_WIF:
		GET_TOKEN();
		
		ret_value = expression();
		VALUE_CHECK();

		if (token.type == T_LEFTBRACET) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		ret_value = p_opteol();
		VALUE_CHECK();
		ret_value = p_statlist();
		VALUE_CHECK();

		if (token.type == T_WELSE) {
			GET_TOKEN();
		} 
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		if (token.type == T_LEFTBRACET) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}
		
		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}
		
		ret_value = p_opteol();
		VALUE_CHECK();
		ret_value = p_statlist();
		VALUE_CHECK();
		
		break;
	case T_WFOR:
		GET_TOKEN();

		ret_value = p_defstat();
		VALUE_CHECK();

		if (token.type == T_SEMI) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		ret_value = expression();
		VALUE_CHECK();

		if (token.type == T_SEMI) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		ret_value = p_assignstat();
		VALUE_CHECK();

		if (token.type == T_LEFTBRACET) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}
		if (token.type == T_EOL) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		ret_value = p_statlist();
		VALUE_CHECK();

		break;
	case T_WRETURN:
		GET_TOKEN();

		ret_value = p_expressionlist();
		VALUE_CHECK();
		break;
	case T_ID:
		GET_TOKEN();

		ret_value = p_idstat();
		VALUE_CHECK();
		break;
	default:
		break;
	}
	return ret_value;
}

int p_defstat() {
	
	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_ID:
		GET_TOKEN();
		if (token.type == T_DOUBLEDOT) {     
			GET_TOKEN();

			ret_value = expression();
			VALUE_CHECK();
		} 
		break;

	case T_SEMI:
		ret_value = ERR_RIGHT;
		break;

	default:
		break;
	}
	return ret_value;
}

int p_assignstat() {
	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_ID:
		ret_value = p_idlist();
		VALUE_CHECK();
		ret_value = p_expressionlist();
		VALUE_CHECK();
		break;

	case T_LEFTBRACET:
		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_idlist() {

	int ret_value = ERR_SYNAN;

	if (token.type == T_ID) {
		GET_TOKEN();

		ret_value = p_idnext();
		VALUE_CHECK();
	}

	return ret_value;
}

int p_idnext() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_COMMA:
		GET_TOKEN();

		if (token.type == T_ID) {
			GET_TOKEN();

			ret_value = p_idnext();
			VALUE_CHECK();
		}

		break;

	case T_ASSIGN:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_expressionlist() {
	
	int ret_value = ERR_SYNAN;

	ret_value = expression();
	VALUE_CHECK();

	ret_value = p_expressionnext();
	VALUE_CHECK();

	return ret_value;
}

int p_expressionnext() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
		case T_COMMA:
			GET_TOKEN();

			ret_value = expression();
			VALUE_CHECK();
			ret_value = p_expressionnext();
			VALUE_CHECK();
			break;
		case T_LEFTBRACET:
		case T_EOL:
			ret_value = ERR_RIGHT;
			break;
		default:
			break;
	}
	return ret_value;
}

int p_termlist() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_ID:
	case T_INT:
	case T_DOUBLE:
	case T_STRING:
		ret_value = p_term();
		VALUE_CHECK();
		ret_value = p_termnext();
		VALUE_CHECK();
		break;

	case T_RIGHTBR:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_termnext() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_COMMA:
		GET_TOKEN();

		ret_value = p_term();
		VALUE_CHECK();
		ret_value = p_termnext();
		VALUE_CHECK();
		break;

	case T_RIGHTBR:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_term() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_ID:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_INT:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_STRING:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	case T_DOUBLE:
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}

int p_idstat() {

	int ret_value = ERR_SYNAN;
	
	switch (token.type) {
	case T_DOUBLEDOT:
		GET_TOKEN();
		
		ret_value = expression();
		VALUE_CHECK();
		break;

	case T_COMMA:
	case T_ASSIGN:
	
		ret_value = p_idnext();
		VALUE_CHECK();
		ret_value = p_exprorid();
		VALUE_CHECK();
		break;
	case T_LEFTBR:
		GET_TOKEN();

		ret_value = p_termlist();
		VALUE_CHECK();
		break;
	default:
		break;
	}
	return ret_value;
}

int p_exprorid() {
	
	int ret_value = ERR_SYNAN;
	
	switch (token.type) {
		case T_INT:
		case T_DOUBLE:
		case T_STRING:
		case T_LEFTBR:
			ret_value = p_expressionlist();
			VALUE_CHECK();
			break;
		case T_ID:
			PEEK_TOKEN();
			
			if (tokenp.type == T_LEFTBR) {
				GET_TOKEN();
				GET_TOKEN();

				ret_value = p_termlist();
				VALUE_CHECK();
			}
			else {
				ret_value = p_expressionlist();
				VALUE_CHECK();
			}
			break;
		default:
			break;
	}
	return ret_value;
}

int p_opteol() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
	case T_EOL:
		GET_TOKEN();

		ret_value = p_opteol();
		VALUE_CHECK();
		break;

	case T_WPACKAGE:
	case T_END_OF_FILE:
	case T_WFUNC:
	case T_WIF:
	case T_WFOR:
	case T_ID:
	case T_WRETURN:
	case T_RIGHTBRACET:
		ret_value = ERR_RIGHT;
		break;
	default:
		break;
	}

	return ret_value;
}