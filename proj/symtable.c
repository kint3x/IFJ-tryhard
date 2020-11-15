/**
 * Projekt: Implementácia tabulky symbolov pre jazyk IFJ20
 *
 * @brief Implementácia tabulky symbolov pomocou stacku s binárnym stromom.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */
#include "symtable.h"

BT_symitemptr BTree_init(){
	BT_symitemptr new = (BT_symitemptr)malloc(sizeof(struct BT_symitem));
	if(new==NULL) return NULL;
	new->name=nstring_init();
	if(new->name==NULL) return NULL;

	new -> item_type = NONE;
	new -> var_type = UNDEF;
	new -> scope = -1;
	new -> num_arguments = 0;
	new -> num_returns = 0;
	new -> LPtr = NULL;
	new -> RPtr = NULL;
	return new;
}

void BTree_dispose(BT_symitemptr root){
	if(root!=NULL){
		BTree_dispose(root->LPtr);
		BTree_dispose(root->RPtr);
		BTree_freeleaf(root);
	}
}

void BTree_freeleaf(BT_symitemptr ptr){
	nstring_free(ptr->name);
	free(ptr);
}

void BTree_print(BT_symitemptr ptr){
	if(ptr!=NULL){
		BTree_print(ptr->LPtr);
		BTree_print(ptr->RPtr);
		printf("<<BTREE: it_type: %d, var_type: %d, name: %s, scope: %d>>\n",ptr->item_type,ptr->var_type,ptr->name->string, ptr->scope);
	}
}

BT_symitemptr BTree_additem(BT_symitemptr root,Titem_type item_type, char *name){
	BT_symitemptr *ptr=(&root);
	int newkey = BTree_getkeyvalue(name);
	while((*ptr)!=NULL){
		if((*ptr)->key>newkey){
			(*ptr)=(*ptr)->LPtr;
		}
		else (*ptr) = (*ptr)->RPtr;
	}
	(*ptr)=BTree_init();
	(*ptr)->key=newkey;
	(*ptr)->item_type=item_type;
	nstring_add_str((*ptr)->name,name);
	return (*ptr);
}

int BTree_getkeyvalue(char *string){
	int i=0,val=0;
	while(string[i++]!='\0'){
		val +=string[i];
	}
	return val;
}

//BT_STACK FUNCTIONS

Tree_stack * BTstack_init(){
	Tree_stack *ptr;
	ptr = (Tree_stack *) malloc(sizeof(Tree_stack));
	if (ptr == NULL) return NULL;
	ptr -> root = NULL;
	ptr->prev = NULL;
	ptr->next=NULL;
	return ptr;
}
Tree_stack *BTstack_top(Tree_stack *root){
	Tree_stack **ptr=(&root);
	Tree_stack *prev=NULL;

	while((*ptr)!=NULL){
		prev = (*ptr);
		(*ptr)=(*ptr)->next;
	}
	(*ptr)=BTstack_init();
	if((*ptr)==NULL) return NULL;
	(*ptr)->prev = prev;
	return (*ptr);
}
Tree_stack *BTstack_pop(Tree_stack *root){
	Tree_stack *ret=NULL;
	if(root==NULL){
		fprintf(stderr, "Tree_stack pop zly pop prazdneho stacku\n" );
		return NULL;
	}
	while(root->next!=NULL){
		ret=root->prev;
		root=root->next;
	}
	
	if(ret!=NULL){
		ret->prev->next=NULL;
	}

	BTree_dispose(root->root);
	free(root);
	return ret;
}