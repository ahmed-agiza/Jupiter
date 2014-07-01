#include "instruction.h"
#include <QDebug>


#define fParam2 QVector<__int32> *base, int rs, int rt, int rd, __int16 imm, int shamt, int &PC, memory *mem

int add(fParam);
int addu(fParam);
int sub(fParam);
int subu(fParam);
int and_(fParam);
int or_(fParam);
int xor_(fParam);
int nor_(fParam);
int srlv(fParam);
int sllv(fParam);
int srav(fParam);
int slt(fParam);
int sltu(fParam);

int sb(fParam);
int lb(fParam);
int lbu(fParam);
int sh(fParam);
int lh(fParam);
int lhu(fParam);
int sw(fParam);
int lw(fParam);
int lwl(fParam);
int lwr(fParam);
int swl(fParam);
int swr(fParam);
int ll(fParam);
int sc(fParam);


int addi(fParam);
int addiu(fParam);
int andi(fParam);
int ori(fParam);
int nori(fParam);
int xori(fParam);
int srl(fParam);
int sll(fParam);
int sra(fParam);
int slti(fParam);
int sltiu(fParam);
int beq(fParam);
int bne(fParam);
int lui(fParam);

int jr(fParam);
int jalr(fParam);
int mfhi(fParam);
int mflo(fParam);
int mthi(fParam);
int mtlo(fParam);
int mult(fParam);
int multu(fParam);
int div(fParam);
int divu(fParam);
int j_(fParam);
int jal(fParam);
int syscall(fParam);
int nop(fParam);
//    addi
//    addiu
//    andi
//    ori
//    nori
//    xori
//    srl
//    sll
//    sra
//    slti
//    sltiu
//    beq
//    bne


//  instruction $register, $register, label
//      beq
//      bne


//  instruction $register, immediate
//      lui


//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi


//  instruction $register, $register
//      mult
//      multu
//      div
//      divu


//  instruction label
//      j
//      jal


//  instruction
//      syscall
//      nop

#define Rdr (*base)[rd]
#define Rtr (*base)[rt]
#define Rsr (*base)[rs]
#define RHI (*base)[32]
#define RLO (*base)[33]

#define OVExNo 1 //Overflow exception code.
#define SyscallExNo 2 //Syscall exception code.
#define UnknownSyscall 3

#define incPC PC += 4

#define PC0Addr 0x00400000

#define data0Addr 0x10010000


instruction::instruction(QString n, QVector<__int32> *b, int o, int s, int t, int d, __int16 im, int sh, instructionFormat f)
{
    opcode = o;
    registers = b;
    name = n;
    rs = s;
    rd = d;
    rt = t;
    imm = im;
    shamt = sh;
    MIPSV = 1; //*
    func = &add;//*
    format = f;


    for (int i = 0; i < 31; i++)
       registers->push_back(i);

   /* qDebug() << "Before:";
    qDebug() << (*registers)[rs];
    qDebug() << (*registers)[rt];
    qDebug() << (*registers)[rd];



    int x;
    execute(x);

    qDebug() << "After:";
    qDebug() << (*registers)[rs];
    qDebug() << (*registers)[rt];
    qDebug() << (*registers)[rd];*/

}

instruction::instruction(const instruction &inst)
{
    registers = inst.registers;
    name = inst.name;
    rs = inst.rs;
    rd = inst.rd;
    rt = inst.rt;
    imm = inst.imm;
    shamt = inst.shamt;
    MIPSV = inst.MIPSV;
    func = inst.func;
    opcode = inst.opcode;

    mem = inst.mem;
    format = inst.format;


}

instruction::instruction()
{

}

instruction & instruction::operator = (const instruction &inst)
{
    registers = inst.registers;
    name = inst.name;
    rs = inst.rs;
    rd = inst.rd;
    rt = inst.rt;
    imm = inst.imm;
    shamt = inst.shamt;
    MIPSV = inst.MIPSV;
    func = inst.func;
    opcode = inst.opcode;

    mem = inst.mem;

    format = inst.format;

    return *this;
}



void instruction::setRegisters(QVector<__int32> *b)
{
    registers = b;
}

void instruction::setMem(memory *m)
{
    mem = m;
}

void instruction::setValues(QString n, int o, int s, int d, int t, __int16 im, int sh)
{
    opcode = o;
    name = n;
    rs = s;
    rd = d;
    rt = t;
    imm = im;
    shamt = sh;
}

void instruction::setOp(int o)
{
    opcode = o;
}

void instruction::setName(QString n)
{
    name = n;
}

instruction::~instruction()
{

}

void instruction::setRs(int s)
{
    rs = s;
}

void instruction::setRd(int d)
{
    rd = d;
}

void instruction::setRt(int t)
{
    rt = t;
}

void instruction::setImm(__int16 im)
{
    imm = im;
}




void instruction::setShamt(int sh)
{
    shamt = sh;
}

void instruction::setFunc(int (*sFunc)(fParam))
{
    func = sFunc;
}

QString instruction::getName() const
{
    return name;
}

int instruction::getOp() const
{
    return opcode;
}

int instruction::getRs() const
{
    return rs;
}

int instruction::getRd() const
{
    return rd;
}
int instruction::getRt() const
{
    return rt;
}

int instruction::getRsData() const
{
    return (*registers)[rs];
}
int instruction::getRdData() const
{
    return (*registers)[rd];
}
int instruction::getRtData() const
{
    return (*registers)[rt];
}

__int16 instruction::getImm() const
{
    return imm;
}
int instruction::getShamt() const
{
    return shamt;
}

void instruction::execute(int &inPC)
{
   int x = (*func)(registers, rs, rt, rd, imm, shamt, inPC, mem);
   if(x != 0)
       emit raiseException(x);

}

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
    Rtr = mem->loadWord(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int lwr(fParam2)
{
    Rtr = mem->loadWordR(Rsr - data0Addr + imm) ;
    incPC;
    return 0;
}
int swl(fParam2)
{
    mem->storeWordL(Rsr - data0Addr + imm, Rtr);
    incPC;
    return 0;
}
int swr(fParam2)
{
    mem->storeWordR(Rsr - data0Addr + imm, Rtr);
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
