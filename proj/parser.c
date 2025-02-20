/**
 * Projekt: Implementace překladače imperativního jazyka IFJ20.
 *
 * @brief Implementácia syntaktickej a sémantickej analýzy.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */

// #TODO vyhodnocovanie returnov -> expr
// #TODO conditions, for 
// #TODO vyhodnotenie funkcii do premenny -> spolu s expr

#include "parser.h"

#define ADD_GLOBAL_FUNCTION() int res_add_gl= BTree_newnode(&Global_tree,T_WFUNC,token.data,&Act_func,0); if(res_add_gl!=ERR_RIGHT) return res_add_gl;
#define LOCAL_TOP() int top_stack=BTStack_top(&Local_trees,&Act_scope); if(top_stack!=ERR_RIGHT) return top_stack; uniq_scope++;
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
	if(search->item_type==T_DOUBLE) nstring_add_char(stack_left,'f');\
 }\
 nstring_free(tmp);\
}

bool BOOL_IN_FUNCTION;
bool BOOL_DEFINED_FUN;
bool BOOL_PARAM_OR_DATA;
bool BOOL_IN_RETURN;
bool BOOL_IN_WFOR;
bool BOOL_FOUND_RETURN;

bool PRINT;

Nstring *saved_uniq;

Nstring *saved_ID;
tType saved_type;
int termcount;
int arg_count;
int gl_counter;
unsigned int uniq_scope;

Nstring *stack_left;
Nstring *func_args;
Nstring *right_expr;

EStackPtr Err_stack;

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
		//print_token(t);
		token.type = t->type;
		token.data = t->data;
		free(t);					// UVOLNIME STRUKTURU 
		t = NULL;
	}
}
 void stderr_print(int ret_value){
 	if(ret_value==ERR_SEMAN_NOT_DEFINED){
 		fprintf(stderr, "semantická chyba v programu – nedefinovaná funkce/promenna, pokus o redefi-nici funkce/promenne, atp\n");
 	}
 	if(ret_value==ERR_SEMAN_TYPE_COMPATIBILITY){
 		fprintf(stderr, "semanticka chyba typove kompatibility v aritmetickych, retezcovych a relacnich vyrazech\n");
 	}
 	if(ret_value==ERR_SEMAN_PARAMETERS){
 		fprintf(stderr, "semantická chyba v programu – spatny pocet/typ parametru ci navratovych hodnotu volani funkce ci navratu z funkce.\n");
 	}
 	if(ret_value==ERR_SEMAN_OTHERS){
 		fprintf(stderr, "ostatni semanticke chyby\n");
 	}
 	if(ret_value==ERR_ZERO_DIVIDING){
 		fprintf(stderr, "deleni nulou\n");
 	}
 }

int Init_builtinfunct(){
	BTreePtr new=NULL;
	Nstring *news=nstring_init();

	if(!nstring_add_str(news,"inputs")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"si")) return ERR_INTERNAL;
	new->num_returns = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"inputi")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"ii")) return ERR_INTERNAL;
	new->num_returns = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"inputf")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"fi")) return ERR_INTERNAL;
	new->num_returns = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"print")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;

	nstring_clear(news);

	if(!nstring_add_str(news,"int2float")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"f")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"i")) return ERR_INTERNAL;
	new->num_returns = 1;
	new->num_arguments = 1;

	nstring_clear(news);

	if(!nstring_add_str(news,"float2int")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"i")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"f")) return ERR_INTERNAL;
	new->num_returns = 1;
	new->num_arguments = 1;

	nstring_clear(news);

	if(!nstring_add_str(news,"len")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"i")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"s")) return ERR_INTERNAL;
	new->num_returns = 1;
	new->num_arguments = 1;

	nstring_clear(news);

	if(!nstring_add_str(news,"substr")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"si")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"sii")) return ERR_INTERNAL;
	new->num_returns = 2;
	new->num_arguments = 3;

	nstring_clear(news);

	if(!nstring_add_str(news,"ord")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
	if(!nstring_add_str(new->returns,"ii")) return ERR_INTERNAL;
	if(!nstring_add_str(new->args,"si")) return ERR_INTERNAL;
	new->num_returns = 2;
	new->num_arguments = 2;

	nstring_clear(news);

	if(!nstring_add_str(news,"chr")) return ERR_INTERNAL;
	if(BTree_newnode(&Global_tree, T_WFUNC,news,&new,0)!=ERR_RIGHT) return ERR_INTERNAL;
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
	uniq_scope=0;
	Local_trees=NULL;
	Act_node=NULL;
	Act_func=NULL;
	Act_scope=NULL;
	Global_tree=NULL;
	Err_stack=NULL;
	saved_type=T_UNKNOWN;
	saved_ID=nstring_init();
	stack_left=nstring_init();
	func_args=nstring_init();
	right_expr=nstring_init();
	saved_uniq=nstring_init();
	termcount=0;
	Init_builtinfunct();

}

