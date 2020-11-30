/**
 * Projekt: Implementace překladače imperativního jazyka IFJ20.
 *
 * @brief Generácia kódu.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 * @author Martin Matějka <xmatej55@vutbr.cz>
 * @author Ľubomír Závodský <xzavod14@vutbr.cz>
 */

#include "code_generator.h"



//funkcie začínajú s $
//premenne pre generator zacinaju &
//uzivatelske zacinaju s %
//definicia predprogramovaných funkcii
#define FUN_INT2FLOAT \
"\n LABEL $int2float"\
"\n PUSHFRAME"\
"\n DEFVAR LF@&ret1"\
"\n INT2FLOAT LF@&ret1 LF@&arg1"\
"\n POPFRAME"\
"\n RETURN"\


#define FUN_FLOAT2INT \
"\nLABEL $float2int"\
"\nPUSHFRAME"\
"\nDEFVAR LF@&ret1"\
"\nFLOAT2INT LF@&ret1 LF@&arg1"\
"\nPOPFRAME"\
"\nRETURN"\


#define FUN_LEN \
"\n#Start oflen "\
"\n LABEL $len " \
"\n PUSHFRAME " \
"\n DEFVAR LF@&ret1 " \
"\n STRLEN LF@&ret1 LF@&arg1 " \
"\n POPFRAME " \
"\n RETURN"\


#define FUN_ORD \
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
"\n#WRITE LF@&arg1" /*ZAKOMENTOVANE*/ \
"\n#WRITE LF@&arg2" /*ZAKOMENTOVANE*/ \
"\nSTRI2INT LF@&ret1 LF@&arg1 LF@&arg2" \
"\nMOVE LF@&ret2 int@0" /*DOPLNENE*/  \
"\nJUMP $ord$end"\
"\nLABEL $ord$wrong"\
"\nMOVE LF@&ret2 int@1"\
"\nLABEL $ord$end"\
"\nPOPFRAME"\
"\nRETURN"



#define FUN_SUBSTR \
"\n#START OF FUNCTION SUBSTR"\
"\nLABEL  $substr" 	\
"\nPUSHFRAME"  \
"\nDEFVAR LF@&ret1"\
"\nDEFVAR LF@&ret2" \
"\nDEFVAR LF@&len"\
"\nDEFVAR LF@&bool"\
"\nDEFVAR LF@&str"\
"\nDEFVAR LF@&counter"\
"\nMOVE LF@&counter int@0"\
"\nMOVE LF@&str string@"\
"\nMOVE LF@&ret1 string@"\
"\nCREATEFRAME"\
"\nDEFVAR TF@&arg1"\
"\nMOVE TF@&arg1 LF@&arg1"\
"\nCALL $len"\
"\nMOVE LF@&len TF@&ret1"\
"\nLT LF@&bool LF@&arg2 int@0" \
"\nJUMPIFEQ $substr$err LF@&bool bool@true"\
"\nGT LF@&bool LF@&arg2 LF@&len" \
"\nJUMPIFEQ $substr$err LF@&bool bool@true"\
"\nLT LF@&bool LF@&arg3 int@0" \
"\nJUMPIFEQ $substr$err LF@&bool bool@true"\
"\nLABEL &sub_while"  \
"\nLT LF@&bool LF@&counter LF@&arg3" \
"\nJUMPIFEQ &sub_while_e LF@&bool bool@false" \
"\nLT LF@&bool LF@&arg2 LF@&len"  \
"\nJUMPIFEQ &sub_while_e LF@&bool bool@false" \
"\nGETCHAR LF@&str LF@&arg1 LF@&arg2"\
"\nCONCAT LF@&ret1 LF@&ret1 LF@&str"\
"\nADD LF@&arg2 LF@&arg2 int@1"\
"\nADD LF@&counter LF@&counter int@1"\
"\nJUMP &sub_while"\
"\nLABEL &sub_while_e"\
"\nJUMP $substr$right"\
"\nLABEL $substr$err"\
"\nMOVE LF@&ret2 int@1"\
"\nJUMP $substr$end"\
"\nLABEL $substr$right"\
"\nMOVE LF@&ret2 int@0"\
"\nLABEL $substr$end"\
"\nPOPFRAME"\
"\nRETURN"\

