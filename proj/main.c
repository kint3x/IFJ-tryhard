/**
 * Projekt: Implementácia riadiacej funkcie pre jazyk IFJ20
 *
 * @brief Implementácia stacku.
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 * @author Martin Matějka <xmatej55@vutbr.cz>
 * @author Ľubomír Závodský <xzavod14@vutbr.cz>
 */

#include <stdio.h>
#include "parser.h"

int main(){
	int ERR=0;

	ERR=parse();


	//printf("%d\n",ERR);

	return ERR;
}