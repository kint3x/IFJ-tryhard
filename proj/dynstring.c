/**
 * Projekt: Implementácia prekladača pre jazyk IFJ20
 *
 * @brief Implementácia nafukovacieho stringu.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */

#include "dynstring.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PRE_STRING_ALLOC_SIZE 10 // kolko si má string predalokovať


Nstring *nstring_init(){
	Nstring *s;
	s=(Nstring *)malloc(sizeof(Nstring));
	if(s==NULL){
		//fprintf(stderr,"Nepodarilo sa allokovať štruktúru Nstring\n");
		return NULL;
	}
	s->string=(char *)malloc(PRE_STRING_ALLOC_SIZE*sizeof(char));
	if(s->string==NULL){
		free(s);
		//fprintf(stderr, "Nepodarilo sa allokovať string v štruktúre!\n");
		return NULL;
	}
	s->string_size=0;
	s->allocated_size=PRE_STRING_ALLOC_SIZE;
	nstring_clear(s);
	return s;

}

void nstring_free(Nstring *s){
	if(s==NULL) return;
	free(s->string);
	free(s);
	s=NULL;
}

void nstring_clear(Nstring *s){
	s->string[0]='\0';
	s->string_size=0;
}

bool nstring_add_str(Nstring *s, char *str){
	if(s->allocated_size <= strlen(str) ){
		s->string=(char *)realloc(s->string,s->allocated_size+strlen(str)+1);
		if(s->string == NULL){
			//fprintf(stderr, "Nepodarilo sa spraviť realloc pri funkcí nstring_add_char\n");
			return false;
		}
		s->allocated_size+=strlen(str)+1;
	}
	strcpy(s->string+s->string_size,str);
	s->string_size+=strlen(str);
	return true;
}

bool nstring_add_char(Nstring *s, char c){

	if(s->allocated_size <= s->string_size + 1){
		s->string=(char *)realloc(s->string,s->allocated_size+PRE_STRING_ALLOC_SIZE);
		if(s->string == NULL){
			//fprintf(stderr, "Nepodarilo sa spraviť realloc pri funkcí nstring_add_char\n");
			return false;
		}
		s->allocated_size+=PRE_STRING_ALLOC_SIZE;

	}

	s->string[s->string_size]=c;
	s->string[s->string_size+1]='\0';
	s->string_size++;
	return true;
}

int nstring_str_cmp(Nstring *s, const char *str){
	return strcmp(s->string,str);
}



void nstring_print(Nstring *s) {
    printf("%s", s->string);
}

void nstring_char_remove(Nstring *s){
	if(s->string_size == 0){
		return;
	}
	s->string_size -= 1;
	s->string[s->string_size]='\0';
}

int nstring_cmp(Nstring *s, Nstring *d){
	if(s==NULL || d==NULL) return -1;
	return strcmp(s->string,d->string);
}

bool nstring_is_clear(Nstring *s){
	if(s==NULL) return false;
	if(strlen(s->string)>0) return false;
	else return true;
}

bool nstring_ret_cmp(Nstring *left,Nstring *right){
	if(left==NULL) return false;
	if(right==NULL) return false;

	if(strlen(left->string)!=strlen(right->string)) return false;
	for(int i=0;i<strlen(left->string);i++){
		if((left->string)[i]!=(right->string)[i]){
			if((left->string)[i] != 'n'){
				if((right->string)[i]!='n') return false;
			}
		}
	}
	return true;
}

bool nstring_cpy(Nstring *a, Nstring *b){
	if(a==NULL) return false;
	if(b==NULL) return false;

	nstring_clear(b);
	if(!nstring_add_str(b,a->string)) return false;
	return true;
}

int nstring_len(Nstring *s){
	return s->string_size;
}

int nstring_2int(Nstring *s){
	return atoi(s->string);
}


double nstring_3float(Nstring *s){
	return atof(s->string);
}