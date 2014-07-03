#ifndef REGISTERSMODEL_H
#define REGISTERSMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>

class RegistersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RegistersModel(QObject *parent );
    RegistersModel(QVector<int> &r, QObject *parent);
    void setRegs(QVector<int> *r);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int position, int rows, const QModelIndex &index);
    bool removeRows(int position, int rows, const QModelIndex &index);

    QList<QPair<int, int> > *getAllData();

private:
    void constructMap();
    QMap<int, QString> registersMap;
    QList<QPair<int, int> > *tableData;
    QVector<int>regs;

signals:

public slots:

};

#endif // REGISTERSMODEL_H
