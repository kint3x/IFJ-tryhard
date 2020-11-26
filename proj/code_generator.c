
#include "code_generator.h"

//#TODO generovanie unikatnych nazvov premennych
//#TODO stack s pointerami na premenne



//funkcie začínajú s $
//premenne pre generator zacinaju &
//uzivatelske zacinaju s %
//definicia predprogramovaných funkcii
#define FUN_INT2FLOAT\
"\n LABEL $int2float"\
"\n PUSHFRAME"\
"\n DEFVAR LF@&ret1"\
"\n INT2FLOAT LF@&ret1 LF@&arg1"\
"\n POPFRAME"\
"\n RETURN"\


#define FUN_FLOAT2INT\
"\nLABEL $float2int"\
"\nPUSHFRAME"\
"\nDEFVAR LF@&ret1"\
"\nFLOAT2INT LF@&ret1 LF@&arg1"\
"\nPOPFRAME"\
"\nRETURN"\


#define FUN_LEN\
"\n#Start oflen "\
"\n LABEL $len " \
"\n PUSHFRAME " \
"\n DEFVAR LF@&ret1 " \
"\n STRLEN LF@&ret1 LF@&arg1 " \
"\n POPFRAME " \
"\n RETURN"\


#define FUN_ORD\
"\nLABEL $ord"\
"\nPUSHFRAME" \
"\nDEFVAR LF@&ret1"\
"\nMOVE LF@&ret1 int@"\
"\nDEFVAR LF@&ret2"\
"\nMOVE LF@&ret2 int@"\
"\nCREATEFRAME"\
"\nDEFVAR TF@&arg1"\
"\nMOVE TF@&arg1 LF@&arg1"\
"\nCALL $len"\
"\n# TF@&ret1 -DLŽKA retazca"\
"\nDEFVAR LF@&length"\
"\nMOVE LF@&length TF@&ret1"\
"\nSUB LF@&length LF@&length int@1"\
"\nDEFVAR LF@&cond_ret"\
"\nLT LF@&cond_ret  LF@&arg2 int@0"\
"\nJUMPIFEQ $ord$wrong LF@&cond_ret bool@true"\
"\nGT LF@&cond_ret  LF@&arg2 LF@&length"\
"\nJUMPIFEQ $ord$wrong LF@&cond_ret bool@true"\
"\nWRITE LF@&arg1"\
"\nWRITE LF@&arg2"\
"\nSTRI2INT LF@&ret1 LF@&arg1 LF@&arg2"\
"\nJUMP $ord$end"\
"\nLABEL $ord$wrong"\
"\nMOVE LF@&ret2 int@1"\
"\nLABEL $ord$end"\
"\nPOPFRAME"\
"\nRETURN"\



#define FUN_SUBSTR\
"\nLABEL  $substr"\
"\nPUSHFRAME "\
"\nDEFVAR LF@&ret1"\
"\nMOVE LF@&ret1 String@"\
"\nCREATEFRAME"\
"\nDEFVAR TF@&arg1"\
"\nMOVE TF@&arg1 LF@&arg1"\
"\nCALL $len"\
"\n# TF@&ret1 -DLŽKA retazca"\
"\nDEFVAR LF@&length"\
"\nMOVE LF@&length TF@&ret1"\
"\nDEFVAR LF@&cond_ret"\
"\nLT LF@&cond_ret  LF@&length int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nEQ LF@&cond_ret  LF@&length int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nLT LF@&cond_ret  LF@&arg2 int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nEQ LF@&cond_ret  LF@&arg2 int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nGT LF@&cond_ret  LF@&arg2 LF@&length"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nEQ LF@&cond_ret  LF@&arg3 int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nDEFVAR LF@&n"\
"\nMOVE LF@&n LF@&length"\
"\nSUB  LF@&n LF@&n LF@&arg2"\
"\nADD LF@&n LF@&n int@1"\
"\nDEFVAR LF@&nevcondition"\
"\nLT LF@&nevcondition LF@&arg3 int@0"\
"\nJUMPIFEQ $substr$changen LF@&nevcondition bool@true"\
"\nGT LF@&nevcondition LF@&arg3 LF@&n"\
"\nJUMPIFEQ $substr$changen LF@&nevcondition bool@true"\
"\nJUMP $substr$cut"\
"\nLABEL $substr$changen"\
"\nMOVE LF@&arg3 LF@&n"\
"\nLABEL $substr$cut"\
"\n DEFVAR LF@&i"\
"\nMOVE LF@&i LF@&arg2"\
"\nSUB LF@&i LF@&i int@1"\
"\nDEFVAR LF@&outchar"\
"\nDEFVAR LF@&conloop"\
"\nLABEL $substr$startloop"\
"\nGETCHAR LF@&outchar LF@&arg1 LF@&i"\
"\nADD LF@&i LF@&i int@1"\
"\nCONCAT LF@&ret1 LF@&ret1 LF@&outchar"\
"\nSUB LF@&arg3 LF@&arg3 int@1"\
"\nGT LF@&conloop LF@&arg3 int@0"\
"\nJUMPIFEQ $substr$startloop LF@&conloop bool@true"\
"\nLABEL $substr$end "\
"\nPOPFRAME "\
"\nRETURN"\

#define FUN_CHR\
"\nLABEL $chr"\
"\nPUSHFRAME"\
"\nDEFVAR LF@&ret1"\
"\nMOVE LF@&ret1 string@"\
"\nDEFVAR  LF@&condition"\
"\nLT LF@&condition LF@&arg1 int@0"\
"\nJUMPIFEQ $chr$end LF@&condition bool@true"\
"\nGT LF@&condition LF@&arg1 int@255"\
"\nINT2CHAR LF@&ret1 LF@&arg1"\
"\nJUMPIFEQ $chr$end LF@&condition bool@true"\
"\nLABEL $chr$end"\
"\nPOPFRAME"\
"\nRETURN"\





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