#define FUN_CHR \
"\nLABEL $chr"\
"\nPUSHFRAME"\
"\nDEFVAR LF@&ret1"\
"\nDEFVAR LF@&ret2"/*DOPLNENE*/\
"\nMOVE LF@&ret1 string@"\
"\nDEFVAR  LF@&condition"\
"\nLT LF@&condition LF@&arg1 int@0"\
"\nJUMPIFEQ $chr$err LF@&condition bool@true"\
"\nGT LF@&condition LF@&arg1 int@255"\
"\nJUMPIFEQ $chr$err LF@&condition bool@true"\
"\nINT2CHAR LF@&ret1 LF@&arg1"\
"\nMOVE LF@&ret2 int@0"/*DOPLNENE*/\
"\nJUMP $chr$end"/*DOPLNENE*/\
"\nLABEL $chr$err"/*DOPLNENE*/\
"\nMOVE LF@&ret2 int@1" /*DOPLNENE*/\
"\nLABEL $chr$end"\
"\nPOPFRAME"\
"\nRETURN" \

#define  FUN_INPUTI \
"\nLABEL $inputi"  \
"\nPUSHFRAME"  \
"\nDEFVAR LF@&ret1"  \
"\nDEFVAR LF@&ret2"  \
"\nDEFVAR LF@&char"  \
"\nREAD LF@&char int"  \
"\nDEFVAR LF@&cond_return"  \
"\nEQ LF@&cond_return LF@&char nil@nil"  \
"\nJUMPIFEQ $inputi$end LF@&cond_return bool@false"  \
"\nLABEL $inputi$err"  \
"\nMOVE LF@&ret1 nil@nil"  \
"\nMOVE LF@&ret2 int@1"  \
"\nPOPFRAME"  \
"\nRETURN"  \
"\nLABEL $inputi$end"  \
"\nMOVE LF@&ret1 LF@&char"  \
"\nMOVE LF@&ret2 int@0"  \
"\nPOPFRAME"  \
"\nRETURN"  \


#define  FUN_INPUTF \
"\nLABEL $inputf"  \
"\nPUSHFRAME"  \
"\nDEFVAR LF@&ret1"  \
"\nDEFVAR LF@&ret2"  \
"\nDEFVAR LF@&char"  \
"\nREAD LF@&char float"  \
"\nDEFVAR LF@&cond_return"  \
"\nEQ LF@&cond_return LF@&char nil@nil"  \
"\nJUMPIFEQ $inputf$end LF@&cond_return bool@false"  \
"\nLABEL $inputf$err"  \
"\nMOVE LF@&ret1 nil@nil"  \
"\nMOVE LF@&ret2 int@1"  \
"\nPOPFRAME"  \
"\nRETURN"  \
"\nLABEL $inputf$end"  \
"\nMOVE LF@&ret1 LF@&char"  \
"\nMOVE LF@&ret2 int@0"  \
"\nPOPFRAME"  \
"\nRETURN"   

#define  FUN_INPUTS \
"\nLABEL $inputs"  \
"\nPUSHFRAME"  \
"\nDEFVAR LF@&ret1"  \
"\nDEFVAR LF@&ret2"  \
"\nDEFVAR LF@&char"  \
"\nREAD LF@&char string"  \
"\nDEFVAR LF@&cond_return"  \
"\nEQ LF@&cond_return LF@&char nil@nil"  \
"\nJUMPIFEQ $inputs$end LF@&cond_return bool@false"  \
"\nLABEL $inputs$err"  \
"\nMOVE LF@&ret1 nil@nil"  \
"\nMOVE LF@&ret2 int@1"  \
"\nPOPFRAME"  \
"\nRETURN"  \
"\nLABEL $inputs$end"  \
"\nMOVE LF@&ret1 LF@&char"  \
"\nMOVE LF@&ret2 int@0"  \
"\nPOPFRAME"  \
"\nRETURN"  




#define GEN_HEADER \
"\n#Start of program"\
"\n.ifjcode20"\
"\nCREATEFRAME"\
"\nCALL $main" \
"\nJUMP end"

#define FUNCTION_INPUTI \
"\n#FUNCTION INPUTI" \


#define GEN_END "\nLABEL end"

Nstring generated_code;

Nstring pre_function;
Nstring def_vars;
Nstring post_function;


