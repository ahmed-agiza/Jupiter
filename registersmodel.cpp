#include "registersmodel.h"

RegistersModel::RegistersModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    constructMap();
}


RegistersModel::RegistersModel(QVector<int> &r, QObject *parent=0) : QAbstractTableModel(parent)
{
   regs = r;
   constructMap();
}

void RegistersModel::constructMap()
{
    registersMap[0] = "$zero";
    registersMap[1] = "$at";
    registersMap[2] = "$v0";
    registersMap[3] = "$v1";
    registersMap[4] = "$a0";
    registersMap[5] = "$a1";
    registersMap[6] = "$a2";
    registersMap[7] = "$a3";
    registersMap[8] = "$t0";
    registersMap[9] = "$t1";
    registersMap[10] = "$t2";
    registersMap[11] = "$t3";
    registersMap[12] = "$t4";
    registersMap[13] = "$t5";
    registersMap[14] = "$t6";
    registersMap[15] = "$t7";
    registersMap[16] = "$s0";
    registersMap[17] = "$s1";
    registersMap[18] = "s2";
    registersMap[19] = "$s3";
    registersMap[20] = "$s4";
    registersMap[21] = "$s5";
    registersMap[22] = "$s6";
    registersMap[23] = "$s7";
    registersMap[24] = "$t8";
    registersMap[25] = "$t9";
    registersMap[26] = "$k0";
    registersMap[27] = "$k1";
    registersMap[28] = "$gp";
    registersMap[29] = "$sp";
    registersMap[30] = "$fp";
    registersMap[31] = "$ra";

}

int RegistersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tableData->size();
}
int RegistersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant RegistersModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    if (!index.isValid() || index.row() >= tableData->size() || index.row() < 0 || index.column() < 0 || index.column() > 2)
        return QVariant();

    if (role == Qt::DisplayRole)
        return QVariant(regs[index.row()]);

    return QVariant();
}

QVariant RegistersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

    if (orientation == Qt::Horizontal)
    {
             switch (section) {
                 case 0:
                     return QVariant("Register");

                 case 1:
                     return QVariant("Value");

                 default:
                     return QVariant();
             }
  }

    return QVariant();
}
Qt::ItemFlags RegistersModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return Qt::ItemIsEnabled;

         return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool RegistersModel::setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole)
{

    if (index.isValid() && role == Qt::EditRole)
    {
            QPair<int, int> p = tableData->value(index.row());
             if(index.column() == 0)
                    p.first = value.toInt();
             else if (index.column() == 1)
                    p.second = value.toInt();
             else
                 return false;

             tableData->replace(index.row(), p);

             emit(dataChanged(index, index));

             return true;
    }

    return false;
}

bool RegistersModel::insertRows(int position, int rows, const QModelIndex &index=QModelIndex())
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    QPair<int, int> r; r.first = 0; r.second = 0;
    for (int row=0; row < rows; row++)
       tableData->insert(position, r);


    endInsertRows();
    return true;
}

bool RegistersModel::removeRows(int position, int rows, const QModelIndex &index=QModelIndex())
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; ++row)
     {
        tableData->removeAt(position);
     }

     endRemoveRows();
     return true;
}
QList<QPair<int, int> > *RegistersModel::getAllData()
{
    return tableData;
}
