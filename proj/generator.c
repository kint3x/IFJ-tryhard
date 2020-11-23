/*
 autor:  Lubomír Závodský <xzavod14>									*
* rok vytvorené: 2020		                            	*
* projekt ifj 2020/2021                               *
*/
//
// generator vnutorneho kodu
//library
#include "generator.h"

void generatecodefromtoken(Token *token){
//premenne
  //  static int parametrs =  1; //počet parametrov
  //  print_token(token);
 //= nstring_init();







    static bool package = false; //bol package v uvode čakam až kym nepride eol
    static bool funcCall = false;  // volá sa funkce
    static bool funcDEF = false;  // definuje sa funkce
    static char *id;



	static long counter = 1;



  if (package){

      switch (token->type)
              {
                  case T_EOL:
                    package = false;
                    break;
                  }
                }
  else if (funcCall)
{
  switch (token->type)
  {case T_ID:
    *id = token->type;
  break;
  case T_EOL:
  printf("JUMP $$");
  nstring_print(*id);
    printf("\n");

  funcCall = false;
  break;
}
}
  else  {
      	switch (token->type)
      	{

      	case T_EOL:
      		printf("EOL");
      		break;
      		case T_ID:


      		printf("ID");
      		break;
      	case T_END_OF_FILE:
      		printf("EOF\n");
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
      			//definujem funkciu
            funcCall = true;
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
      		case T_WPACKAGE://korektne začatý program generujem hlavičku
      			printf(".IFJcode20\n");
            package = true;
            // bol package takže
      			break;
      	case T_ERR:
      		printf("T_ERR");
      		break;

      	}}

      }
