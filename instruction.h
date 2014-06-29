#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QBitArray>
#include <QVector>
#include <QString>
enum instructionFormat {RFromat, IFormat, JFromat};

class instruction
{
public:
    instruction(QString, QVector<QBitArray> *, int, int, int, int, int, int, instructionFormat);
    void setRegisters(QVector<QBitArray> *);
    void setValues(QString, int, int, int, int);
    void setName(QString);
    void setRs(int);
    void setRd(int);
    void setRt(int);
    void setImm(int);
    void setAddr(int);
    void setPC(int*);
    void setShamt(int);
    void execute(void);
    void setFunc(void*);

private:
    QVector<QBitArray> *registers;
    QString name;
    int address;
    int *PC;
    int rs;
    int rd;
    int rt;
    int imm;
    int shamt;
    void (*func)(int, int, int, int, int, int, int);

    instructionFormat format;

};

#endif // INSTRUCTION_H