#define ADD_CODE(_code) if(!nstring_add_str(&post_function,(_code))) return false;
#define ADD_HEADER_CODE(_code) if(!nstring_add_str(&pre_function,(_code))) return false;
#define ADD_VARS_CODE(_code) if(!nstring_add_str(&def_vars,(_code))) return false;
#define ADD_GLOBAL_CODE(_code) if(!nstring_add_str(&generated_code,(_code))) return false;

bool generate_start(){
	Nstring_onlyinit(&generated_code);
	Nstring_onlyinit(&pre_function);
	Nstring_onlyinit(&def_vars);
	Nstring_onlyinit(&post_function);
	ADD_GLOBAL_CODE(GEN_HEADER);
	ADD_GLOBAL_CODE("\n#BUILTIN FUNCTIONS ---------------");
	ADD_GLOBAL_CODE(FUN_INT2FLOAT);
	ADD_GLOBAL_CODE(FUN_FLOAT2INT);
	ADD_GLOBAL_CODE(FUN_LEN);
	ADD_GLOBAL_CODE(FUN_ORD);
	ADD_GLOBAL_CODE(FUN_SUBSTR);
	ADD_GLOBAL_CODE(FUN_CHR);
	ADD_GLOBAL_CODE(FUN_INPUTI);
	ADD_GLOBAL_CODE(FUN_INPUTF);
	ADD_GLOBAL_CODE(FUN_INPUTS);

	ADD_GLOBAL_CODE("\n#---------------------------")
	return true;
}

bool G_Fun_header(Nstring *name){
	ADD_HEADER_CODE("\n#FUNCTION "); ADD_HEADER_CODE("$");ADD_HEADER_CODE(name->string);
	ADD_HEADER_CODE("\nLABEL "); ADD_HEADER_CODE("$");ADD_HEADER_CODE(name->string);
	ADD_HEADER_CODE("\nPUSHFRAME");
	ADD_HEADER_CODE("\nDEFVAR LF@&bin");
	ADD_HEADER_CODE("\nDEFVAR LF@&concat1");
	ADD_HEADER_CODE("\nDEFVAR LF@&concat2");
	ADD_HEADER_CODE("\nDEFVAR LF@&concat3");
	ADD_HEADER_CODE("\nDEFVAR LF@&float1");
	ADD_HEADER_CODE("\nDEFVAR LF@&float2");
	ADD_HEADER_CODE("\nDEFVAR LF@&int1");
	ADD_HEADER_CODE("\nDEFVAR LF@&int2");
	ADD_HEADER_CODE("\nDEFVAR LF@&bool");
	return true;
}
bool G_Fun_argument(Nstring *s,int poradie,int scope){
	char buf[30];
	sprintf(buf,"%d",scope);
	ADD_VARS_CODE("\nDEFVAR LF@");ADD_VARS_CODE("%");ADD_VARS_CODE(s->string);ADD_VARS_CODE(buf);
	sprintf(buf,"%d",scope);
	ADD_CODE("\nMOVE LF@%");ADD_CODE(s->string);ADD_CODE(buf);
	ADD_CODE(" LF@&arg")
	sprintf(buf,"%d",poradie);
	ADD_CODE(buf);
	return true;
}

bool G_Fun_ret_value(int poradie){
	char buf[30];
	sprintf(buf,"%d",poradie);
	ADD_VARS_CODE("\nDEFVAR LF@&ret");ADD_VARS_CODE(buf);
	return true;
}
bool G_Fun_pop_to_ret(int counter){
	char buf[30];
	sprintf(buf,"%d",counter);
	ADD_CODE("\nPOPS LF@&ret");ADD_CODE(buf);
	return true;
}

