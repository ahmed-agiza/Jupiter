#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include "memory.h"

#include <QAbstractTableModel>
#include <QComboBox>

enum MemorySegment {TextSegment, DataSegment, StackSegment, HeapSegment};
enum DisplayMode {Word, UnsignedWord, Byte, UnsignedByte};
enum MemoryBase {IntegerBase, HexBase, BinaryBase, AsciiBase};


class MemoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MemoryModel(QObject *parent );
    MemoryModel(Memory *m, QObject *parent, MemorySegment ms);
    MemoryModel(Memory *m, QObject *parent, MemorySegment ms, QComboBox *ab, QComboBox *mm, QComboBox *mb);
    void setMemory(Memory *m);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setModifiers(QComboBox *ab, QComboBox *mm, QComboBox *mb);


    //QList<QPair<QString, int> > getAllData();


    ~MemoryModel();

private:
    Memory *memory;
    MemorySegment memoryType;
    int rCount;
    int baseAddress;
    QComboBox* addressBase, *memoryMode, *memoryBase;
    int initMemorySegment();
    void updateCount(int);


signals:

public slots:
};

//bool setData(const QModelIndex &index, const QVariant &value, int role);
/* bool insertRows(int position, int rows, const QModelIndex &index);
bool removeRows(int position, int rows, const QModelIndex &index);*/

#endif // MEMORYMODEL_H