int parse(){
	int	ret_value=ERR_SYNAN;
	generate_start(); // štart generácie

	init_global_var();

	ret_value=p_prog();

	//SKONTROLUJE CI SA NACHADZA MAIN a ma urceny pocet typov a argumentov
	Nstring *find_main=nstring_init();
	nstring_add_str(find_main,"main");
	BTreePtr find=BTree_findbyname(&Global_tree,find_main);
	if(ret_value==ERR_RIGHT){
		if(find==NULL){
			ret_value=ERR_SEMAN_NOT_DEFINED;
		}
		else{
			if((find->num_arguments != 0) || (find->num_returns !=0 )) {
				ret_value= ERR_SEMAN_PARAMETERS;
			}
		}
	}
	nstring_free(find_main);
	// END KONTROLY
	// VYriešenie problémov na stacku
	if(ret_value==ERR_RIGHT){
		ret_value=EStack_solveproblems(&Err_stack,Global_tree);
	}
	//BTree_print(&Global_tree);
	//BTStack_printall(&Local_trees);
	BTree_dispose(&Global_tree);

	BTStack_dispose(&Local_trees);
	EStack_dispose(&Err_stack);
	nstring_free(saved_ID);
	nstring_free(stack_left);
	nstring_free(func_args);
	nstring_free(right_expr);
	nstring_free(saved_uniq);

	if(ret_value==ERR_RIGHT) G_PRINT();
	G_FREEALL();
	if(token.type!=T_END_OF_FILE)nstring_free(token.data); // uvolnit nstring v token.data

	stderr_print(ret_value);
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
			nstring_cpy(token.data,saved_ID);
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}

		if (token.type == T_EOL) {
			if(nstring_str_cmp(saved_ID,"main")!=0){return ERR_SEMAN_OTHERS;}
			nstring_clear(saved_ID);
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
		G_buildfun();
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
	BOOL_FOUND_RETURN = false; // incializacia ci najde return
	if (token.type == T_WFUNC) {
		;LOCAL_TOP();
		GET_TOKEN();
		if (token.type == T_ID) {
			ADD_GLOBAL_FUNCTION(); // Pridanie Funkcie

			G_Fun_header(token.data);//GENERACIA NAZVU

			GET_TOKEN();

			if (token.type == T_LEFTBR) {
				GET_TOKEN();
				gl_counter=1;//global counter na 0
				BOOL_PARAM_OR_DATA=true;
				ret_value = p_paramlist();
				VALUE_CHECK();
				Act_func->AoR=1; // prepnutie na return types
				gl_counter=1;//global counter na 0
				BOOL_PARAM_OR_DATA=false;
				ret_value = p_datatypelist();
				VALUE_CHECK();
				BOOL_IN_FUNCTION=true;
				ret_value = p_statlist();
				VALUE_CHECK();
				BOOL_IN_FUNCTION=false;

				// kontrola ci naslo return ak tam mal byť
				if(!BOOL_FOUND_RETURN){
					if(Act_func->returns->string[0]!='\0') return ERR_SEMAN_PARAMETERS; // Ak nenašiel return ale funkcia má return values tak err	
					G_Fun_return();//Print return pre void funkcie
				}

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
		G_Fun_argument(token.data,gl_counter++,uniq_scope); // Generacia argumentu
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
			G_Fun_argument(token.data,gl_counter++,uniq_scope); // Generacia argumentu
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
		//CHANGED PARSER # Tento block som pridal aby podporovalo fun id()()
		if(token.type==T_RIGHTBR){
			GET_TOKEN();
			if(token.type==T_LEFTBRACET){
				GET_TOKEN();
				if(token.type==T_EOL){
					ret_value=p_opteol();
					VALUE_CHECK();
				}
			}
			break;
		}
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
		if(!BOOL_PARAM_OR_DATA) G_Fun_ret_value(gl_counter++); //iba ak sme v navratovych datovych typoch funkcie
		GET_TOKEN();
		if(!nstring_is_clear(saved_ID)){
			BTStack_newnode(&Act_scope,T_INT, saved_ID,uniq_scope);
		}
		BTree_insertAoR(Act_func,T_WINT);
		ret_value = ERR_RIGHT;
		break;

	case T_WFLOAT64:
		if(!BOOL_PARAM_OR_DATA) G_Fun_ret_value(gl_counter++); //iba ak sme v navratovych datovych typoch funkcie
		GET_TOKEN();
		if(!nstring_is_clear(saved_ID)){
			BTStack_newnode(&Act_scope,T_DOUBLE, saved_ID,uniq_scope);
		}
		BTree_insertAoR(Act_func,T_WFLOAT64);
		ret_value = ERR_RIGHT;
		break;

	case T_WSTRING:
		if(!BOOL_PARAM_OR_DATA) G_Fun_ret_value(gl_counter++); //iba ak sme v navratovych datovych typoch funkcie
		GET_TOKEN();
		if(!nstring_is_clear(saved_ID)){
			BTStack_newnode(&Act_scope,T_STRING, saved_ID,uniq_scope);
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
		
		int lablel_uniq=uniq_scope; // preberie Uniq label pre IF
		bool tmp_cond;  // semanticka kontrola či je podmienka v ife
		tType tmp_check;

		ret_value = expression(&tmp_check,&tmp_cond,Local_trees);
		VALUE_CHECK();

		G_if_label_cond(lablel_uniq);

		if(!tmp_cond) return ERR_SEMAN_TYPE_COMPATIBILITY; // semanticka chyba ak nie

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
		G_if_label(lablel_uniq);
		ret_value = p_statlist();
		VALUE_CHECK();
		G_end_else(lablel_uniq);
		
		break;
	case T_WFOR:
		;LOCAL_TOP();
		GET_TOKEN();
		BOOL_IN_WFOR=true;
		ret_value = p_defstat();
		int temp_uniq_scope=uniq_scope;
		VALUE_CHECK();
		if (token.type == T_SEMI) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}
		G_label_for_for(temp_uniq_scope); // zaciatok foru
		ret_value = expression(&tmp_check,&tmp_cond,Local_trees);
		VALUE_CHECK();
		G_for_label_cond(temp_uniq_scope); // vyhodnotenie cond

		if(!tmp_cond) return ERR_SEMAN_TYPE_COMPATIBILITY; // semanticka chyba ak nie je podmienka

		if (token.type == T_SEMI) {
			GET_TOKEN();
		}
		else {
			ret_value = ERR_SYNAN;
			VALUE_CHECK();
		}
		G_for_ass_start(temp_uniq_scope);
		ret_value = p_assignstat();
		VALUE_CHECK();

		G_for_ass_end(temp_uniq_scope);
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
		BOOL_IN_WFOR=false;

		top_stack=BTStack_top(&Local_trees,&Act_scope); if(top_stack!=ERR_RIGHT) return top_stack; uniq_scope++;// LOCAL TOP() bez int=

		ret_value = p_statlist();
		VALUE_CHECK();
		G_for_end(temp_uniq_scope);
		;LOCAL_POP();

		break;
	case T_WRETURN:
		GET_TOKEN();
		BOOL_FOUND_RETURN=true;
		BOOL_IN_RETURN = true; // definujem že kontrolujem return
		gl_counter=1; //gl counter pre pocitanie
		if(token.type==T_EOL) {   BOOL_IN_RETURN = false; return ERR_RIGHT;  } //hotfix return epsilon pravidlo 
		ret_value = p_expressionlist();
		VALUE_CHECK();
		BOOL_IN_RETURN = false; // definujem že  končím kontrolovanie returnu
			
		if(strcmp(Act_func->returns->string,right_expr->string)!=0) return ERR_SEMAN_PARAMETERS; // Vracia zle typy
		G_Fun_return(); // Generuje koniec funkcie

		break;
	case T_ID:
		;bool ad_res=nstring_add_str(saved_ID,token.data->string); if(!ad_res) return ERR_INTERNAL;
		saved_type=token.type;
		nstring_clear(saved_uniq); // uvolnenie pre unikátne
		GET_TOKEN();

		ret_value = p_idstat();
		VALUE_CHECK();


		nstring_clear(stack_left);
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
		nstring_clear(saved_ID); nstring_add_str(saved_ID,(token.data)->string); // UCHOVAM ID
		GET_TOKEN();
		if (token.type == T_DOUBLEDOT) {     
			GET_TOKEN();

			bool tmp_cond;
			tType tmp_check;
			ret_value = expression(&tmp_check,&tmp_cond,Local_trees);
			VALUE_CHECK();

			if(tmp_cond) return ERR_SEMAN_OTHERS; // v definicii nemoze byt relacny operator
			if( (tmp_check==T_INT) || (tmp_check==T_STRING )||(tmp_check == T_DOUBLE)){  //Prida premennu do stromu, ak nie je ani jedneho typu zavola chybu
				ret_value=BTStack_newnode(&Act_scope,tmp_check, saved_ID,uniq_scope);
				VALUE_CHECK();
			}
			else return ERR_SEMAN_OTHERS;
			BTreePtr search = BTStack_searchbyname(&Act_scope,saved_ID); if(search==NULL) return ERR_INTERNAL;
			G_defstat_var(saved_ID,search->uniq_scope);
			
			nstring_clear(saved_ID);
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
		nstring_clear(saved_ID);nstring_cpy(token.data,saved_ID);
		ret_value = p_idlist();
		VALUE_CHECK();
		ret_value = p_expressionlist();
		VALUE_CHECK();
		nstring_clear(saved_ID);
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
		char sbuf[30];
		BTreePtr temp = BTStack_searchbyname(&Local_trees,saved_ID);
		if(nstring_str_cmp(saved_ID,"_")!=0){
		snprintf(sbuf, 30, "%d",temp->uniq_scope);
		nstring_add_str(saved_uniq,saved_ID->string);nstring_add_str(saved_uniq,sbuf); nstring_add_char(saved_uniq,'|');
		}
		else{
			nstring_add_str(saved_uniq,saved_ID->string);nstring_add_char(saved_uniq,'|');
		}
		GET_TOKEN();
		if (token.type == T_ID) {
			nstring_clear(saved_ID); nstring_add_str(saved_ID,(token.data)->string); // UCHOVAM ID
			GET_TOKEN();

			ret_value = p_idnext();
			VALUE_CHECK();
		}

		break;

	case T_ASSIGN:
		SEARCH_AND_PUSH();
		if(nstring_str_cmp(saved_ID,"_")!=0){
			temp = BTStack_searchbyname(&Local_trees,saved_ID);
			sprintf(sbuf,"%d",temp->uniq_scope);
			nstring_add_str(saved_uniq,saved_ID->string);
			nstring_add_str(saved_uniq,sbuf);
		}
		else {
			nstring_add_str(saved_uniq,saved_ID->string);
		}

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
	nstring_clear(right_expr);
	bool tmp_cond;
	tType tmp_check;
	ret_value = expression(&tmp_check,&tmp_cond,Local_trees);
	VALUE_CHECK();
	nstring_push_type(right_expr,tmp_check);
	//Blok pre tlačenie POPS
	if(BOOL_IN_RETURN){
		G_Fun_pop_to_ret(gl_counter);
		gl_counter++;
	}
	else{
	Nstring *new=nstring_init();
	nstring_get_and_delete(saved_uniq,new);
	G_expr_pops(new);
	nstring_free(new);
	}
	// Koniec bloku
	ret_value = p_expressionnext();
	VALUE_CHECK();

	return ret_value;
}

int p_expressionnext() {

	int ret_value = ERR_SYNAN;

	switch (token.type) {
		case T_COMMA:
			GET_TOKEN();

			bool tmp_cond;
			tType tmp_check;
			ret_value = expression(&tmp_check,&tmp_cond,Local_trees);
			VALUE_CHECK();
			nstring_push_type(right_expr,tmp_check);
			//Blok pre tlačenie POPS
			if(BOOL_IN_RETURN){
				G_Fun_pop_to_ret(gl_counter);
				gl_counter++;
			}
			else{
				Nstring *new=nstring_init();
				nstring_get_and_delete(saved_uniq,new);
				G_expr_pops(new);
				nstring_free(new);
			}
			// Koniec bloku
			ret_value = p_expressionnext();
			VALUE_CHECK();
			break;
		case T_LEFTBRACET:
		case T_EOL:
			if(BOOL_IN_RETURN){
					//return neporovnava parametre
			}
			else if(BOOL_IN_WFOR){
				if(!nstring_ret_cmp(stack_left,right_expr)) return ERR_SEMAN_PARAMETERS;  //kontrola ci priradzovanie premmenny
			}
			else{
				if(!nstring_ret_cmp(stack_left,right_expr)) return ERR_SEMAN_OTHERS;  //kontrola ci priradzovanie premmenny je spravny pocet/typ id,id2 = 5 , b
			}
			nstring_clear(stack_left);
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
		arg_count=1;
		ret_value = p_term();
		VALUE_CHECK();
		ret_value = p_termnext();
		VALUE_CHECK();
		break;

	case T_RIGHTBR:
		if(BOOL_DEFINED_FUN){ // ak bola funkcia definovaná zistíme, inak hádžeme na ERR Stack
			if((func_args->string)[termcount]!='\0') return ERR_SEMAN_PARAMETERS; // Mala mať funkcia argumenty ale nemala? Chyba!
			if(!nstring_is_clear(stack_left)){ // ak na lavo máme premenné, musíme ich porovnať s return types funkcie
				if(!nstring_ret_cmp(stack_left,(BTree_findbyname(&Global_tree,saved_ID))->returns)) return ERR_SEMAN_PARAMETERS;
			}else{ //ak na lavo nemáme premenné, musíme zistiť, či funkcia má návratové hodnoty, inak vráti chybu
				if(BTree_findbyname(&Global_tree,saved_ID)->num_returns>0) return ERR_SEMAN_PARAMETERS;
			}
		}
		else{
			if(EStack_addcall(&Err_stack,saved_ID,stack_left,func_args)==ERR_INTERNAL) return ERR_INTERNAL; //hádžeme na stack
		}
		
		if(!PRINT) G_callfunc(saved_ID);
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
		if(BOOL_DEFINED_FUN){ // ak bola funkcia definovaná zistíme, inak hádžeme na ERR Stack
			if(!nstring_is_clear(stack_left)){ // ak na lavo máme premenné, musíme ich porovnať s return types funkcie
				if(!nstring_ret_cmp(stack_left,(BTree_findbyname(&Global_tree,saved_ID))->returns)) return ERR_SEMAN_PARAMETERS;
			}else {//ak na lavo nemáme premenné, musíme zistiť, či funkcia má návratové hodnoty, inak vráti chybu
				if(BTree_findbyname(&Global_tree,saved_ID)->num_returns>0) return ERR_SEMAN_PARAMETERS;
			}
		}
		else{
			if(EStack_addcall(&Err_stack,saved_ID,stack_left,func_args)==ERR_INTERNAL) return ERR_INTERNAL; //hádžeme na stack
		}
		if(!PRINT) G_callfunc(saved_ID);
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
		; // Nájde či premmenna existuje, porovná ci typy sedia podla indexu  termcount, v func_args mám skopírované argumenty funkcie v hlavičke
		if(strcmp(token.data->string,"_")==0) return ERR_SEMAN_PARAMETERS; // _ nemôže byť v parametroch
		BTreePtr search = BTStack_searchbyname(&Local_trees,token.data); // vyhlada premennu
		if(search == NULL) return ERR_SEMAN_NOT_DEFINED; // ak nenajde neexistuje
		if(!PRINT){
			char cmp;
			if(search->item_type==T_INT){cmp = 'i';} if(search->item_type==T_DOUBLE){ cmp = 'f';} if(search->item_type==T_STRING){cmp = 's';}
			if(BOOL_DEFINED_FUN){ //Ak je už definovaná porovná, inak hádže typ do dynstringu
				if(cmp!=(func_args->string)[termcount]) return ERR_SEMAN_PARAMETERS;
				termcount++; 
			}else
			{
				nstring_add_char(func_args,cmp);
			}
			char buf[100];
			sprintf(buf,"%s%d",search->name->string,search->uniq_scope);
			Nstring *tmp = nstring_init();
			nstring_add_str(tmp,buf);
			G_callfunc_arg(tmp,arg_count++,token.type);
			nstring_free(tmp);
		}
		else { 
		G_fun_print(token.data, token.type,search->uniq_scope);
		//VOLA GENERATOR PRINT
		}

		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_INT:
	   	;
	   	if(!PRINT){
		   	if(BOOL_DEFINED_FUN){
		   		if('i'!=(func_args->string)[termcount]){ 
		   			return ERR_SEMAN_PARAMETERS;
		  		}
		  		termcount++;
		   	}
		   	else{
					nstring_add_char(func_args,'i');
				}
			G_callfunc_arg(token.data,arg_count++,token.type);
		}
		else { //VOLA GENERATOR PRINT
			G_fun_print(token.data, token.type,0);
		}
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;

	case T_STRING:
		;if(!PRINT){
			if(BOOL_DEFINED_FUN){
		   		if('s'!=(func_args->string)[termcount]){
		   			return ERR_SEMAN_PARAMETERS;
		  		}
		  		termcount++;
		   	}
		   	else
				{
					nstring_add_char(func_args,'s');
				}
			G_callfunc_arg(token.data,arg_count++,token.type);
		}
		else { //VOLA GENERATOR PRINT
			G_fun_print(token.data, token.type,0);
		}
		GET_TOKEN();

		ret_value = ERR_RIGHT;
		break;
	case T_DOUBLE:
		;if(!PRINT){
			if(BOOL_DEFINED_FUN){
		   		if('f'!=(func_args->string)[termcount]){ 
		   			return ERR_SEMAN_PARAMETERS;
		  		}
		  		termcount++;
		   	}
		   	else
				{
					nstring_add_char(func_args,'f');
				}
			G_callfunc_arg(token.data,arg_count++,token.type);
		} 
		else { //VOLA GENERATOR PRINT
			G_fun_print(token.data, token.type,0);
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
		
		bool tmp_cond;
		saved_type=T_UNKNOWN;
		ret_value = expression(&saved_type,&tmp_cond,Local_trees); // > odtialto dostanem saved_type
		VALUE_CHECK();
		if(tmp_cond) return ERR_SEMAN_OTHERS; // Nemôžeme priradit bool operatory
		if( (saved_type==T_INT) || (saved_type==T_STRING )||(saved_type == T_DOUBLE)){  //Prida premennu do stromu, ak nie je ani jedneho typu zavola chybu
			ret_value=BTStack_newnode(&Act_scope,saved_type, saved_ID,uniq_scope);
			VALUE_CHECK();
		}
		else return ERR_SEMAN_OTHERS;
		BTreePtr tmp_sr=BTStack_searchbyname(&Local_trees,saved_ID); if(tmp_sr==NULL) return ERR_INTERNAL;
		G_declare_var(tmp_sr->name,tmp_sr->uniq_scope);
		nstring_clear(saved_uniq);
		char buf[30];
		sprintf(buf,"%d",tmp_sr->uniq_scope);
		nstring_add_str(saved_uniq,tmp_sr->name->string);
		nstring_add_str(saved_uniq,buf);
		G_expr_pops(saved_uniq);
		nstring_clear(saved_uniq);
		break;

	case T_COMMA:
	case T_ASSIGN:
	
		ret_value = p_idnext();
		VALUE_CHECK();
		ret_value = p_exprorid();
		VALUE_CHECK();
		break;
	case T_LEFTBR:
		if((BTStack_searchbyname(&Local_trees,saved_ID))!=NULL) return ERR_SEMAN_OTHERS; // skúšam či funkcia nie je prekrytá premennou
		if(nstring_str_cmp(saved_ID,"print")==0) PRINT=true;      // Funkcia print má iné sem. pravidlá
		else PRINT = false;

		BTreePtr search = BTree_findbyname(&Global_tree,saved_ID); // pokusi sa najst funkciu v strome
		if(search != NULL){
					BOOL_DEFINED_FUN=true; // funkcia existuje
					nstring_clear(func_args); // vynuluje zasobnik pre argumenty funkcie
					nstring_add_str(func_args,search->args->string); // prida argumenty funkcie do zasobníka
		}
		else {
				BOOL_DEFINED_FUN=false; //existuje
				nstring_clear(func_args);
		}
		G_createframe();

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
				G_createframe();
				if(nstring_str_cmp(token.data,"print")==0) PRINT=true;
				else PRINT = false;
				nstring_clear(saved_ID); nstring_add_str(saved_ID,(token.data)->string); // uchova id
				BTreePtr search = BTree_findbyname(&Global_tree,saved_ID); // pokusi sa najst funkciu v strome
				if(search != NULL){
					BOOL_DEFINED_FUN=true; // funkcia existuje
					nstring_clear(func_args); // vynuluje lavy zasobnik
					nstring_add_str(func_args,search->args->string); // prida argumenty funkcie do zasobníka
				}
				else {
				BOOL_DEFINED_FUN=false; //existuje
				nstring_clear(func_args);
				}

				GET_TOKEN();
				GET_TOKEN();
				termcount=0;
				ret_value = p_termlist();
				VALUE_CHECK();
				
				//G_callfunc(saved_ID);
				G_aftercall_empty_write(saved_uniq);
				nstring_clear(saved_uniq);
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