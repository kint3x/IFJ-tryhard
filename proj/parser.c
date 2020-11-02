
#include <stdio.h>

Token token; // GLOBALNA PREMENNA S AKTUALNYM TOKENOM
token.data=NULL;

int p_getnexttoken(){
              
	nstring_free(token.data);  // odalokovanie predosleho dynstringu

	Token *t=getNextToken();    // uloží si nový token
	if(t==NULL){
		return ERR_INTERNAL;
	}
	token.type=t->type;         // vloží ho do globálnej premennej
	token.data=t->data;
	
	free(t);					//uvolní token (nafukovací string neuvolní)
	
	if(token.type==T_END_OF_FILE){		// ak je token end uvolni aktualny dynstring pre END
		nstring_free(token.data);     
	}
}

int p_prog(){

	return 0;

}