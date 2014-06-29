#ifndef MEMORY_H
#define MEMORY_H

#include <QMap>

class memory
{
public:
    memory();

    void storeBye(int, int);
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
    void loadLinked(int) const;


private:
    QMap <int, int> data;
};

#endif // MEMORY_H
