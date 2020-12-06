/**
 * Projekt: Implementácia prekladača pre jazyk IFJ20
 *
 * @brief Implementácia err stacku pre volané funkcie pred prečítaním ich hlavičky.
 *
 * @author Martin Matějka <xmatej55@vutbr.cz>
 */

#include "sem_estack.h"

int EStack_addcall(EStackPtr *root,Nstring *called_name, Nstring *left, Nstring *arg_types){
	while((*root)!=NULL){
		root=&((*root)->next);
	}
	(*root)=(EStackPtr)malloc(sizeof(struct estack));
	if((*root)==NULL) return ERR_INTERNAL;

	(*root)->called_name=nstring_init();
	if((*root)->called_name==NULL) return ERR_INTERNAL;
	(*root)->left=nstring_init();
	if((*root)->left==NULL) return ERR_INTERNAL;
	(*root)->called_arg_types=nstring_init();
	if((*root)->called_arg_types==NULL) return ERR_INTERNAL;

	(*root)->next=NULL;
	if(!nstring_cpy(called_name,(*root)->called_name)) return ERR_INTERNAL;
	if(!nstring_cpy(left,(*root)->left)) return ERR_INTERNAL;
	if(!nstring_cpy(arg_types,(*root)->called_arg_types)) return ERR_INTERNAL;
	return ERR_RIGHT;
}

int EStack_solveproblems(EStackPtr *root,BTreePtr global_tree){
	BTreePtr search;
	while((*root)!=NULL){
		search = BTree_findbyname(&global_tree,(*root)->called_name); //zistime či funkcia už existuje

		if(search == NULL) return ERR_SEMAN_NOT_DEFINED; 	//ak nie voláme chybu

		if(nstring_len((*root)->left) > 0){	// ak sme volali a priradzovali musíme porovnať návratové typy
			if(!nstring_ret_cmp((*root)->left,search->returns)) return ERR_SEMAN_PARAMETERS;
		}
		else{ // tak potom funkcia nemôže mať return
			if(search->num_returns>0) return ERR_SEMAN_PARAMETERS;
		}

		// nakoniec porovnáme volané parametre

		if(!nstring_ret_cmp((*root)->called_arg_types,search->args)) return ERR_SEMAN_PARAMETERS;

		//ak je všetko ok posunieme sa na ďalšie volanie

		root=&((*root)->next);
	}
	// všetky volania prešli, môžeme vrátiť ERR_RIGHT
	return ERR_RIGHT;
}

void EStack_dispose(EStackPtr *root){
	EStackPtr tmp=(*root);
	EStackPtr del;
	(*root)=NULL;
	while(tmp!=NULL){
		del=tmp;
		tmp=tmp->next;
		nstring_free(del->called_name);
		nstring_free(del->left);
		nstring_free(del->called_arg_types);
		free(del);
	}
}