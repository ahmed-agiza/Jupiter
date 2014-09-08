#include "instruction.h"
#include "InstructionFuncs.h"
#include <QDebug>

//#define fParam2 QVector<__int32> *base, int rs, int rt, int rd, __int16 imm, int shamt, int &PC, Memory *mem
#define UNUSE_R Q_UNUSED(imm); Q_UNUSED(PC); Q_UNUSED(mem);
#define UNUSE_I Q_UNUSED(rd); Q_UNUSED(shamt);
#define UNUSE_J Q_UNUSED(rs); Q_UNUSED(rt); Q_UNUSED(rd); Q_UNUSED(shamt);

int add(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);

    __int32 res = Rsr + Rtr;

    if ((Rtr > 0 && Rsr > 0 && res <0) || (Rtr < 0 && Rsr < 0 && res > 0)){
        incPC;
        return OVExNo; //PC to be checked.
    }

    Rdr = res;
    incPC;
    return 0;
}


int addu(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr + Rtr;
    incPC;
    return 0;

}

int sub(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);

    __int32 res = Rsr - Rtr;


    if ((Rtr > 0 && Rsr < 0 && res <0) || (Rtr < 0 && Rsr > 0 && res > 0)){
        incPC;
        return OVExNo; //PC to be checked.
    }

    Rdr = res;
    incPC;
    return 0;
}

int subu(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr - Rtr;
    incPC;
    return 0;
}

int and_(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr & Rtr;
    incPC;
    return 0;
}
int or_(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr | Rtr;
    incPC;
    return 0;
}

int xor_(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr ^ Rtr;
    incPC;
    return 0;
}

int nor_(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = ~(Rsr | Rtr);
    incPC;
    return 0;
}

int srlv(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr >> Rtr;
    if (Rsr < 0)
        Rdr = Rdr * -1;
    incPC;
    return 0;
}

int sllv(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr << Rtr;
    incPC;
    return 0;
}

int srav(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = Rsr >> Rtr;
    incPC;
    return 0;
}

int slt(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = (Rsr < Rtr);
    incPC;
    return 0;
}

int sltu(fParam2)
{
    UNUSE_R
    Q_UNUSED(shamt);
    Rdr = (((uint)Rsr) < ((uint) Rtr));
    incPC;
    return 0;
}

