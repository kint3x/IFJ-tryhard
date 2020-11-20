/*
* autor:  Lubomír Závodský <xzavod14>									*
* rok vytvorené: 2020		                            	*
* projekt ifj 2020/2021                               *
*/
//
// lexikalni analyzator
//library
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "scaner.h"

int c; // variable for load chars from input

int i = 1; // pocitadlo na zastavenie
//inicializace tokenu
Token *T_init()
{
	struct Token *token = malloc(sizeof(Token));


	return token;
}
// funkce na citanie suborus
void setSourceFile(FILE *f)
{
	source = f;
	if (source == NULL)
	{
		//fprintf(stderr,"Null file\n");
		return;
	}
}

// hlavni funkce lexikalniho analyzatoru //prerobit main na funkciu co vrati tokeny
Token *getNextToken()
{
	setSourceFile(stdin);// for testing must be delete
	static T_state state = START; // stav v ktorom sa bude za�inat
	Token *token = T_init();// inicializace struktury tokenu
	token->data = nstring_init();// inicializace nafukovacieho retazca
	token->type = T_UNKNOWN; // defaultne určeny typ tokenu
	while (i)
	{
		c = getc(source);
	//	printf("%c\n", c);
		switch (state)
		{
			//start
			case START:

				 if (c == '\n')// if eol
				{
					state = EOLINE;
				}
				//identifikator
				else if (c == '_' || isalpha(c))
				{
					state = ID;
				}
				//cISLO
				else if (isdigit(c) && (!(c == '0')))

										state = TINT;
				else if (c == '0')

				  state = ZERO;
				//STRING

				else if (c == '\"')
					state = STRING;
				//operatory
				else if (c == '-')
					state = MINUS;
				else if (c == '+')
					state = PLUS;
				else if (c == '*')
					state = MULTIPLE;
				else if (c == '/')
					state = TESTDIVIDE;
				//logic operators
				else if (c == '!')
					state = NOT;
				else if (c == '&')
					state = AND;
				else if (c == '|')
					state = OR;
				//others char
				else if (c == ';')
					state = SEMI;
				else if (c == '{')
					state = LEFTBRACET;
				else if (c == '}')
					state = RIGHTBRACET;
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
				//priradit typ tokenu
				state = END;
				break;
				}
				else
				{
					//lexikalna chyba token sa nerozpoznal
					token->type = T_ERR;
					//fprintf(stderr,"lexikal error UNKNOWN char\n");
					nstring_add_char(token->data, c);
					i = 0;//ukončený while
					return token;
				}
				nstring_add_char(token->data, c);
				break;
			//dalsie stavy
			//prechodove stavy
			//koncove stavy:

			case EOLINE:
					ungetc(c, source);
					token->type = T_EOL;
					state = START;
					return token;// use in parser -> token T_EOL
					case ID:

								token->type = T_ID;
								if ((isalpha(c) || (isdigit(c)) || (c == '_')))
								{

									//token->data = c;
									state = KEYW;
									nstring_add_char(token->data, c);//kontrola zda nenastalo klučové slovo
								}
								else
								{
									ungetc(c, source);//koniec id už neni žiaden vhodný znak na vstupe *vratíme na vstup
									state = START;
									return token;
								}
						break;
					case KEYW:
					//kontrola zda je skutočne načtený cely slovo
					if (! ((isalpha(c) || (isdigit(c)) || (c == '_'))) ){
						if (!(nstring_str_cmp(token->data, "else")))
							{
								token->type = T_WELSE;
								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "float64")))
							{
								token->type = T_WFLOAT64;

								state = START;
								ungetc(c, source);
								return token;
							}

							else if (!(nstring_str_cmp(token->data, "for")))
							{
								token->type = T_WFOR;

								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "func")))
							{
								token->type = T_WFUNC;

								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "if")))
							{
								token->type = T_WIF;

								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "int")))
							{
								token->type = T_WINT;

								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "package")))
							{
								token->type = T_WPACKAGE;

								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "return")))
							{
								token->type = T_WRETURN;

								state = START;
								ungetc(c, source);
								return token;
							}
							else if (!(nstring_str_cmp(token->data, "string")))
							{
								token->type = T_WSTRING;

								state = START;
								ungetc(c, source);
								return token;
							}
						else{
							// klučove slovo nerozpoznane
						ungetc(c, source);
								state = ID; //DONE //done
							}
								break;}

							else {
								// neprebehla kontrola keyword lebo niesme na konci slova
								ungetc(c, source);
								state = ID;
								break;
							}





				case EPLUSMINUSZERO: // E+-0

				if (isdigit(c)&& (!(c == '0')) )
				{
					nstring_add_char(token->data, c);
					token->type = T_ERR;
					state = START;
					return token;

				}
				else if (c == '0') {nstring_add_char(token->data, c);
				break;}
				else {
					state = START;
					ungetc(c, source);
					return token;
				}
				case DOUBLECONTROL:	//za bodkov očakavaný číslo
					if (!isdigit(c)) {//fprintf(stderr,"chyba double .\n");

						token->type = T_ERR;
						state = START;
						return token;

						}

						state = DOUBLE;

						ungetc(c, source);
						break;
				case EPLUSMINUSNUMBER:
				if (isdigit(c)){nstring_add_char(token->data, c); break; }

				else {
						ungetc(c, source);
						state = START;
						return token;

				}
				case EPLUSMINUS:

				if (isdigit(c) && (!(c == '0')) )
				{
					state = EPLUSMINUSNUMBER;
					nstring_add_char(token->data, c); break; }
				else if (c == '0') {
						state = EPLUSMINUSZERO;
						nstring_add_char(token->data, c);
						break;
					}
				else {
						ungetc(c, source);
						token->type = T_ERR;
						state = START;
						return token;

				}
				case E:
				if ((c == '+') || (c == '-'))
				{
					nstring_add_char(token->data, c);
					state = EPLUSMINUS;
					break;

				}
				else if (isdigit(c) && (!(c == '0')) ){
					nstring_add_char(token->data, c);
				state = EPLUSMINUSNUMBER;
				break; }
				else if (c == '0') {
					state = EPLUSMINUSZERO;
					nstring_add_char(token->data, c);
					break;;
				}
				else{
					token->type = T_ERR;
					state = START;
					return token;

				}
				case TINT:
				token->type = T_INT;
				if (isdigit(c))
				{
					nstring_add_char(token->data, c);
					state = TINT;
					break;
				}
				else if (c == '.')// ak sa nachadza bodka kontrolujem 10tine cislo
				{
					nstring_add_char(token->data, c);
					state = DOUBLECONTROL;
					break;
					}
				else if ((c == 'E') || (c == 'e'))
				{
					nstring_add_char(token->data, c);
					state = E;
					break;

				 }
				 else
					{

							state = START;
							ungetc(c, source);
							return token;
					}


					/*token->type = T_INT;
					if (isdigit(c))
					{
						nstring_add_char(token->data, c);
						state = TINT;
						break;
					}
					else if (c == '.')// ak sa nachadza bodka kontrolujem 10tine cislo
					{

						nstring_add_char(token->data, c);
							c = getc(source);
							if (!isdigit(c)) {//fprintf(stderr,"chyba double .\n");
							token->type = T_ERR;
							return token;

					 }state = DOUBLE;
						ungetc(c, source);
					}
					else if ((c == 'E') || (c == 'e'))
					{
						nstring_add_char(token->data, c);
						c = getc(source);
						if (!isdigit(c)) {//fprintf(stderr,"chyba double po e nenasleduje číslo\n");
						token->type = T_ERR;
						return token;
					 }
						ungetc(c, source);
						state = DOUBLE;
					}
					else
					{

							state = START;
							ungetc(c, source);
							return token;




					}
					break;*/
				case DOUBLE:
				//printf("%c\n", c);
					token->type = T_DOUBLE;
				if (isdigit(c))
				{
					nstring_add_char(token->data, c);
					state = DOUBLE;
					break;
				}
				else if ((c == 'E') || (c == 'e'))
				{
					nstring_add_char(token->data, c);
					state = E;
					break;

				 }
				 else
				{

						state = START;
						ungetc(c, source);
						return token;
				}


			case ZERO:

				//c = getc(source);
				if (isdigit(c))

				{

					token->type = T_ERR;
					state =START;
					//fprintf(stderr,"lexikal error UNKNOWN char\n");
					nstring_add_char(token->data, c);
					ungetc(c, source);
					return token;
				}
				else if (c == '.')// ak sa nachadza bodka kontrolujem 10tine cislo
				{
					nstring_add_char(token->data, c);
					state = DOUBLECONTROL;
					break;
					}
				else if ((c == 'E') || (c == 'e'))
				{
					nstring_add_char(token->data, c);
					state = E;
					break;

				 }
				else
			{
				token->type = T_INT;
				state =START;
				return token;




			 }
			case NOT:
				if (c == '=')
				{
					token->type = T_NOTEQUAL;
					nstring_add_char(token->data, c);
					state = START;
					return token;
				}//vrati token !=

				ungetc(c, source); // nasledujuci znak nieje =  znak sa vrati na spracovanie
				token->type = T_NOT;
				state = START;
				return token; //DONE
			case AND:
				ungetc(c, source);
				token->type = T_AND;
				state = START;
				return token; //DONE
			case OR:
				ungetc(c, source);
				token->type = T_OR;
				state = START;
				return token; //DONE
			case MULTIPLE:
				ungetc(c, source);
				token->type = T_MUL;
				state = START;
				return token; //DONE
			case SEMI:
								ungetc(c, source);
								token->type = T_SEMI;
								state = START;
								return token;//done
			case MORE:
				if (c == '=')
				{
					nstring_add_char(token->data, c);
					token->type = T_MORE_EQ;
					state = START;
					return token;
				}
				else
				{
					ungetc(c, source);
					token->type = T_MORE;
					state = START;
					return token;
				}//done
			case MINUS:
				ungetc(c, source);
				//nstring_add_char(token->data, c);
				token->type = T_MINUS;
				state = START;
				return token; //DONE
			case LESS:
				if (c == '=')
				{
					nstring_add_char(token->data, c);
					token->type = T_LESS_EQ;
					state = START;
					return token;
				}
				else
				{
					ungetc(c, source);
					token->type = T_LESS;
					state = START;
					return token;
				}//done
			case HEX_CON:
				if (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || isdigit(c))
				{
					nstring_add_char(token->data, c);
					state = STRING;
				}
				else
				{
					token->type = T_ERR;
					state = START;
					return token;
				}

				break;//done zapis znaku pomocov escape sekvencie kontrola lexikalneho rozsahu
			case HEX:

				if (((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')) || isdigit(c))
				{
					nstring_add_char(token->data, c);
					state = HEX_CON;
					break;
				}
				else
				{
					token->type = T_ERR;
					state = START;
					return token;// done zapis znaku pomocov escape sekvencie kontrola lexikalneho rozsahu
				}


			case DOUBLEDOT:
					if (c == '=') {
						token->type = T_DOUBLEDOT;
						state = START;
							nstring_add_char(token->data, c);
						return token;
						 }
						 //ak sa nenachadza = za tým  neni to platny znak
					else{
						token->type = T_ERR;
						i=0;
						//fprintf(stderr,"UNKNOWN token :\n");
						return token;}
			case PLUS:
					ungetc(c, source);
					token->type = T_PLUS;
					state = START;
					return token;//done
			case TESTDIVIDE:
						if (c == '/') {
							ungetc(c, source);
							state = COMENT;
							break;
						}
						else if (c == '*') {
							ungetc(c, source);
							state = BLOCKCOMENT;
							break;
						}
						else{
							state = START;
							ungetc(c, source);
							token->type = T_DIV;
						return token;}// stav ktory kontroluje význam znaku /
			case COMENT:

					if (c == '\n') {
						nstring_char_remove(token->data);
						state = START;
						//token->type = T_COMENT;
						ungetc(c, source);
						break;
					}
					else if (c == EOF)
						{
							ungetc(c, source);
							state = START;
							break;
						}
					else {
						//nstring_add_char(token->data, c);
						break;
					}//riadkový komentar
			case BLOCKCOMENT:
						nstring_char_remove(token->data);
						if (c == '*') {
							nstring_add_char(token->data, c);
							c = getc(source);
							if (c == '/') {
								//token->type = T_COMENT;
								//nstring_add_char(token->data, c);
								nstring_char_remove(token->data);
								state = START;
								break;
								//return token;
							}else if (c == EOF)//do konca suboru nebol ukončený započatý blokový komentar
								{
									token->type = T_ERR;
									//fprintf(stderr,"neukončený blokový komentar\n");
									state = START;
								return token;
							}
							else
							{
								ungetc(c, source);
								 break;
						 }}
						 else if (c == EOF)//do konca suboru nebol ukončený započatý blokový komentar
							 {
								 token->type = T_ERR;
								 //fprintf(stderr,"neukončený blokový komentar\n");
								 state = START;
							 return token;}
						else {

						//nstring_add_char(token->data, c);
						break;
					 }

			case SPACE:
					ungetc(c, source);
					nstring_char_remove(token->data);//white space discard
					state = START;
					break;
			case LEFTBR:
					ungetc(c, source);
					token->type = T_LEFTBR;
					state = START;
					return token; //DONE
			case COMMA:
						ungetc(c, source);
						token->type = T_COMMA;
						state = START;
						return token; //DONE

			case RIGHTBR:
						ungetc(c, source);
						token->type = T_RIGHTBR;
						state = START;
						return token; //DONE
			case LEFTBRACET:
							ungetc(c, source);
							token->type = T_LEFTBRACET;
							state = START;
							return token; //DONE
			case RIGHTBRACET:
								ungetc(c, source);
								token->type = T_RIGHTBRACET;
								state = START;
								return token; //DONE
			case STRING:
							//printf("retazec\n");

							if (c == '\"')
							{
								nstring_add_char(token->data, c);
								token->type = T_STRING;
								state = START;
								return token;
							}
							else if (c == 92)
							{

								nstring_add_char(token->data, c);
								state = ESC;
							}
							else if (c > 31)
							{
								nstring_add_char(token->data, c);
								state = STRING;
							}
							else if (c == EOF)
							{
								//fprintf(stderr,"error noend of string /n");
								token->type = T_ERR;
								state = START;
								return token;
							}
				break;
			case ESC:

				if ((c == 'n') || (c == 't') || (c == '"')|| (c == 92))
				{

					nstring_add_char(token->data, c);
					state = STRING;
				}
				else if (c == 'x')
				{
					nstring_add_char(token->data, c);
					state = HEX;
				}
				else
				{
					token->type = T_ERR;
					state = START;
					return token;
				}
				break;
			case ASSIGN:
								if (c == '=')
								{
								nstring_add_char(token->data, c);
									token->type = T_EQ_COMP;
									state = START;
									return token;
								}
							
								else
								{

									ungetc(c, source);
									token->type = T_ASSIGN;
									state = START;
									return token;
								}
				break;
			case END:
		token->type = T_END_OF_FILE;

			i = 0;
					return token;
		}//end of switch
	}//end of while
	return NULL;
}
/*
int main() {
	while (i) {

	print_token(getNextToken());

}

	return 0;
}*/
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
	case T_END_OF_FILE:
		printf("EOF");
		break;
	case T_WELSE:
		printf("ELSE");
		break;
	case T_WFLOAT64:
		printf("WFLOAT64");
		break;
		case T_WFOR:
			printf("WFOR");
			break;
		case T_WFUNC:
			printf("WFUNC");
			break;
		case T_WINT:
			printf("WINT");
			break;
		case T_WIF:
		printf("WIF");
		break;
		case T_WSTRING:
		printf("WSTRING");
		break;
		case T_SEMI:
		printf("SEMI");
		break;
		case T_LEFTBRACET:
		printf("LEFTBRACET");
		break;
		case T_RIGHTBRACET:
		printf("RIGHTBRACET");
		break;
		case T_WRETURN:
		printf("WRETURN");
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
		case T_WPACKAGE:
			printf("PACKAGE");
			break;
	case T_ERR:
		printf("T_ERR");
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
