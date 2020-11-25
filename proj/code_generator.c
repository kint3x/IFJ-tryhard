
#include "code_generator.h"

//#TODO generovanie unikatnych nazvov premennych
//#TODO stack s pointerami na premenne



//funkcie začínajú s $
//premenne pre generator zacinaju &
//uzivatelske zacinaju s %
//definicia predprogramovaných funkcii
#define FUN_LEN\
"\n#Start oflen "\
"\n LABEL $len " \
"\n PUSHFRAME " \
"\n DEFVAR LF@&ret1 " \
"\n STRLEN LF@&ret1 LF@&arg1 " \
"\n POPFRAME " \
"\n RETURN"

#define FUN_SUBSTR\
"\nPUSHFRAME "\
"\nDEFVAR LF@&ret1"\
"\nMOVE LF@&ret1 String@"\
"\nCREATEFRAME"\
"\nDEFVAR TF@&arg1"\
"\nMOVE TF@&arg1 LF@&arg1"\
"\nCALL $len"\
"\n# TF@&ret1 -DLŽKA retazca"\
"\nDEFVAR LF@&length"\
"\nMOVE LF@&length TF@&ret1"\
"\nDEFVAR LF@&cond_ret"\
"\nLT LF@&cond_ret  LF@&length int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nEQ LF@&cond_ret  LF@&length int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nLT LF@&cond_ret  LF@&arg2 int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nEQ LF@&cond_ret  LF@&arg2 int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nGT LF@&cond_ret  LF@&arg2 LF@&length"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nEQ LF@&cond_ret  LF@&arg3 int@0"\
"\nJUMPIFEQ $substr$end LF@&cond_ret bool@true"\
"\nDEFVAR LF@&n"\
"\nMOVE LF@&n LF@&length"\
"\nSUB  LF@&n LF@&n LF@&arg2"\
"\nADD LF@&n LF@&n int@1"\
"\nDEFVAR LF@&nevcondition"\
"\nLT LF@&nevcondition LF@&arg3 int@0"\
"\nJUMPIFEQ $substr$changen LF@&nevcondition bool@true"\
"\nGT LF@&nevcondition LF@&arg3 LF@&n"\
"\nJUMPIFEQ $substr$changen LF@&nevcondition bool@true"\
"\nJUMP $substr$cut"\
"\nLABEL $substr$changen"\
"\nMOVE LF@&arg3 LF@&n"\
"\nLABEL $substr$cut"\
"\n DEFVAR LF@&i"\
"\nMOVE LF@&i LF@&arg2"\
"\nSUB LF@&i LF@&i int@1"\
"\nDEFVAR LF@&outchar"\
"\nDEFVAR LF@&conloop"\
"\nLABEL $substr$startloop"\
"\nGETCHAR LF@&outchar LF@&arg1 LF@&i"\
"\nADD LF@&i LF@&i int@1"\
"\nCONCAT LF@&ret1 LF@&ret1 LF@&outchar"\
"\nSUB LF@&arg3 LF@&arg3 int@1"\
"\nGT LF@&conloop LF@&arg3 int@0"\
"\nJUMPIFEQ $substr$startloop LF@&conloop bool@true"\
"\nLABEL $substr$end "\
"\nPOPFRAME "\
"\nRETURN "\







#define GEN_HEADER\
"\n#Start of program"\
"\n .ifjcode20"\
"\n JUMP $main"


#define FUNCTION_INPUTI \
"\n #FUNCTION INPUTI" \


#define GEN_END "\nLABEL END"
