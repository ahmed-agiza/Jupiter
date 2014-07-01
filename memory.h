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
    int loadByteU(int) const;

    void storeHWord(int, int);
    int loadHWord(int) const;
    int loadHWordU(int) const;

    void storeWord(int, int);
    void storeWordL(int, int);
    void storeWordR(int, int);
    int loadWord(int) const;
    int loadWordL(int) const;
    int loadWordR(int) const;

    int storeConditional(int, int);
    int loadLinked(int) const;

    ~memory();

signals:
    void outOfRange();

private:
    QByteArray memBytes;
    int size;



};

#endif // MEMORY_H
