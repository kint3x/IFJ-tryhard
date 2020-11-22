/**
 * Projekt: Implementácia syntaktickej analýzy pre jazyk IFJ20
 *
 * @brief Implementácia stacku.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */

// #TODO vyhodnocovanie returnov -> expr
// #TODO vyhodnotenie funkcii do premenny -> spolu s expr
//#TODO err_stack  nstring "si" id func "kebab"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "scaner.h"
#include "expr.h"
#include "symtable.h"

#define ADD_GLOBAL_FUNCTION() int res_add_gl= BTree_newnode(&Global_tree,T_WFUNC,token.data,&Act_func); if(res_add_gl!=ERR_RIGHT) return res_add_gl;
#define LOCAL_TOP() int top_stack=BTStack_top(&Local_trees,&Act_scope); if(top_stack!=ERR_RIGHT) return top_stack;
#define LOCAL_POP() BTStack_pop(&Local_trees,&Act_scope);

#define SEARCH_AND_PUSH() ; if(1){\
Nstring *tmp=nstring_init();\
nstring_add_char(tmp,'_');\
if(nstring_cmp(saved_ID,tmp)==0){\
	nstring_add_char(stack_left,'n');\
}\
else{\
	nstring_free(tmp);\
	tmp=NULL;\
	BTreePtr search = BTStack_searchbyname(&Local_trees,saved_ID);\
	if(search==NULL) return ERR_SEMAN_NOT_DEFINED;\
	if(search->item_type==T_INT) nstring_add_char(stack_left,'i');\
	if(search->item_type==T_STRING) nstring_add_char(stack_left,'s');\
	if(search->item_type==T_DOUBLE) nstring_add_char(stack_left,'i');\
 }\
 nstring_free(tmp);\
}

bool BOOL_IN_FUNCTION;
bool PRINT;
Nstring *saved_ID;
tType saved_type;
int termcount;

Nstring *stack_left;
Nstring *func_args;


BTreeStackPtr Local_trees;
BTreeStackPtr Act_scope;

BTreePtr Act_node;
BTreePtr Act_func;
BTreePtr Global_tree;

Token token; // GLOBALNA PREMENNA S AKTUALNYM TOKENOM
Token tokenp; // pomocny token

void peek_nexttoken() {
	Token *t = getNextToken();   // NACITAME NOVY TOKEN
	//print_token(t);
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

int Init_builtinfunct(){
	BTreePtr new=NULL;
	Nstring *news=nstring_init();

	if(!nstring_add_str(news,"inputs")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"si")) return ERR_INTERNAL;
	new->num_returns = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"inputi")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"ii")) return ERR_INTERNAL;
	new->num_returns = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"inputf")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"fi")) return ERR_INTERNAL;
	new->num_returns = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"print")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;

	nstring_clear(news);

	if(!nstring_add_str(news,"int2float")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"f")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"i")) return ERR_INTERNAL;
	new->num_returns = 1;
	new->num_arguments = 1;

	nstring_clear(news);

	if(!nstring_add_str(news,"float2int")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"i")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"f")) return ERR_INTERNAL;
	new->num_returns = 1;
	new->num_arguments = 1;

	nstring_clear(news);

	if(!nstring_add_str(news,"len")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"i")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"s")) return ERR_INTERNAL;
	new->num_returns = 1;
	new->num_arguments = 1;

	nstring_clear(news);

	if(!nstring_add_str(news,"substr")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"si")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"sii")) return ERR_INTERNAL;
	new->num_returns = 2;
	new->num_arguments = 3;

	nstring_clear(news);

	if(!nstring_add_str(news,"ord")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"ii")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"si")) return ERR_INTERNAL;
	new->num_returns = 2;
	new->num_arguments = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"chr")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"si")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"i")) return ERR_INTERNAL;
	new->num_returns = 2;
	new->num_arguments = 1;

	nstring_free(news);
	return ERR_RIGHT;
}

void init_global_var(){
	
	token.type = T_UNKNOWN; // Inicializacia 
	token.data = NULL; // Nastavi dynstring na NULL
	tokenp.data = NULL;
	tokenp.type = -1;
	Local_trees=NULL;
	Act_node=NULL;
	Act_func=NULL;
	Act_scope=NULL;
	Global_tree=NULL;
	saved_type=T_UNKNOWN;
	saved_ID=nstring_init();
	stack_left=nstring_init();
	func_args=nstring_init();
	termcount=0;
	Init_builtinfunct();

}

int parse(){
	int	ret_value=ERR_SYNAN;

	init_global_var();

	ret_value=p_prog();

	//SKONTROLUJE CI SA NACHADZA MAIN
	Nstring *find_main=nstring_init();
	nstring_add_str(find_main,"main");
	if(BTree_findbyname(&Global_tree,find_main)==NULL){
		if(ret_value==ERR_RIGHT) ret_value=ERR_SEMAN_NOT_DEFINED;
	}
	nstring_free(find_main);
	// END KONTROLY

	//BTree_print(&Global_tree);
	BTStack_printall(&Local_trees);
	BTree_dispose(&Global_tree);
	BTStack_dispose(&Local_trees);
	nstring_free(saved_ID);
	nstring_free(stack_left);
	nstring_free(func_args);
	if(token.type!=T_END_OF_FILE)nstring_free(token.data); // uvolnit nstring v token.data
	return ret_value;
}

