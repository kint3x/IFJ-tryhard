/**
 * Projekt: Implementácia tabulky symbolov pre jazyk IFJ20
 *
 * @brief Implementácia tabulky symbolov pomocou stacku s binárnym stromom.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */
#include "symtable.h"

void BTree_init(BTreePtr *root){
	(*root)=NULL;
	return;
}

int BTree_getkey(Nstring *name){
	if(name==NULL) return -1;
	int count=0;
	for(int i=0;i<strlen(name->string);i++){
		count += name->string[i]; 
	}
	return count;
}

void BTree_dispose(BTreePtr *root){
	if((*root)!=NULL){
		BTree_dispose(&((*root)->LPtr));
		BTree_dispose(&((*root)->RPtr));
		BTree_freeleaf((*root));
	}
	(*root)=NULL;
}

void BTree_freeleaf(BTreePtr ptr){
	nstring_free(ptr->name);
	nstring_free(ptr->args);
	nstring_free(ptr->returns);
	free(ptr);
}

int BTree_newnode(BTreePtr *root, tType item, Nstring *n, BTreePtr *setptr){
	int newkey=BTree_getkey(n);
	if(newkey==-1) return false;

	while((*root)!=NULL){
		if(newkey<(*root)->key){
			root=&((*root)->LPtr);
		}
		else if(newkey>(*root)->key){
			root=&((*root)->RPtr);
		}
		else{
			if(nstring_cmp(n,(*root)->name)==0){
				if((*root)->item_type==item) return ERR_SEMAN_NOT_DEFINED;
				root=&((*root)->RPtr);
			}
			else{
				root=&((*root)->RPtr);
			}
		}
	}
	(*root)=(BTreePtr)malloc(sizeof(struct BTree));
	if((*root)==NULL) return ERR_INTERNAL;

	(*root)->name=nstring_init();
	if((*root)->name==NULL) return ERR_INTERNAL;

	if(nstring_add_str((*root)->name,n->string)==false) return ERR_INTERNAL;
	if(((*root)->args=nstring_init())==NULL) return ERR_INTERNAL;
	if(((*root)->returns=nstring_init())==NULL) return ERR_INTERNAL;
	(*root)->key=newkey;
	(*root)->item_type=item;
	(*root)->num_arguments=0;
 	(*root)->num_returns=0;
 	(*root)->LPtr=NULL;
 	(*root)->RPtr=NULL;

 	if(setptr!=NULL){
 		(*setptr)=(*root);
 	}
	return ERR_RIGHT;
}

void BTree_insertAoR(BTreePtr node,tType type){
	if(node==NULL) return;

	if(node->AoR==0){
		if(type == T_WINT)
		nstring_add_char(node->args,'i');
		if(type == T_WFLOAT64)
		nstring_add_char(node->args,'f');
		if(type == T_WSTRING)
		nstring_add_char(node->args,'s');
		/*if(type == T_WBOOL)
		nstring_add_char(node->args,'b');*/

		node->num_arguments++;
	}
	if(node->AoR==1){
		if(type == T_WINT)
		nstring_add_char(node->returns,'i');
		if(type == T_WFLOAT64)
		nstring_add_char(node->returns,'f');
		if(type == T_WSTRING)
		nstring_add_char(node->returns,'s');
		/*if(type == T_WBOOL)
		nstring_add_char(node->returns,'b');*/

		node->num_returns++;
	}
	return;
}


BTreePtr BTree_findbyname(BTreePtr *root,Nstring *n){
	int newkey=BTree_getkey(n);
	if(newkey==-1) return NULL;

	while((*root)!=NULL){
		if(newkey<(*root)->key){
			root=&((*root)->LPtr);
		}
		else if(newkey>(*root)->key){
			root=&((*root)->RPtr);
		}
		else{
			if(nstring_cmp(n,(*root)->name)==0){
				return (*root);
			}
			else{
				root=&((*root)->RPtr);
			}
		}
	}
	return NULL;
}

void BTree_print(BTreePtr *root){
	if((*root)!=NULL){
		BTree_print(&((*root)->LPtr));
		BTree_print(&((*root)->RPtr));
		printf("<<BTREE NODE: type: %d, name: %s , args: %d:%s, returns: %d:%s>>\n",(*root)->item_type,(*root)->name->string,
			(*root)->num_arguments,(*root)->args->string,(*root)->num_returns,(*root)->returns->string);
	}
}