int sb(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    mem->storeByte(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}

int lb(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = mem->loadByte(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}

int lbu(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = mem->loadByteU(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}

int sh(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    mem->storeHWord(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int lh(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = mem->loadHWord(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int lhu(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = mem->loadHWordU(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int sw(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);

    mem->storeWord(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int lw(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = mem->loadWord(Rsr + imm) ;
    incPC;
    return 0;
}
int lwl(fParam2){
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = (Rtr & 0x0000FFFF)|((short)(mem->loadWordL(Rsr - data0Addr + imm)) << 16) ;
    incPC;
    return 0;
}
int lwr(fParam2){
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = (Rtr & 0xFFFF0000) | ((short)mem->loadWordR(Rsr - data0Addr + imm)) ;
    incPC;
    return 0;
}

int swl(fParam2){
    UNUSE_I
    Q_UNUSED(PC);
    mem->storeWordL(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int swr(fParam2){
    UNUSE_I
    Q_UNUSED(PC);
    mem->storeWordR(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int ll(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Rtr = mem->loadLinked(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int sc(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    mem->storeConditional(Rsr - data0Addr + imm, Rtr);
    Rtr = 1;
    incPC;
    return 0;
}


int addi(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    __int32 res = Rsr + imm;

    if ((Rtr > 0 && imm > 0 && res <0) || (Rtr < 0 && imm < 0 && res > 0)){
        qDebug() << "Addi OF";
        return OVExNo;
    }

    Rtr = res;

    incPC;
    return 0;
}

int addiu(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = Rsr + imm;
    incPC;
    return 0;
}
int andi(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = Rsr & imm;
    incPC;
    return 0;
}
int ori(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = Rsr | imm;
    incPC;
    return 0;
}
int nori(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = ~(Rsr | imm);
    incPC;
    return 0;
}
int xori(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = Rsr ^ imm;
    incPC;
    return 0;
}
int srl(fParam2)
{
    UNUSE_R
    Rdr = Rtr >> shamt;
    if (Rtr < 0)
        Rdr = Rdr * -1;
    incPC;
    return 0;
}
int sll(fParam2)
{
    UNUSE_R
    Rdr = Rtr << shamt;
    incPC;
    return 0;
}
int sra(fParam2)
{
    UNUSE_R
    Rdr = Rtr >> shamt;
    incPC;
    return 0;
}
int slti(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = (Rsr < imm);
    incPC;
    return 0;
}
int sltiu(fParam2)
{
    UNUSE_I
    Q_UNUSED(PC);
    Q_UNUSED(mem);
    Rtr = (((uint)Rsr) < ((uint) imm));
    incPC;
    return 0;
}
int beq(fParam2)
{
    UNUSE_I
    Q_UNUSED(mem);
    if(Rtr == Rsr)
        PC = PC + imm * 4;

    incPC;
    return 0;
}
int bne(fParam2)
{
    UNUSE_I
    Q_UNUSED(mem);
    if(Rtr != Rsr)
        PC = PC + imm * 4;
    incPC;
    return 0;
}
int lui(fParam2)
{
    UNUSE_I
    Q_UNUSED(mem);

    Rtr = imm << 16;
    incPC;
    return 0;
}

int jr(fParam2)
{
    Q_UNUSED(imm); Q_UNUSED(shamt); Q_UNUSED(mem);
    PC = Rsr - PC0Addr;
    return 0;
}
int jalr(fParam2)
{
    Q_UNUSED(imm); Q_UNUSED(shamt); Q_UNUSED(mem);
    Rdr = PC - PC0Addr + 4;
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
    __int64 Rsr64 = Rsr;
    __int64 Rtr64 = Rtr;
    __int64 res = Rsr64 * Rtr64;
    __int64 hi = ((res & 0xFFFFFFFF00000000) >> 32);
    __int64 lo = res & 0xFFFFFFFF;
    //qDebug() << res;
   // qDebug() << Rsr;
    //qDebug() << Rtr;

   // RLO = (unsigned int) res & 0xFFFFFFFF;
   // RHI = (int) ((res & 0xFFFFFFFF00000000) >> 32);
    RLO = static_cast<__int32> (lo);
    RHI = static_cast<__int32> (hi);
    //qDebug() << RHI;
    //qDebug() << RLO;
    incPC;
    return 0;
}
int multu(fParam2)
{
   Uint64  Rsr64 = Rsr;
   Uint64 Rtr64 = Rtr;
   Uint64 res = Rsr64 * Rtr64;
   Uint64 hi = ((res & 0xFFFFFFFF00000000) >> 32);
   Uint64 lo = res & 0xFFFFFFFF;
    //qDebug() << res;
   // qDebug() << Rsr;
    //qDebug() << Rtr;

   // RLO = (unsigned int) res & 0xFFFFFFFF;
   // RHI = (int) ((res & 0xFFFFFFFF00000000) >> 32);
    RLO = static_cast<Uint32> (lo);
    RHI = static_cast<Uint32> (hi);
    //qDebug() << RHI;
    //qDebug() << RLO;
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
    PC = (PC & 0xF0000000) | (imm * 4);
    //incPC;
    return 0;
}
int jal(fParam2)
{
    (*base)[31] = PC + PC0Addr + 4;
    PC = (PC & 0xF0000000) | (imm * 4);
    //incPC;
    return 0;
}
int syscall(fParam2)
{
   /* switch((*base)[2])
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
            return SyscallExNo;
            break;
        case 11:

            break;
        case 12:

            break;
        case 13:

            break;
        default:
            incPC;
            return UnknownSyscall;
            break;
    }
    incPC;*/
    return SyscallExNo;

}
int nop(fParam2)
{
    incPC;
    return 0;
}
