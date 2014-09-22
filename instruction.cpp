#include "instruction.h"
#include <QDebug>


Instruction::Instruction(QString n, QVector<__int32> *b, int o, int s, int t, int d, __int32 im, int sh, InstructionFormat f)
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

    if(format == RFormat){
        instructionWord = ((opcode << 26)|(rs<<21)|(rt<<16)|(rd<<11)|(shamt<<6)|(0));
    }else if(format == IFormat){
        instructionWord = (opcode << 26)|(rs<<21)|(rt<<16)|(imm);
    }else{
        instructionWord = (opcode << 26)|im;
    }

    /*for (int i = 0; i < 32; i++)        // this was 31 I changed it to 32
       registers->push_back(i);*/

    func = NULL;
    fromAssembler = false;
    lineNumber = -1;
    breakpoint = false;
    fake = false;

}

Instruction::Instruction(const Instruction &inst)
    :registers(inst.registers),
    name(inst.name),
    rs(inst.rs),
    rd(inst.rd),
    rt(inst.rt),
    imm(inst.imm),
    shamt(inst.shamt),
    MIPSV(inst.MIPSV),
    func(inst.func),
    opcode(inst.opcode),
    mem(inst.mem),
    format(inst.format),
    instructionWord(inst.instructionWord),
    fromAssembler(inst.fromAssembler),
    lineNumber(inst.lineNumber),
    breakpoint(inst.breakpoint)
{}

Instruction::Instruction()
{

}

Instruction & Instruction::operator = (const Instruction &inst)
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

    instructionWord = inst.instructionWord;
    return *this;
}

void Instruction::setFromAssembler(bool flag){
    fromAssembler = flag;
}

bool Instruction::isFromAssembler() const{
    return fromAssembler;
}

void Instruction::setRawString(QString insText){
    rawString = insText;
}

QString Instruction::getRawString() const{
    return rawString;
}

void Instruction::setLineNumber(int line){
    lineNumber = line;
}

int Instruction::getLineNumber() const{
    return lineNumber;
}

void Instruction::setBreakpoint(bool val){
    breakpoint = val;
}

bool Instruction::isBreakpoint() const{
    return breakpoint;
}

void Instruction::setFake(bool val){
    fake = val;
}

bool Instruction::isFake() const{
    return fake;
}



void Instruction::setRegisters(QVector<__int32> *b)
{
    registers = b;
}

void Instruction::setMem(Memory *m)
{
    mem = m;
}

void Instruction::setValues(QString n, int o, int s, int d, int t, __int16 im, int sh)
{
    opcode = o;
    name = n;
    rs = s;
    rd = d;
    rt = t;
    imm = im;
    shamt = sh;
}

void Instruction::setOp(int o)
{
    opcode = o;
}

void Instruction::setName(QString n)
{
    name = n;
}

Instruction::~Instruction()
{

}

void Instruction::setRs(int s)
{
    rs = s;
}

void Instruction::setRd(int d)
{
    rd = d;
}

void Instruction::setRt(int t)
{
    rt = t;
}

void Instruction::setImm(__int16 im)
{
    imm = im;
}




void Instruction::setShamt(int sh)
{
    shamt = sh;
}

void Instruction::setFunc(int (*sFunc)(fParam))
{
    func =  sFunc;
}

QString Instruction::getName() const
{
    return name;
}

int Instruction::getOp() const
{
    return opcode;
}

int Instruction::getRs() const
{
    return rs;
}

int Instruction::getRd() const
{
    return rd;
}
int Instruction::getRt() const
{
    return rt;
}

int Instruction::getRsData() const
{
    return (*registers)[rs];
}
int Instruction::getRdData() const
{
    return (*registers)[rd];
}
int Instruction::getRtData() const
{
    return (*registers)[rt];
}

__int16 Instruction::getImm() const
{
    return imm;
}
int Instruction::getShamt() const
{
    return shamt;
}

void Instruction::execute(int &inPC)
{
//    qDebug() << "Before excecuting " << this->name;
//    for (int i = 0; i < registers->size(); i++){
//        qDebug() << i << ": " << registers->at(i);
//    }
//    qDebug() << "------------------";

    int x = -1;
    if (func != NULL)
        x = (* func)(registers, rs, rt, rd, imm, shamt, inPC, mem);
    else
        qDebug() << "Null function!";

   if(x != 0)
       emit raiseException(x);

}

int Instruction::getWord()
{
    return instructionWord;
}