bool G_Fun_return(){
	ADD_CODE("\nPOPFRAME");
	ADD_CODE("\nRETURN");
	return true;
}
bool G_PRINT(){
	ADD_GLOBAL_CODE("\nJUMP end");
	ADD_GLOBAL_CODE("\nLABEL exit_zerodiv");
	ADD_GLOBAL_CODE("\nEXIT int@9");
	ADD_GLOBAL_CODE(GEN_END);
	printf("%s\n",generated_code.string);
	G_FREEALL();
	 return true;
}
bool G_end(){
	ADD_CODE("\nLABEL end");
	return true;
}
bool G_expr_term(Token token,int uniq){
	ADD_CODE("\nPUSHS ");
	char buf[40];
	if(token.type==T_ID){
		sprintf(buf,"%s%d",token.data->string,uniq);
		ADD_CODE("LF@%");ADD_CODE(buf);
	}
	if(token.type==T_INT){
		ADD_CODE("int@");ADD_CODE(token.data->string);
	}
	if(token.type==T_DOUBLE){
		double val=nstring_3float(token.data);
		sprintf(buf,"%a",val);
		ADD_CODE("float@");ADD_CODE(buf);
	}
	if(token.type==T_STRING){
		nstring_string_to_escape(token.data);
		ADD_CODE("string@");ADD_CODE(token.data->string);
	}
	return true;
}

bool G_declare_var(Nstring *s,int uniq){
	char buf[30];
	sprintf(buf,"%d",uniq);
	ADD_VARS_CODE("\nDEFVAR LF@%");
	ADD_VARS_CODE(s->string);ADD_VARS_CODE(buf);
	return true;
}

bool G_expr_pops(Nstring *s){
	if(nstring_str_cmp(s,"_")==0){
		ADD_CODE("\nCLEARS");
	}
	else{
		ADD_CODE("\nPOPS LF@%");ADD_CODE(s->string);
	}
	return true;
}

bool G_expr_operat(char c,tType type){
	if(c=='+'){
		ADD_CODE("\nADDS");
	}
	else if(c=='-'){
		ADD_CODE("\nSUBS");
	}
	else if(c=='/'){
		if(type==T_INT) {
			ADD_CODE("\nPOPS LF@&int1");
			ADD_CODE("\nJUMPIFEQ exit_zerodiv LF@&int1 int@0")
			ADD_CODE("\nPUSHS LF@&int1");
			ADD_CODE("\nIDIVS");
		}
		else {
			ADD_CODE("\nPOPS LF@&float1");
			ADD_CODE("\nJUMPIFEQ exit_zerodiv LF@&float1 float@0x0p+0");
			ADD_CODE("\nPUSHS LF@&float1");
			ADD_CODE("\nDIVS");
		}

	}
	else if(c=='*'){
		ADD_CODE("\nMULS");
	}
	return true;
}

bool G_expr_string_concat(){
	ADD_CODE("\nPOPS LF@&concat2");
	ADD_CODE("\nPOPS LF@&concat1");
	ADD_CODE("\nCONCAT LF@&concat3 LF@&concat1 LF@&concat2");
	ADD_CODE("\nPUSHS LF@&concat3");
	return true;
}

bool G_createframe(){
	ADD_CODE("\nCREATEFRAME");
	return true;
}
bool G_callfunc(Nstring *s){
	ADD_CODE("\nCALL $");ADD_CODE(s->string);
	return true;
}

bool G_callfunc_arg(Nstring *s, int poradie, tType type){
	char buf[30];
	char buf2[30];
	sprintf(buf,"%d",poradie);

	if(type==T_ID){
		ADD_CODE("\nDEFVAR TF@&arg");ADD_CODE(buf);
		ADD_CODE("\nMOVE TF@&arg");ADD_CODE(buf);ADD_CODE(" LF@%");ADD_CODE(s->string);
	}
	if(type==T_INT){
		ADD_CODE("\nDEFVAR TF@&arg");ADD_CODE(buf);
		ADD_CODE("\nMOVE TF@&arg");ADD_CODE(buf);ADD_CODE(" int@");ADD_CODE(s->string);
	}
	if(type==T_DOUBLE){
		double val=nstring_3float(s);
		sprintf(buf2,"%a",val);
		ADD_CODE("\nDEFVAR TF@&arg");ADD_CODE(buf);
		ADD_CODE("\nMOVE TF@&arg");ADD_CODE(buf);ADD_CODE(" float@");ADD_CODE(buf2);
	}
	if(type==T_STRING){
		nstring_string_to_escape(s);
		ADD_CODE("\nDEFVAR TF@&arg");ADD_CODE(buf);
		ADD_CODE("\nMOVE TF@&arg");ADD_CODE(buf);ADD_CODE(" string@");ADD_CODE(s->string);
	}
	return true;
}

