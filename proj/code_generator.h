


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
#endif