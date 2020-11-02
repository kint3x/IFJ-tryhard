
#include <stdio.h>

Token token=NULL; // GLOBALNA PREMENNA S AKTUALNYM TOKENOM


void p_getnexttoken(){
	if(token!=NULL){               //Ak token nie je null tak na začiatku uvolní predošlý nafukovací string
		nstring_free(token.data);
	}

	Token *t=getNextToken();    // uloží si nový token
	token.type=t->type;         // vloží ho do globálnej premennej
	token.data=t->data;
	
	free(t);					//uvolní token (nafukovací string neuvolní)
	
	if(token.type==END){		// ak je token end uvolni aktualny dynstring pre END
		nstring_free(token.data);     
	}
}

int p_prog(){

	return 0;

}