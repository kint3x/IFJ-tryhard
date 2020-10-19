


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
		fprintf(stderr, "Nepodarilo sa allokovať string v štruktúre!\n");
		return false;
	}
	s->string_size=0;
	s->allocated_size=PRE_STRING_ALLOC_SIZE;
	nstring_clear(s);
	return true;

}

void nstring_free(Nstring *s){
	free(s->string);

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
