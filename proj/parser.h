

#ifndef _PARSER_H
#define _PARSER_H

#include "scaner.h"
#include "err.h"

#define VALUE_CHECK() if (ret_value != ERR_RIGHT) return ret_value;
#define GET_TOKEN() p_getnexttoken(); if ((token.type == T_UNKNOWN) || (token.type == T_ERR)) return ERR_LEXSCAN; 
#define PEEK_TOKEN() peek_nexttoken(); if ((tokenp.type == T_UNKNOWN) || (tokenp.type == T_ERR)) return ERR_LEXSCAN; 

void p_getnexttoken();
int p_prog();
int p_funclist();
int p_func();
int p_paramlist();
int p_paramnext();
int p_datatypelist();
int p_datatypenext();
int p_datatype();
int p_statnext();
int p_statlist();
int p_stat();
int p_defstat();
int p_assignstat();
int p_idlist();
int p_idnext();
int p_expressionlist();
int p_expressionnext();
int p_termlist();
int p_termnext();
int p_term();
int p_idstat();
int p_exprorid();
int p_opteol();

#endif