int p_prog() {

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
		;LOCAL_TOP();
		GET_TOKEN();

		if (token.type == T_ID) {
			ADD_GLOBAL_FUNCTION(); // Pridanie Funkcie
			GET_TOKEN();

			if (token.type == T_LEFTBR) {
				GET_TOKEN();

				ret_value = p_paramlist();
				VALUE_CHECK();
				Act_func->AoR=1; // prepnutie na return types
				ret_value = p_datatypelist();
				VALUE_CHECK();
				BOOL_IN_FUNCTION=true;
				ret_value = p_statlist();
				VALUE_CHECK();
				BOOL_IN_FUNCTION=false;

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
		nstring_add_str(saved_ID,token.data->string);
		GET_TOKEN();
		ret_value = p_datatype();
		VALUE_CHECK();
		nstring_clear(saved_ID);
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
			nstring_add_str(saved_ID,token.data->string);
			GET_TOKEN();
		}
		else break;

		ret_value = p_datatype();
		VALUE_CHECK();
		nstring_clear(saved_ID);
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
		if(!nstring_is_clear(saved_ID)){
			BTStack_newnode(&Act_scope,T_INT, saved_ID);
		}
		BTree_insertAoR(Act_func,T_WINT);

		ret_value = ERR_RIGHT;
		break;

	case T_WFLOAT64:
		GET_TOKEN();
		if(!nstring_is_clear(saved_ID)){
			BTStack_newnode(&Act_scope,T_DOUBLE, saved_ID);
		}
		BTree_insertAoR(Act_func,T_WFLOAT64);
		ret_value = ERR_RIGHT;
		break;

	case T_WSTRING:
		GET_TOKEN();
		if(!nstring_is_clear(saved_ID)){
			BTStack_newnode(&Act_scope,T_STRING, saved_ID);
		}
		BTree_insertAoR(Act_func,T_WSTRING);
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
		LOCAL_POP();
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
			;LOCAL_TOP();
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
			LOCAL_TOP();
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
		;LOCAL_TOP();
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
		;bool ad_res=nstring_add_str(saved_ID,token.data->string); if(!ad_res) return ERR_INTERNAL;
		saved_type=token.type;

		GET_TOKEN();

		ret_value = p_idstat();
		VALUE_CHECK();
		nstring_clear(saved_ID);
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
		SEARCH_AND_PUSH();
		GET_TOKEN();

		if (token.type == T_ID) {
			GET_TOKEN();

			ret_value = p_idnext();
			VALUE_CHECK();
		}

		break;

	case T_ASSIGN:
		SEARCH_AND_PUSH();

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
		if((func_args->string)[termcount]!='\0') return ERR_SEMAN_PARAMETERS;
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
		;
		BTreePtr search = BTStack_searchbyname(&Local_trees,token.data); // vyhlada premennu
		if(search == NULL) return ERR_SEMAN_NOT_DEFINED; // ak nenajde neexistuje
		if(!PRINT){
		char cmp;
		if(search->item_type==T_INT){cmp = 'i';} if(search->item_type==T_DOUBLE){ cmp = 'f';} if(search->item_type==T_STRING){cmp = 's';}
		if(cmp!=(func_args->string)[termcount]) return ERR_SEMAN_PARAMETERS;
		termcount++;
		}
		else { 

		//VOLA GENERATOR PRINT
		}

		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_INT:
	   	;if(!PRINT){
		if('i'!=(func_args->string)[termcount]) return ERR_SEMAN_PARAMETERS;
		termcount++;
	  	}
	  	else { //VOLA GENERATOR PRINT
		}
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_STRING:
		;if(!PRINT){
		if('s'!=(func_args->string)[termcount]) return ERR_SEMAN_PARAMETERS;
		termcount++;}
		else { //VOLA GENERATOR PRINT
		}
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	case T_DOUBLE:
		;if(!PRINT){
		if('f'!=(func_args->string)[termcount]) return ERR_SEMAN_PARAMETERS;
		termcount++; }
		else { //VOLA GENERATOR PRINT
		}

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

		ret_value = expression(); // > odtialto dostanem saved_type
		VALUE_CHECK();

		//expr &saved_type//expr
		/*if( (saved_type==T_INT) || (saved_type==T_STRING )||(saved_type == T_DOUBLE)){  //Prida premennu do stromu, ak nie je ani jedneho typu zavola chybu
			ret_value=BTStack_newnode(&Act_scope,saved_type, saved_ID);
			VALUE_CHECK();
		}
		else return ERR_SEMAN_OTHERS;
		*/

		break;

	case T_COMMA:
	case T_ASSIGN:
	
		ret_value = p_idnext();
		VALUE_CHECK();
		ret_value = p_exprorid();
		VALUE_CHECK();
		break;
	case T_LEFTBR:
		//SEMSOM
		if(nstring_str_cmp(saved_ID,"print")==0) PRINT=true;
		else PRINT = false;

		BTreePtr search = BTree_findbyname(&Global_tree,saved_ID); // pokusi sa najst funkciu v strome
		if(search == NULL) return ERR_SEMAN_NOT_DEFINED; // funkcia neexistuje
		nstring_clear(func_args); // vynuluje lavy zasobnik
		nstring_add_str(func_args,search->args->string); // prida argumenty funkcie do zasobníka

		GET_TOKEN();

		termcount=0;
		ret_value = p_termlist();
		VALUE_CHECK();
		termcount=0;
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
				if(nstring_str_cmp(token.data,"print")==0) PRINT=true;
				else PRINT = false;
				nstring_clear(saved_ID); nstring_add_str(saved_ID,(token.data)->string); // uchova id
				BTreePtr search = BTree_findbyname(&Global_tree,saved_ID); // pokusi sa najst funkciu v strome
				if(search == NULL) return ERR_SEMAN_NOT_DEFINED; // funkcia neexistuje
				nstring_clear(func_args); // vynuluje lavy zasobnik
				nstring_add_str(func_args,search->args->string); // prida argumenty funkcie do zasobníka

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