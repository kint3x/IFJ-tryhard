
#include "code_generator.h"

//#TODO generovanie unikatnych nazvov premennych
//#TODO stack s pointerami na premenne



//funkcie začínajú s $
//premenne pre generator zacinaju &
//uzivatelske zacinaju s %

#define GEN_HEADER\
"\n#Start of program"\
"\n .ifjcode20"\
"\n JUMP $main"


#define FUNCTION_INPUTI \
"\n #FUNCTION INPUTI" \


#define GEN_END "\nLABEL END"