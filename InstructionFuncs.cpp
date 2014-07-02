#include "instruction.h"
#include "InstructionFuncs.h"

int add(fParam2)
{

    __int32 res = Rtr + Rsr;


    if ((Rtr > 0 && Rsr > 0 && res <0) || (Rtr < 0 && Rsr < 0 && res > 0))
        return OVExNo; //PC to be checked.

    Rdr = res;
    incPC;
    return 0;
}


int addu(fParam2)
{
    Rdr = Rtr + Rsr;
    incPC;
    return 0;

}

int sub(fParam2)
{
    __int32 res = Rtr - Rsr;


    if ((Rtr > 0 && Rsr < 0 && res <0) || (Rtr < 0 && Rsr > 0 && res > 0))
        return OVExNo; //PC to be checked.

    Rdr = res;
    incPC;
    return 0;
}

int subu(fParam2)
{
    Rdr = Rtr - Rsr;
    incPC;
    return 0;
}

int and_(fParam2)
{
    Rdr = Rtr & Rsr;
    incPC;
    return 0;
}
int or_(fParam2)
{
    Rdr = Rtr | Rsr;
    incPC;
    return 0;
}

int xor_(fParam2)
{
    Rdr = Rtr ^ Rsr;
    incPC;
    return 0;
}

int nor_(fParam2)
{
    Rdr = ~(Rtr | Rsr);
    incPC;
    return 0;
}

int srlv(fParam2)
{
    Rdr = Rtr >> Rsr;
    if (Rtr < 0)
        Rdr = Rdr * -1;
    incPC;
    return 0;
}

int sllv(fParam2)
{
    Rdr = Rtr << Rsr;
    incPC;
    return 0;
}

int srav(fParam2)
{
    Rdr = Rtr >> Rsr;
    incPC;
    return 0;
}

int slt(fParam2)
{
    Rdr = (Rsr < Rtr);
    incPC;
    return 0;
}

int sltu(fParam2)
{
    Rdr = (((uint)Rsr) < ((uint) Rtr));
    incPC;
    return 0;
}

int sb(fParam2)
{
    mem->storeByte(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}

int lb(fParam2)
{
    Rtr = mem->loadByte(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}

int lbu(fParam2)
{
    Rtr = mem->loadByteU(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}

int sh(fParam2)
{
    mem->storeHWord(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int lh(fParam2)
{
    Rtr = mem->loadHWord(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int lhu(fParam2)
{
    Rtr = mem->loadHWordU(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int sw(fParam2)
{
    mem->storeWord(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int lw(fParam2)
{
    Rtr = mem->loadWord(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int lwl(fParam2)
{
    Rtr = (Rtr & 0x0000FFFF)|((short)(mem->loadWordL(Rsr - data0Addr, imm)) << 16) ;
    incPC;
    return 0;
}
int lwr(fParam2)
{
    Rtr = (Rtr & 0xFFFF0000) | ((short)mem->loadWordR(Rsr - data0Addr, imm)) ;
    incPC;
    return 0;
}
int swl(fParam2)
{
    mem->storeWordL(Rsr - data0Addr, imm, Rtr);
    incPC;
    return 0;
}
int swr(fParam2)
{
    mem->storeWordR(Rsr - data0Addr, imm, Rtr);
    incPC;
    return 0;
}
int ll(fParam2)
{
    Rtr = mem->loadLinked(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int sc(fParam2)
{
    mem->storeConditional(Rsr - data0Addr + imm, Rtr);
    Rtr = 1;
    incPC;
    return 0;
}


int addi(fParam2)
{
    __int32 res = Rtr + imm;

    if ((Rtr > 0 && imm > 0 && res <0) || (Rtr < 0 && imm < 0 && res > 0))
        return OVExNo;

    Rdr = res;

    incPC;
    return 0;
}

int addiu(fParam2)
{
    Rdr = Rtr - imm;
    incPC;
    return 0;
}
int andi(fParam2)
{
    Rdr = Rtr & imm;
    incPC;
    return 0;
}
int ori(fParam2)
{
    Rdr = Rtr | imm;
    incPC;
    return 0;
}
int nori(fParam2)
{
    Rdr = ~(Rtr | imm);
    incPC;
    return 0;
}
int xori(fParam2)
{
    Rdr = Rtr ^ imm;
    incPC;
    return 0;
}
int srl(fParam2)
{
    Rdr = Rtr >> shamt;
    if (Rtr < 0)
        Rdr = Rdr * -1;
    incPC;
    return 0;
}
int sll(fParam2)
{
    Rdr = Rtr << shamt;
    incPC;
    return 0;
}
int sra(fParam2)
{
    Rdr = Rtr >> shamt;
    incPC;
    return 0;
}
int slti(fParam2)
{
    Rdr = (Rsr < imm);
    incPC;
    return 0;
}
int sltiu(fParam2)
{
    Rdr = (((uint)Rsr) < ((uint) imm));
    incPC;
    return 0;
}
int beq(fParam2)
{
    if(Rtr == Rsr)
        PC = PC + imm;
    else
        incPC;
    return 0;
}
int bne(fParam2)
{
    if(Rtr == Rsr)
        PC = PC + imm;
    else
        incPC;
    return 0;
}
int lui(fParam2)
{
    Rtr = imm << 16;
    incPC;
    return 0;
}

int jr(fParam2)
{
    PC = Rsr - PC0Addr;
    return 0;
}
int jalr(fParam2)
{
    Rdr = PC + PC0Addr + 8; //To be checked.
    PC = Rsr;
    incPC;
    return 0;
}
int mfhi(fParam2)
{
    Rdr = RHI;
    incPC;
    return 0;
}
int mflo(fParam2)
{
    Rdr = RLO;
    incPC;
    return 0;
}
int mthi(fParam2)
{
    RHI = Rsr;
    incPC;
    return 0;
}
int mtlo(fParam2)
{
    RLO = Rsr;
    incPC;
    return 0;
}
int mult(fParam2)
{
    long res = Rsr * Rtr;
    RLO = res & 0xFFFFFFFF;
    RHI = res >> 32;
    incPC;
    return 0;
}
int multu(fParam2)
{
    long res = ((unsigned int)Rsr) * ((unsigned int)Rtr);
    RLO = res & 0xFFFFFFFF;
    RHI = res >> 32;
    incPC;
    return 0;
}
int div(fParam2)
{
    RLO = (Rsr / Rtr);
    RHI = (Rsr % Rtr);
    incPC;
    return 0;
}
int divu(fParam2)
{
    RLO = ((unsigned int)Rsr) / ((unsigned int)Rtr);
    RHI = ((unsigned int)Rsr) % ((unsigned int)Rtr);
    incPC;
    return 0;
}
int j_(fParam2)
{
    PC = imm + 4;
    return 0;
}
int jal(fParam2)
{
    PC = imm + 4;
    (*base)[31] = PC + PC0Addr + 8; //To be checked.
    return 0;
}
int syscall(fParam2)
{
    switch((*base)[2])
    {
        case 0:

            break;
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 8:

            break;
        case 9:

            break;
        case 10:
            PC = -1;
            break;
        case 11:

            break;
        case 12:

            break;
        case 13:

            break;
        default:
            incPC; //To be checked.
            return UnknownSyscall;
            break;
    }
    incPC;
    return SyscallExNo;

}
int nop(fParam2)
{
    incPC;
    return 0;
}
