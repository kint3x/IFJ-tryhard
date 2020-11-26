
#include "code_generator.h"

//#TODO generovanie unikatnych nazvov premennych
//#TODO stack s pointerami na premenne



//funkcie začínajú s $
//premenne pre generator zacinaju &
//uzivatelske zacinaju s %

#define GEN_HEADER \
"\n#Start of program"\
"\n.ifjcode20"\
"\nCREATEFRAME"\
"\nCALL $main" \
"\nJUMP end"

#define FUNCTION_INPUTI \
"\n#FUNCTION INPUTI" \


#define GEN_END "\nLABEL END"

Nstring generated_code;

#define ADD_CODE(_code) if(!nstring_add_str(&generated_code,(_code))) return false;


bool generate_start(){
	ADD_CODE(GEN_HEADER);

	return true;
}

bool G_Fun_header(Nstring *name){
	ADD_CODE("\n#FUNCTION "); ADD_CODE("$");ADD_CODE(name->string);
	ADD_CODE("\nPUSHFRAME");
	ADD_CODE("\nDEFVAR LF@&bin")
	return true;
}
bool G_Fun_argument(Nstring *s,int poradie){
	char buf[30];
	sprintf(buf,"%d",poradie);
	ADD_CODE("\nDEFVAR ");ADD_CODE("%");ADD_CODE(s->string);ADD_CODE(buf);
	ADD_CODE("\nMOVE LF@&arg");ADD_CODE(buf);
	return true;
}
bool G_Fun_def_return(int poradie){
	char buf[30];
	sprintf(buf,"%d",poradie);
	ADD_CODE("\nDEFVAR LF@&ret");ADD_CODE(buf);
	return true;
}

bool G_Fun_ret_value(int poradie){
	char buf[30];
	sprintf(buf,"%d",poradie);
	ADD_CODE("\nDEFVAR LF@&ret")ADD_CODE(buf);
	return true;
}

bool G_Fun_return(){
	ADD_CODE("\nPOPFRAME");
	ADD_CODE("\nRETURN");
	return true;
}
void G_PRINT(){
	printf("%s\n",generated_code.string);
	free(generated_code.string);
}