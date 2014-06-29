#include "instruction.h"


void instruction::execute()
{
   (*func)(rs, rt, rd, imm, shamt);

}
