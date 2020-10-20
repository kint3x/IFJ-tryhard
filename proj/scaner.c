/*
* autor:  Lubomír Závodský <xzavod14>									*
* rok vytvoření: 2020		                            	*
* projekt ifj 2020/2021                               *
*/

// lexikalni analyzator
//library
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "scaner.h"

char c; // variable for load chars from input

int i = 1; // pocitadlo na zastavenie
//inicializace tokenu
Token *T_init()
{
	struct Token *token = malloc(sizeof(Token));


	return token;
}

void setSourceFile(FILE *f)
{
	source = f;

	printf("loading file\n" );

	if (source == NULL)
	{
		printf("Null file\n");
		return;
	}
} // funkce na citanie suboru



// hlavni funkce lexikalniho analyzatoru //prerobit main na funkciu co vrati tokeny
Token *getNextToken()
{
	printf("1.analizing start\n");
	printf("start load file\n");

	setSourceFile(stdin);// for testing must be delete
	static T_state state = START; // stav v ktorom sa bude začinat

	Token *token = T_init();// inicializace struktury tokenu
Nstring *pole;
	printf("priradenie do data\n");
	token->data = nstring_init();
	printf("som tu jo\n");
	;// inicializace nafukovacieho retazca

		printf("token init\n");
	while (i)
	{

		c = getc(source);

		switch (state)
		{
			case START:

				if (c == '\n')
				{
					printf("som tu\n");
					printf("da%d\n",c);
					nstring_add_char(token->data, c);
					state = EOLINE;
					printf("EOLINE\n");
				}

				else if (c == '_' || isalpha(c))
				{
					state = ID;

				}
								//ČISLO
				else if (isdigit(c))
					state = TINT;

									//operatory
				else if (c == '-')
					state = MINUS;
				else if (c == '+')
					state = PLUS;
				else if (c == '*')
					state = MULTIPLE;
				//	else if (c == '/')
					//	state = TESTDIVIDE;
					// komentar dorobit / MOzE byt delenie

				//logic operators
				else if (c == '!')
					state = NOT;
				else if (c == '&')
					state = AND;
				else if (c == '|')
					state = OR;
				//others char

				else if (c == '(')
					state = LEFTBR;
				else if (c == ')')
					state = RIGHTBR;
				else if (c == ':')
					state = DOUBLEDOT;
				else if (c == ',')
					state = COMMA;
				else if (c == '=')
					state = ASSIGN;
				// less MORE
				else if (c == '<')
					state = LESS;
				else if (c == '>')
					state = MORE;
				// space
				else if (isspace(c))
					state = SPACE;
					//end of file
					else if (c == EOF)
					{
						printf("EOF\n");

						//priradit typ tokenu
						token->type = T_END_OF_FILE;

						state = END;

						break;
					}
					else
					{
						//poslat NULL

						return NULL;
					}
					break;
//dalsie stavy
case EOLINE:

		printf("STAV  EOLINE\n" );
		ungetc(c, source);
		token->type = T_EOL;
		state = START;
		return token;





		}//end of switch
	}//end of while

}
int main() {
	while (1) {

	print_token(getNextToken());

	}

	return 0;
}
//funkce na vizualnu kontrolu tokenov
void print_token(Token *token)
{
	static long counter = 1;

	printf("\n--------------------\n");
	printf("%ld. token\n", counter++);
	printf("of type: ");

	switch (token->type)
	{
	case T_UNKNOWN:
		printf("UNKNOWN");
		break;


	case T_EOL:
		printf("EOL");
		break;

	}

	printf("\n");
	printf("with value: ");
	nstring_print(token->data);
// tiskne na konci "\n"
	printf("--------------------\n");
}

//testovanie
/*int test()
{
	Token *token;
	token = getNextToken();
	while (token->type != T_END_OF_FILE)
	{
		token = getNextToken();
		print_token(token);
	}

	return 1;
}*/
