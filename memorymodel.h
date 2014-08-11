#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include "memory.h"

#include <QAbstractTableModel>

class MemoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MemoryModel(QObject *parent );
    /*explicit MemoryModel(Memory *m, QObject *parent);
    void setMemory(Memory *m);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role);
   // bool insertRows(int position, int rows, const QModelIndex &index);
   // bool removeRows(int position, int rows, const QModelIndex &index);

    QList<QPair<QString, int> > getAllData();

    void updateData();

    ~MemoryModel();

private:
    void constructMap();
    QMap<int, QString> registersMap;
    //QList<QPair<int, int*> > *tableData;
    QVector<int> *regs;
    void constructData();

signals:

public slots:
*/
};

#endif // MEMORYMODEL_H
