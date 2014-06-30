#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QBitArray>
#include <QVector>
#include <QString>
#include <QObject>
#include "memory.h"

#define fParam QVector<int> *, int, int, int, __int16, int , int &, memory *

enum instructionFormat {RFormat, IFormat, JFromat};

class instruction : public QObject
{
    Q_OBJECT
public:
    instruction(QString n, QVector<__int32> *, int, int, int, int, __int16, int, instructionFormat);

    ~instruction();

    void setRegisters(QVector<__int32> *);
    void setValues(QString, int, int, int, int, __int16, int);
    void setName(QString);
    void setRs(int);
    void setRd(int);
    void setRt(int);
    void setImm(__int16);
    void setShamt(int);
    void setOp(int);
    void setMem(memory *);
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

    void execute(int &);
    void setFunc(int (*sFunc)(fParam));

    instruction(const instruction &inst);
    instruction();
    instruction & operator=(const instruction &inst);

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

    memory *mem;

    instructionFormat format;

};

#endif // INSTRUCTION_H
