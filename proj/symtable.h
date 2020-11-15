/**
 * Projekt: Implementácia tabulky symbolov pre jazyk IFJ20
 *
 * @brief Implementácia tabulky symbolov pomocou stacku s binárnym stromom.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include "dynstring.h"

typedef enum TiTem
{
	NONE,
	VARIABILE,
	FUNCTION
	
}Titem_type;

typedef enum var_type{
	UNDEF,
	INT,
	FLOAT,
	STRING,
	BOOL
}Var_type;

typedef struct BT_symitem {
	int key;

	Titem_type item_type;
	Var_type var_type;
	Nstring *name;
	unsigned short int scope;
	unsigned short int num_arguments;
	unsigned short int num_returns;

	struct BT_symitem *LPtr;
	struct BT_symitem *RPtr;
}*BT_symitemptr;

typedef struct tree_stack{

	BT_symitemptr root;
	struct tree_stack *prev;
	struct tree_stack *next;
}Tree_stack;

// BT funkcie
BT_symitemptr BTree_init();
void BTree_set(BT_symitemptr ptr, Titem_type item_type, Var_type var_type, 
	Nstring *name , unsigned short int scope, unsigned short int num_arguments,
	unsigned short int num_returns);
void BTree_removebyname(Nstring *name);
void BTree_dispose();
void BTree_freeleaf();
void BTree_print();
int BTree_getkeyvalue(char *name);
BT_symitemptr BTree_additem(BT_symitemptr root,Titem_type item_type, char *name);
// BT_Stack funkcie

Tree_stack *BTstack_init();
Tree_stack *BTstack_top();
Tree_stack *BTstack_pop();
#endif