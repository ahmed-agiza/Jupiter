#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instruction.h"
#include <QVector>
#include <QBitArray>
#include <QString>
#include <iostream>
#include <fstream>
#include <QMap>
#include <QSet>
#include <QTextEdit>
#include "memory.h"
//#include "InstructionFuncs.h"
#include "tileengine.h"


#define DOWN_KEY_INDEX 0
#define LEFT_KEY_INDEX 1
#define RIGHT_KEY_INDEX 2
#define UP_KEY_INDEX 3
#define A_KEY_INDEX 4
#define B_KEY_INDEX 5
#define R_KEY_INDEX 6
#define L_KEY_INDEX 7
#define START_KEY_INDEX 8
#define SELECT_KEY_INDEX 9
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
    QVector<Instruction> instructions;
    QVector<Error> errorList;
    QMap<QString, int> registerIndex;
    QMap<QString, int> labels;
    QMap<QString, int> opcode;
    QMap<QString, int (*)(fParam2)> functionsMap;


    Memory *mem;
    TileEngine *engine;
    int address;
    int lineNumber;
    int PC;

    QVector< QPair<QPair<int,int>,QString> > missingBranchLabels;
    QVector< QPair<QPair<int,int>,QString> > missingJumpLabels;

public:
    Assembler(QStringList *stringList, Memory *memory, QVector<int> *mRegisters);
    int getNumber(QString);
    void initializeRegisters();
    void initializeFunctions();
    void handlePR(QRegExp m, QString line);
    void handlePRIL(QRegExp m, QString line);
    void handlePL(QRegExp m, QString line);
    void handlePZ(QRegExp m, QString line);
    void handlePSI(QRegExp m, QString line);
    void handlePDR(QRegExp m, QString line);
    void handlePSR(QRegExp m, QString line);
    void handlePI(QRegExp m, QString line);

    void simulate();
    QVector<int> *registers;
    ~Assembler();
    Assembler();

     QSet<QString> instructionSet;
private slots:
    void exceptionHandler(int);
signals:
    void buttonPressed(int, int, bool);
};

#endif // ASSEMBLER_H
