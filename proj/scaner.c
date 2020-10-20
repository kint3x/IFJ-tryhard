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
	if (source == NULL)
	{
		fprintf(stderr,"Null file\n");
		return;
	}
} // funkce na citanie suboru



// hlavni funkce lexikalniho analyzatoru //prerobit main na funkciu co vrati tokeny
Token *getNextToken()
{


	setSourceFile(stdin);// for testing must be delete
	static T_state state = START; // stav v ktorom sa bude začinat

	Token *token = T_init();// inicializace struktury tokenu
Nstring *pole;

	token->data = nstring_init();
token->type = T_UNKNOWN;
	;// inicializace nafukovacieho retazca


	while (i)
	{

		c = getc(source);

		switch (state)
		{
			case START:

				if (c == '\n')
				{


					state = EOLINE;

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
					nstring_add_char(token->data, c);
					break;
//dalsie stavy
case EOLINE:
		ungetc(c, source);
		token->type = T_EOL;
		state = START;
		return token;
case DOUBLEDOT:
			token->type = T_DOUBLEDOT;
			state = START;
				ungetc(c, source);
			return token;
case PLUS:
					ungetc(c, source);
					token->type = T_PLUS;
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
		case T_ID:
		printf("ID");
		break;



	case T_INT:
		printf("INT");
		break;
	case T_DOUBLEDOT:
		printf("DOUBLEDOT");
		break;
	case T_DOUBLE:
		printf("DOUBLE");
		break;
	case T_COMENT:
		printf("COMENT");
		break;

	case T_PLUS:
		printf("PLUS");
		break;
	case T_MINUS:
		printf("MINUS");
		break;
	case T_MUL:
		printf("MUL");
		break;
	case T_DIV:
		printf("DIV");
		break;
	case T_INTDIV:
		printf("IDIV");
		break;
	case T_STRING:
		printf("STRING");
		break;

	case T_NOT:
		printf("NOT");
		break;
	case T_AND:
		printf("AND");
		break;
	case T_OR:
		printf("OR");
		break;
	case T_EQ_COMP:
		printf("EQUAL");
		break;

	case T_LEFTBR:
		printf("LEFT BRACKET");
		break;
	case T_RIGHTBR:
		printf("RIGHT BRACKET");
		break;
	case T_COMMA:
		printf("COMMA");
		break;
	case T_ASSIGN:
		printf("ASSIGNMENT");
		break;
	case T_SPACE:
		printf("SPACE");
		break;

	case T_LESS:
		printf("LESS");
		break;
	case T_LESS_EQ:
		printf("LESS/EQUAL");
		break;
	case T_MORE:
		printf("MORE");
		break;
	case T_MORE_EQ:
		printf("MORE/EQUAL");
		break;
	case T_NOTEQUAL:
		printf("NOTEQUAL");
		break;



	}

	printf("\n");
	printf("with value: ");
	nstring_print(token->data);
// tiskne na konci "\n"
	printf("\n--------------------\n");
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
