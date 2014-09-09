#ifndef REGISTERSMODEL_H
#define REGISTERSMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QPair>
#include <QComboBox>

class RegistersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RegistersModel(QObject *parent);
    explicit RegistersModel(QVector<int> *r, QObject *parent);
    explicit RegistersModel(QVector<int> *r, QObject *parent, QComboBox *nm, QComboBox *dsb);
    void setRegs(QVector<int> *r);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role);
   /* bool insertRows(int position, int rows, const QModelIndex &index);
    bool removeRows(int position, int rows, const QModelIndex &index);*/

    QList<QPair<QString, int> > getAllData();
    void emitDataChanged();

    void updateData();

    ~RegistersModel();

private:
    void constructMap();
    QMap<int, QString> registersMap;
    //QList<QPair<int, int*> > *tableData;
    QVector<int> *regs;
    void constructData();
    QComboBox *nameMode, *displayBase;

signals:

public slots:

};

#endif // REGISTERSMODEL_H
