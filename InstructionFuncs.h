#ifndef INSTRUCTIONFUNCS_H
#define INSTRUCTIONFUNCS_H
//#include "instruction.h"


#define fParam QVector<int> *, int, int, int, __int16, int , int &, Memory *

#define fParam2 QVector<__int32> *base, int rs, int rt, int rd, __int16 imm, int shamt, int &PC, Memory *mem

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



#endif // INSTRUCTIONFUNCS_H
