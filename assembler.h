#ifndef ASSEMBLER_H
#define ASSEMBLER_H
//#include "instruction.h"
#include <QVector>
#include <QBitArray>
#include <QString>
#include <iostream>
#include <fstream>
#include "globalRegex.h"
#include <QMap>
#include <QTextEdit>

class Assembler
{
public:
    QVector<instruction> instructions;
    QMap<QString, int> registerIndex;
    QMap<QString, int> labels;
    Assembler(QTextEdit &Text);
    int getNumber(QString);
    void initializeRegisters();

};

#endif // ASSEMBLER_H
