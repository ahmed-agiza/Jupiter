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
#include "keyboardmanager.h"
//#include "InstructionFuncs.h"
#include "tileengine.h"
#include "trie.h"


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
    Q_OBJECT
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

    int stringDistance(std::string, std::string);
    int minimum4(int, int, int, int);
public:
    Assembler(QStringList *textFileStringList, QStringList *dataFileStringList, Memory *memory, QVector<int> *mRegisters);
    void parseTextSegment(QStringList*);
    void parseDataSegment(QStringList*);
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

     Trie instructionSet;
     QVector<QString> instructionList;
private slots:
    void exceptionHandler(int);
signals:
    void buttonPressed(int, int, bool);
};

#endif // ASSEMBLER_H
