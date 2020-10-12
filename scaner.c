/*
* autor:  Lubomír Závodský <xzavod14>									*
* rok vytvoření: 2020		                            	*
* projekt ifj 2020/2021                               *
*/

// lexikalni analyzator
//library
#include <stdio.h>
#include <ctype.h>
#include "scan.h"
char c; // variable for load chars from input

int i = 1; // pocitadlo na zastavenie


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



// hlavni funkce lexikalniho analyzatoru
int main()
{
	printf("1.analizing start\n");
	printf("start load file\n");

	setSourceFile(stdin);// for testing must be delete
	static T_state state = START; // stav v ktorom sa bude začinat


	while (i)
	{

		c = getc(source);
		printf("%c\n",c);
		switch (state)
		{
			case START:

				if (c == '\n')
				{
					state = EOLINE;
					printf("EOLINE\n");
				}

				else if (c == '_' || isalpha(c))
				{
					state = ID;

				}
								//ČISLO
				else if (isdigit(c))
					state = INT;

									//operatory
				else if (c == '-')
					state = MINUS;
				else if (c == '+')
					state = PLUS;
				else if (c == '*')
					state = MULTIPLE;
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

						state = END;

						break;
					}
					else
					{
						//poslat token
						return 0;
					}
					break;
//dalsie stavy




		}//end of switch
	}//end of while

}
