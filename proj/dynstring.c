


#include "dynstring.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PRE_STRING_ALLOC_SIZE 10 // kolko si má string predalokovať


Nstring *nstring_init(){
	Nstring *s;
	s=(Nstring *)malloc(sizeof(Nstring));
	if(s==NULL){
		fprintf(stderr,"Nepodarilo sa allokovať štruktúru Nstring\n");
		return NULL;
	}
	s->string=(char *)malloc(PRE_STRING_ALLOC_SIZE*sizeof(char));
	if(s->string==NULL){
		free(s);
		fprintf(stderr, "Nepodarilo sa allokovať string v štruktúre!\n");
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

bool nstring_add_char(Nstring *s, char c){

	if(s->allocated_size <= s->string_size + 1){
		s->string=(char *)realloc(s->string,s->allocated_size+PRE_STRING_ALLOC_SIZE);
		if(s->string == NULL){
			fprintf(stderr, "Nepodarilo sa spraviť realloc pri funkcí nstring_add_char\n");
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

    nstring_add_char(s, '\0');
    printf("%s", s->string);
}

void nstring_char_remove(Nstring *s){
	if(s->string_size == 0){
		return;
	}
	s->string_size -= 1;
	s->string[s->string_size]='\0';
}
