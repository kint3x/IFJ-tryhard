


#ifndef _CODE_GENERATOR
#define _CODE_GENERATOR


#include "dynstring.h"
#include <string.h>
#include "symtable.h"

bool generate_start();
bool G_Fun_header(Nstring *name);
bool G_Fun_argument(Nstring *s,int poradie,int scope);
bool G_Fun_def_return(int poradie);
bool G_Fun_ret_value(int poradie);
bool G_Fun_return();
bool G_PRINT();
bool G_end();
bool G_expr_term(Token token,int uniq);
bool G_declare_var(Nstring *s,int uniq);
bool G_expr_pops(Nstring *s);
bool G_expr_operat(char c,tType type);
bool G_expr_string_concat();
bool G_callfunc_arg(Nstring *s, int poradie, tType type);
bool G_createframe();
bool G_callfunc(Nstring *s);
bool G_aftercall_empty_write(Nstring *zasobnik);
bool G_Fun_pop_to_ret(int couter);
bool G_fun_print(Nstring *s, tType type,int uniq);
bool G_if_operat(char c,tType type);
bool G_if_label(int l);
bool G_if_label_cond(int l);
bool G_end_else(int l);
bool G_label_for_for(int uniq);
bool G_for_label_cond(int uniq);
bool G_for_ass_start(int uniq);
bool G_for_ass_end(int uniq);
bool G_for_end(int uniq);
bool G_defstat_var(Nstring *s,int uniq);
bool G_buildfun();
void G_FREEALL();
#endif