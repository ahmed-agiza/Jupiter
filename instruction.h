#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QBitArray>
#include <QVector>
#include <QString>
#include <QObject>
#include "memory.h"
#include "InstructionFuncs.h"


enum InstructionFormat {RFormat, IFormat, JFormat};

class Instruction : public QObject
{
    Q_OBJECT
public:
    Instruction(QString n, QVector<__int32> *, int, int, int, int, __int32, int, InstructionFormat);

    ~Instruction();

    void setRegisters(QVector<__int32> *);
    void setValues(QString, int, int, int, int, __int16, int);
    void setName(QString);
    void setRs(int);
    void setRd(int);
    void setRt(int);
    void setImm(__int16);
    void setShamt(int);
    void setOp(int);
    void setMem(Memory *);
    QString getName() const;
    int getRs() const;
    int getRd() const;
    int getRt() const;
    int getRsData() const;
    int getRdData() const;
    int getRtData() const;
    __int16 getImm() const;
    int getShamt() const;
    int getOp() const;

    int getWord();
    void execute(int &);
    void setFunc(int (*sFunc)(fParam));

    Instruction(const Instruction &inst);
    Instruction();
    Instruction & operator=(const Instruction &inst);

    void setFromAssembler(bool);
    bool isFromAssembler() const;

    void setRawString(QString);
    QString getRawString() const;

    void setLineNumber(int);
    int getLineNumber() const;

signals:
    void raiseException(int exceptionNumber);

private:
    //QVector<QBitArray> *registers;
    QVector <__int32> *registers;
    QString name;
    int rs;
    int rd;
    int rt;
     __int16 imm;
    int shamt;
    int MIPSV;
    int (*func)(fParam);
    int opcode;

    Memory *mem;

    InstructionFormat format;
    int instructionWord;
    bool fromAssembler;
    QString rawString;
    int lineNumber;
};

#endif // INSTRUCTION_H
