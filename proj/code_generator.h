


#ifndef _CODE_GENERATOR
#define _CODE_GENERATOR


#include "dynstring.h"
#include <string.h>
#include "symtable.h"

bool generate_start();
bool G_Fun_header(Nstring *name);
bool G_Fun_argument(Nstring *s,int poradie);
bool G_Fun_def_return(int poradie);
bool G_Fun_ret_value(int poradie);
bool G_Fun_return();
void G_PRINT();
bool G_end();
bool G_expr_term(Token token,int uniq);
bool G_declare_var(Nstring *s,int uniq);
bool G_expr_pops(Nstring *s);
bool G_expr_operat(char c);
bool G_expr_string_concat();
#endif