bool G_aftercall_empty_write(Nstring *zasobnik){
	char buf[30];
	unsigned int dlzka=(unsigned int)nstring_len(zasobnik);
	int i=1;
	Nstring *tmp = nstring_init();

	while(zasobnik->string[dlzka-1]!='%'){
		sprintf(buf,"%d",i);
		nstring_get_and_delete(zasobnik,tmp);
		if(nstring_str_cmp(tmp,"_")!=0){
			ADD_CODE("\nMOVE LF@%");ADD_CODE(tmp->string);ADD_CODE(" TF@&ret");ADD_CODE(buf);
		}
		i++;
	}

	nstring_free(tmp);
	return true;
}

bool G_fun_print(Nstring *s, tType type,int uniq){
	char buf[30];
	double val=0;
	val = val*5; // <aby prekladac nemarhoval
	if(type==T_ID){
		sprintf(buf,"%s%d",s->string,uniq);
		ADD_CODE("\nWRITE LF@%");ADD_CODE(buf);
	}
	if(type==T_INT){
		ADD_CODE("\nWRITE int@");ADD_CODE(s->string);
	}
	if(type==T_DOUBLE){
		double val=nstring_3float(s);
		sprintf(buf,"%a",val);
		ADD_CODE("\nWRITE float@");ADD_CODE(buf);
	}
	if(type==T_STRING){
		nstring_string_to_escape(s);
		ADD_CODE("\nWRITE string@");ADD_CODE(s->string);
	}
	return true;
}

