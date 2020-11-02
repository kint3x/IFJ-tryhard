

#ifndef _PARSER_H
#define _PARSER_H

#include "scaner.h"
#include "err.h"

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