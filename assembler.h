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
#include <QStringList>
#include "memory.h"
#include "keyboardmanager.h"
//#include "InstructionFuncs.h"
#include "tileengine.h"
#include "trie.h"


class Error
{
public:
    Error(){}
    Error(QString s, int l, QString se = "-"):lineNumber(l),description(s),segment(se){}

    int lineNumber;
    QString description;
    QString segment;
};

class MainWindow;
class Assembler: public QObject
{
    Q_OBJECT
private:
    QVector<Instruction> instructions;
    QVector<Error> errorList;
    QMap<QString, int> registerIndex;
    QMap<QString, int> labels;
    QMap<QString, int> dataLabels;
    QMap<QString, int> opcode;
    QMap<QString, int (*)(fParam2)> functionsMap;
    QStringList strippedInstrs;
    QStringList rawLines;



    Memory *mem;
    MainWindow *mainW;
    TileEngine *engine;

    bool waiting;

    int address;
    int lineNumber;


    int currentProgress;
    int totalCount;

    int simulationSpeed;

    QVector< QPair<QPair<int,int>,QString> > missingBranchLabels;
    QVector< QPair<QPair<int,int>,QString> > missingJumpLabels;

    int stringDistance(std::string, std::string);
    int minimum4(int, int, int, int);


    QStringList logData;

    int PC;
    int activePC;
    bool exitExec;
    bool resumeFlag;

    QMap<int, int> lineMapping;

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


public:
    Assembler(Memory *memory, QVector<int> *mRegisters, MainWindow*);

    void reset();

    void setSimulationSpeed(int);

    void setLineMapping(QMap<int, int>);

    void getLineMapping();
    void setRawList(QStringList);

    QVector<int> *registers;
    ~Assembler();
    Assembler();
    Trie instructionSet;
    QVector<QString> instructionList;
    void handleInstruction();

    int numberOfErrors();

    bool isWaiting();

    inline void executeFunction();
public slots:
    void exceptionHandler(int);
    void assemble(QStringList dataFileStringList, QStringList textFileStringList);
    void simulate();
    void resumeSimulation();
    void readInt(int);
    void readString(QString);
    void readCharacter(QString);

signals:
    void buttonPressed(int, int, bool);
    void simulationComplete();
    void assemblyComplete();
    void progressUpdate(int);
    void simulationActive();
    void inputRequired(int);
    void setReadingLimit(int);
    void printToConsole(QString);
    void logStringSignal(QString);
    void logDataSignal(QStringList); //For testing.
    void sendErrorMessage(int, QString, QString);
    void executingInstruction(int);
    void executingLine(int);
    void instructionExecuted();
};



#endif // ASSEMBLER_H