bool G_if_operat(char c,tType type){
	ADD_CODE("\n#GENERATE CONDITION");
	ADD_CODE("\n#------------------");
	switch(type){
		case T_INT:
			if(c=='<'){
				ADD_CODE("\nLTS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='>'){
				ADD_CODE("\nGTS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='M'){
				ADD_CODE("\nPOPS LF@&int2");
				ADD_CODE("\nPOPS LF@&int1");
				ADD_CODE("\nLT LF@&bool LF@&int1 LF@&int2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nEQ LF@&bool LF@&int1 LF@&int2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nORS");
				ADD_CODE("\nPOPS LF@&bool")
			}
			else if(c=='V'){
				ADD_CODE("\nPOPS LF@&int2");
				ADD_CODE("\nPOPS LF@&int1");
				ADD_CODE("\nGT LF@&bool LF@&int1 LF@&int2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nEQ LF@&bool LF@&int1 LF@&int2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nORS");
				ADD_CODE("\nPOPS LF@&bool")
			}
			else if(c=='='){
				ADD_CODE("\nEQS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='!'){
				ADD_CODE("\nEQS");
				ADD_CODE("\nNOTS")
				ADD_CODE("\nPOPS LF@&bool");
			}
			break;
		case T_STRING:
			if(c=='<'){
				ADD_CODE("\nLTS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='>'){
				ADD_CODE("\nGTS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='M'){
				ADD_CODE("\nPOPS LF@&concat2");
				ADD_CODE("\nPOPS LF@&concat1");
				ADD_CODE("\nLT LF@&bool LF@&concat1 LF@&concat2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nEQ LF@&bool LF@&concat1 LF@&concat2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nORS");
				ADD_CODE("\nPOPS LF@&bool")
			}
			else if(c=='V'){
				ADD_CODE("\nPOPS LF@&concat2");
				ADD_CODE("\nPOPS LF@&concat1");
				ADD_CODE("\nGT LF@&bool LF@&concat1 LF@&concat2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nEQ LF@&bool LF@&concat1 LF@&concat2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nORS");
				ADD_CODE("\nPOPS LF@&bool")
			}
			else if(c=='='){
				ADD_CODE("\nEQS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='!'){
				ADD_CODE("\nEQS");
				ADD_CODE("\nNOTS")
				ADD_CODE("\nPOPS LF@&bool");
			}
			break;
		case T_DOUBLE:
			if(c=='<'){
				ADD_CODE("\nLTS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='>'){
				ADD_CODE("\nGTS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='M'){
				ADD_CODE("\nPOPS LF@&float2");
				ADD_CODE("\nPOPS LF@&float1");
				ADD_CODE("\nLT LF@&bool LF@&float1 LF@&float2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nEQ LF@&bool LF@&float1 LF@&float2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nORS");
				ADD_CODE("\nPOPS LF@&bool")
			}
			else if(c=='V'){
				ADD_CODE("\nPOPS LF@&float2");
				ADD_CODE("\nPOPS LF@&float1");
				ADD_CODE("\nGT LF@&bool LF@&float1 LF@&float2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nEQ LF@&bool LF@&float1 LF@&float2");
				ADD_CODE("\nPUSHS LF@&bool");
				ADD_CODE("\nORS");
				ADD_CODE("\nPOPS LF@&bool")
			}
			else if(c=='='){
				ADD_CODE("\nEQS");
				ADD_CODE("\nPOPS LF@&bool");
			}
			else if(c=='!'){
				ADD_CODE("\nEQS");
				ADD_CODE("\nNOTS")
				ADD_CODE("\nPOPS LF@&bool");
			}
			break;
		default:
			return false;
			break;
	 }
	return true;
}

bool G_if_label(int l){
	char buf[30];
	sprintf(buf,"?end_else%d",l);
	ADD_CODE("\nJUMP ");ADD_CODE(buf);
	sprintf(buf,"?else%d",l);
	ADD_CODE("\nLABEL ");ADD_CODE(buf);
	return true;
}

bool G_if_label_cond(int l){
	char buf[30];
	sprintf(buf,"?else%d",l);
	ADD_CODE("\nJUMPIFNEQ ");ADD_CODE(buf);ADD_CODE(" LF@&bool bool@true")
	return true;
}

bool G_end_else(int l){
	char buf[30];
	sprintf(buf,"?end_else%d",l);
	ADD_CODE("\nLABEL ");ADD_CODE(buf);
	return true;
}

bool G_defstat_var(Nstring *s,int uniq){
	char buf[30];
	sprintf(buf,"%d",uniq);
	ADD_VARS_CODE("\nDEFVAR LF@%");
	ADD_VARS_CODE(s->string);ADD_VARS_CODE(buf);
	ADD_CODE("\nPOPS LF@%");ADD_CODE(s->string);ADD_CODE(buf);
	return true;
}

bool G_label_for_for(int uniq){
	char buf[30];
	sprintf(buf,"%d",uniq);
	ADD_CODE("\n#START OF FOR");
	ADD_CODE("\nLABEL !for_s");ADD_CODE(buf);
	return true;
}

bool G_for_label_cond(int uniq){
	char buf[30];
	sprintf(buf,"!for_e%d",uniq);
	ADD_CODE("\nJUMPIFNEQ ");ADD_CODE(buf);ADD_CODE(" LF@&bool bool@true");
	sprintf(buf,"%d",uniq);
	ADD_CODE("\nJUMP !for_ass_e");ADD_CODE(buf);
	return true;
}

bool G_for_ass_start(int uniq){
	char buf[30];
	sprintf(buf,"%d",uniq);
	ADD_CODE("\n#START OF ASS");
	ADD_CODE("\nLABEL !for_ass_s");ADD_CODE(buf);
	return true;
}

bool G_for_ass_end(int uniq){
	char buf[30];
	sprintf(buf,"%d",uniq);
	ADD_CODE("\nJUMP !for_s");ADD_CODE(buf);
	ADD_CODE("\n#END OF ASS");
	ADD_CODE("\nLABEL !for_ass_e");ADD_CODE(buf);
	return true;
}

bool G_for_end(int uniq){
	char buf[30];
	sprintf(buf,"%d",uniq);
	ADD_CODE("\nJUMP !for_ass_s");ADD_CODE(buf);
	ADD_CODE("\n#END OF FOR");
	ADD_CODE("\nLABEL !for_e");ADD_CODE(buf);
	return true;
}

bool G_buildfun(){
	ADD_GLOBAL_CODE(pre_function.string);
	ADD_GLOBAL_CODE(def_vars.string);
	ADD_GLOBAL_CODE(post_function.string);
	nstring_clear(&def_vars);
	nstring_clear(&pre_function);
	nstring_clear(&post_function);

	return true;
}

void G_FREEALL(){
	free(generated_code.string);
	generated_code.string=NULL;
	free(pre_function.string);
	pre_function.string=NULL;
	free(post_function.string);
	post_function.string=NULL;
	free(def_vars.string);
	def_vars.string=NULL;
}