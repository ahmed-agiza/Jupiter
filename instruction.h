#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QBitArray>
#include <QVector>
#include <QString>
#include <QObject>

#define fParam QVector<int> *, int, int, int, int, int , int &

enum instructionFormat {RFromat, IFormat, JFromat};

class instruction : public QObject
{
    Q_OBJECT
public:
    instruction(QString n, QVector<int> *, int, int, int, int, int, instructionFormat);

    ~instruction();

    void setRegisters(QVector<int> *);
    void setValues(QString, int, int, int, int, int);
    void setName(QString);
    void setRs(int);
    void setRd(int);
    void setRt(int);
    void setImm(int);
    void setShamt(int);
    QString getName() const;
    int getRs() const;
    int getRd() const;
    int getRt() const;
    int getImm() const;
    int getShamt() const;

    void execute(int &);
    void setFunc(int (*sFunc)(fParam));

signals:
    void raiseException(int exceptionNumber);

private:
    //QVector<QBitArray> *registers;
    QVector <int> *registers;
    QString name;
    int rs;
    int rd;
    int rt;
    int imm;
    int shamt;
    int MIPSV;
    int (*func)(fParam);

    instructionFormat format;

};

#endif // INSTRUCTION_H
