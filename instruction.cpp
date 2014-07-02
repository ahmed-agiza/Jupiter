#include "instruction.h"
#include <QDebug>


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
    //func = &add;//*
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

