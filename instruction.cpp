#include "instruction.h"
#include <QDebug>


#define fParam2 QVector<int> *base, int rs, int rt, int rd, int imm, int shamt, int &PC, memory *mem

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



instruction::instruction(QString n, QVector<int> *b/*QVector<QBitArray> *b*/, int o, int s, int t, int d, int im, int sh, instructionFormat f)
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

    qDebug() << "Before:";
    qDebug() << (*registers)[rs];
    qDebug() << (*registers)[rt];
    qDebug() << (*registers)[rd];


    int x;
    execute(x);

    qDebug() << "After:";
    qDebug() << (*registers)[rs];
    qDebug() << (*registers)[rt];
    qDebug() << (*registers)[rd];

}

int add(fParam2)
{
    qDebug() << (*base)[rt] + (*base)[rs];
    (*base)[rd] = (*base)[rt] + (*base)[rs];

    return 0;
}

void instruction::setRegisters(QVector<int> *b)
{
    registers = b;
}

void instruction::setMem(memory *m)
{
    mem = m;
}

void instruction::setValues(QString n, int o, int s, int d, int t, int im, int sh)
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

void instruction::setImm(int im)
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
int instruction::getImm() const
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
