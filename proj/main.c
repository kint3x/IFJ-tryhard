/**
 * Projekt: Implementace překladače imperativního jazyka IFJ20.
 *
 * @brief Spustenie programu
 *
 * @author Matej Kocman <xkocma07@vutbr.cz>
 * @author Martin Matějka <xmatej55@vutbr.cz>
 * @author Ľubomír Závodský <xzavod14@vutbr.cz>
 */

#include <stdio.h>
#include "parser.h"

int main(){
	//implementacia na tri riadky, ez 
	
	int ERR=0;

	ERR=parse();

	return ERR;
}