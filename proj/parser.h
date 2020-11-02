

#ifndef _PARSER_H
#define _PARSER_H

#include "scaner.h"
#include "err.h"

void p_prog();
void p_funclist();
void p_func();
void p_paramlist();
void p_paramnext();
void p_datatypelist();
void p_datatypenext();
void p_datatype();
void p_statnext();
void p_statlist();
void p_stat();
void p_defstat();
void p_assignstat();
void p_idlist();
void p_idnext();
void p_expressionlist();
void p_expressionnext();
void p_termlist();
void p_termnext();
void p_term();
void p_idstat();
void p_exprorid();
void p_opteol();

#endif