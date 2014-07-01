#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instruction.h"
#include <QVector>
#include <QBitArray>
#include <QString>
#include <iostream>
#include <fstream>
#include "globalRegex.h"
#include <QMap>
#include <QTextEdit>
#include "memory.h"

class Error
{
public:
    Error(){}
    Error(QString s,int l):lineNumber(l),description(s){}

    int lineNumber;
    QString description;
};

class Assembler: public QObject
{
private:
    QVector<instruction> instructions;
    QVector<Error> errorList;
    QMap<QString, int> registerIndex;
    QMap<QString, int> labels;
    QMap<QString, int> opcode;
    QVector<int> registers;
    //Assembler(QTextEdit&);
    memory *dataSegment;

public:
    Assembler(QStringList *stringList);
    int getNumber(QString);
    void initializeRegisters();

    ~Assembler();
    Assembler();

private slots:
    void exceptionHandler(int);
};

#endif // ASSEMBLER_H
