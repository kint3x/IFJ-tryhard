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
#include "scaner.h"
#include <string.h>

typedef enum var_type{
	V_UNDEF,
	V_INT,
	V_FLOAT,
	V_STRING,
	V_BOOL
}Variabile;


typedef struct BTree {
	int key;

	tType item_type; //Pre keyword
	Variabile var_type;
	Nstring *name;
	unsigned short int AoR;
	unsigned short int num_arguments;
	unsigned short int num_returns;
	Nstring *args;
	Nstring *returns;

	struct BTree *LPtr;
	struct BTree *RPtr;
}*BTreePtr;

typedef struct tree_stack{

	BTreePtr root;
	struct tree_stack *next;
}*BTreeStackPtr;

void BTree_init(BTreePtr *root);

int BTree_getkey(Nstring *name);

void BTree_dispose(BTreePtr *root);

void BTree_freeleaf(BTreePtr ptr);

void BTree_print(BTreePtr *root);

int BTree_newnode(BTreePtr *root, tType item, Nstring *n,BTreePtr *setptr);

BTreePtr BTree_findbyname(BTreePtr *root,Nstring *n);

void BTree_insertAoR(BTreePtr node,tType type);


void BTStack_init(BTreeStackPtr *root);

int BTStack_top(BTreeStackPtr *root,BTreePtr *change);

void BTStack_pop(BTreeStackPtr *root,BTreePtr *change);

void BTStack_dispose(BTreeStackPtr *root);

BTreePtr BTStack_searchbyname(BTreeStackPtr *root,Nstring *s);

#endif