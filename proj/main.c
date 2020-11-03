
#include <stdio.h>
#include "parser.h"

int main(){
	int ERR=0;

	printf("Hallo\n");
	ERR=p_prog();

	printf("%d\n",ERR);

	return ERR;
}