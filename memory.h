#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>
#include <QByteArray>
#include <QObject>

class memory : public QObject
{
    Q_OBJECT
public:
    memory (int s);
    void storeByte(int, int);
    int loadByte(int) const;
    unsigned int loadByteU(int) const;

    void storeHWord(int, int);
    int loadHWord(int) const;
    unsigned int loadHWordU(int) const;

    void storeWord(int, int);
    void storeWordL(int, int, int);
    void storeWordR(int, int, int);
    int loadWord(int) const;
    unsigned int loadWordU(int) const;
    int loadWordL(int, int) const;
    int loadWordR(int, int) const;

    void storeConditional(int, int);
    int loadLinked(int) const;


    bool isValidByte(int) const;
    bool isValidHWord(int) const;
    bool isValidWord(int) const;
    bool isValidWordL(int, int) const;
    bool isValidWordR(int, int) const;



    ~memory();

signals:
    void raiseException(int);

private:
    QByteArray memBytes;
    int size;



};

#endif // MEMORY_H
