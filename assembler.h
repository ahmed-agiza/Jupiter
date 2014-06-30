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

class Assembler: public QObject
{
public:
    QVector<instruction> instructions;
    QMap<QString, int> registerIndex;
    QMap<QString, int> labels;
    QMap<QString, int> opcode;
    QVector<int> registers;
    //Assembler(QTextEdit&);

public:
    Assembler(QStringList *stringList);
    int getNumber(QString);
    void initializeRegisters();

    ~Assembler();
    Assembler();
};

#endif // ASSEMBLER_H
