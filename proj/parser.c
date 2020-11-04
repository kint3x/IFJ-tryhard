
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define VALUE_CHECK() if (ret_value != ERR_RIGHT) return ret_value;
#define GET_TOKEN() p_getnexttoken(); if (token.type == T_UNKNOWN) return ERR_LEXSCAN; 

Token token; // GLOBALNA PREMENNA S AKTUALNYM TOKENOM
//token.data=NULL;

void p_getnexttoken(){
	//nstring_free(token.data);  // odalokovanie predosleho dynstringu
	Token *t=getNextToken();    // uloží si nový token
	if(t==NULL){
		token.type=ERR_INTERNAL;
		return;
	}
	token.type=t->type;         // vloží ho do globálnej premennej
	token.data=t->data;
	print_token(&token);
	//free(t);					//uvolní token (nafukovací string neuvolní)
	if(token.type==T_END_OF_FILE){		// ak je token end uvolni aktualny dynstring pre END
		//nstring_free(token.data);     
	}
}

int expr_or_cond(){
	//TATO FUNKCIA BUDE HANDLOVAT CI JE TO EXPRESSION ALEBO CONDITION
}

int p_prog() {
	
	int ret_value = ERR_SYNAN;
	GET_TOKEN();

	if (token.type == T_WPACKAGE || token.type == T_EOL) {
		if (!p_opteol()) {
			if (token.type == T_WPACKAGE) {
				GET_TOKEN();
				if (token.type == T_ID) {
					GET_TOKEN();
					if (token.type == T_EOL) {
						GET_TOKEN();
						ret_value = p_opteol();
						VALUE_CHECK();

						ret_value = p_funclist();
						VALUE_CHECK();
					}
				}
			}
		}
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
				else ret_value = ERR_SYNAN;
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
			else ret_value = ERR_SYNAN;

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
	// #TODO Kebabova implementacia zatial
	int ret_value=ERR_SYNAN;
		switch(token.type){
			case T_WIF:
				GET_TOKEN();
				ret_value=expr_or_cond();
				VALUE_CHECK();
				if(token.type!=T_LEFTBRACET) return ERR_SYNAN;
				GET_TOKEN();
				if(token.type!=T_EOL) return ERR_SYNAN;
				GET_TOKEN();
				ret_value=p_opteol();
				VALUE_CHECK();
				ret_value=p_statlist();
				VALUE_CHECK();
				if(token.type!=T_WELSE) return ERR_SYNAN;
				GET_TOKEN();
				if(token.type!=T_LEFTBRACET) return ERR_SYNAN;
				GET_TOKEN();
				if(token.type!=T_EOL) return ERR_SYNAN;
				GET_TOKEN();
				ret_value=p_opteol();
				VALUE_CHECK();
				ret_value=p_statlist();
				VALUE_CHECK();		
				ret_value=ERR_RIGHT;		
				break;
			case T_WFOR:
				GET_TOKEN();
				ret_value=p_defstat();
				VALUE_CHECK();
				if(token.type!=T_SEMI) return ERR_SYNAN;
				GET_TOKEN();
				ret_value=expr_or_cond();
				VALUE_CHECK();
				if(token.type!=T_SEMI) return ERR_SYNAN;
				GET_TOKEN();
				ret_value=p_assignstat();
				VALUE_CHECK();
				if(token.type!=T_LEFTBRACET) return ERR_SYNAN;
				GET_TOKEN();
				if(token.type!=T_EOL) return ERR_SYNAN;
				GET_TOKEN();
				ret_value=p_statlist();
				VALUE_CHECK();
				ret_value=ERR_RIGHT;
				break;
			case T_WRETURN:
				GET_TOKEN();
				ret_value=p_expressionlist();
				VALUE_CHECK();
				ret_value=ERR_RIGHT;
				break;
			case T_ID:
				GET_TOKEN();
				ret_value=p_idstat();
				VALUE_CHECK();
				ret_value=ERR_RIGHT;
			default:
				break;
		}
	return ret_value;
}

int p_defstat() {
	// #TODO Kebabova implementacia zatial
	int ret_value=ERR_SYNAN;
		switch(toke.type){
			case T_ID:
				GET_TOKEN();
				if(token.type!=T_DOUBLEDOT) return ERR_SYNAN;
				GET_TOKEN();
				if(token.type!=T_ASSIGN) return ERR_SYNAN;
				GET_TOKEN();
				ret_value=expr_or_cond();
				VALUE_CHECK();
				ret_value=ERR_RIGHT;	
				break;
			case T_SEMI:
				ret_value=ERR_RIGHT;
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
	//#TODO ZATIAL KEBABOVA IMPLEMENTACIA
	int ret_value=ERR_SYNAN;

	return ret_value;
}

int p_expressionnext() {
	int ret_value=ERR_SYNAN;
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
	// #TODO
	int ret_value=ERR_SYNAN;
	return ret_value;
}

int p_exprorid() {
	// #TODO
	int ret_value=ERR_SYNAN;
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