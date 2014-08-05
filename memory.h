#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>
#include <QByteArray>
#include <QObject>

class memory : public QObject
{
    Q_OBJECT
public:
    memory ();
    void storeByte(int, char);
    int loadByte(int) const;
    unsigned char loadByteU(int) const;

    void storeHWord(int, short);
    short loadHWord(int) const;
    unsigned short loadHWordU(int) const;

    void storeWord(int, int);
    int loadWord(int) const;
    unsigned int loadWordU(int) const;

    void storeConditional(int, int);
    int loadLinked(int) const;

    unsigned int getByteSegment(int) const;
    unsigned int getHWordSegment(int) const;
    unsigned int getWordSegment(int) const;

    //    bool isValidWordL(int, int) const;
    //    bool isValidWordR(int, int) const;
    //    void storeWordL(int, int, int);
    //    void storeWordR(int, int, int);
    //    int loadWordL(int, int) const;
    //    int loadWordR(int, int) const;


    ~memory();

signals:
    void raiseException(int);

private:
    QByteArray textSegment;
    QByteArray dataSegment;
    QByteArray heapSegment;
    QByteArray stackSegment;

    const unsigned int textSegmentBaseAddress = 0x4000000;
    const unsigned int dataSegmentBaseAddress = 0x10010000;
    const unsigned int heapSegmentBaseAddress = 0x100d0000;
    const unsigned int stackSegmentLimitAddress = 0x80000000;

    const unsigned int textSegmentPhysicalSize = 64 * 1024;
    const unsigned int dataSegmentPhysicalSize = 64 * 1024;
    const unsigned int heapSegmentPhysicalSize = 128 * 1024;
    const unsigned int stackSegmentPhysicalSize = 128 * 1024;
};

#endif // MEMORY_H
