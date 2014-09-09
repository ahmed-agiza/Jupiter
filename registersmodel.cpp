#include "registersmodel.h"

#include <QDebug>

const QString B10("Decimal");
const QString B16("Hexadecimal");
const QString B02("Binary");
const QString NUM("Numbers");
const QString STR("Names");
const QString BTH("Both");

QString getPaddedBinary(int number, int padding);
QString getPaddedHex(int number, int padding);

RegistersModel::RegistersModel(QObject *parent) :
    QAbstractTableModel(parent){
    constructMap();
}


RegistersModel::RegistersModel(QVector<int> *r, QObject *parent=0) : QAbstractTableModel(parent)
{
   regs = r;
   constructMap();
   //tableData = new QList<QPair<int, int*> >;
  // constructData();

}

RegistersModel::RegistersModel(QVector<int> *r, QObject *parent=0, QComboBox *nm = 0, QComboBox *dsb = 0) : QAbstractTableModel(parent)
{
   regs = r;
   constructMap();
   nameMode = nm;
   displayBase = dsb;
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
    registersMap[32] = "HI";
    registersMap[33] = "LO";
    registersMap[34] = "PC";

}



/*void RegistersModel::constructData()
{
    tableData->clear();
    for (int i = 0; i < regs->size(); i++){
        QPair<int, int> mPair;
        mPair.first = i;
        mPair.second = regs->at(i);
        qDebug() << i << "  " << regs->at(i);
        tableData->append(mPair);
    }
}*/

void RegistersModel::updateData()
{
    //constructData();
}

RegistersModel::~RegistersModel()
{
    //delete regs;          // I commented this out. you're deleting regs which was not created in this class. It crashes the program
    //delete tableData;
}


int RegistersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return regs->size();
}
int RegistersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant RegistersModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);

    if (!index.isValid() || index.row() >= regs->size() || index.row() < 0 || index.column() < 0 || index.column() > 2)
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            if (index.row() > 31 && index.row() < 35)
                return registersMap[index.row()];
            if (nameMode->currentText() == NUM){
                return "$" + QString::number(index.row());
            }else if (nameMode->currentText() == STR){
                return registersMap[index.row()];
            }else{
                return "$" + QString::number(index.row()) + "/" + registersMap[index.row()];
            }

        }else if (index.column() == 1){
            int value = (__int32)regs->at(index.row());
            value = value & 0xFFFFFFFF;
            if (displayBase->currentText() == B16){
                return "0x" + getPaddedHex(value, 32).toUpper();
            }else if (displayBase->currentText() == B02){
                return "0b" + getPaddedBinary(value, 32);
            }else{
                return value;
            }
        }
        else
         return QVariant();
    }

        //return "3";
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
    /*if (!index.isValid())
             return Qt::ItemIsEnabled;*/

         return QAbstractTableModel::flags(index);// | Qt::ItemIsEditable;
}
/*bool RegistersModel::setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole)
{

    if (index.isValid() && role == Qt::EditRole)
    {
             //QPair<int, int> p = tableData->value(index.row());
             if(index.column() == 0)
                    registersMap[index.row()] = value.toString();
             else if (index.column() == 1)
                    (*regs)[index.row()] = value.toInt();
             else
                 return false;

             //tableData->replace(index.row(), p);

             emit(dataChanged(index, index));

             return true;
    }

    return false;
}*/

/*bool RegistersModel::insertRows(int position, int rows, const QModelIndex &index=QModelIndex())
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    //<int, int> r; r.first = 0; r.second = 0;
    for (int row=0; row < rows; row++)
       //tableData->insert(position, r);



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
}*/

QList<QPair<QString, int> > RegistersModel::getAllData()
{
    QList<QPair<QString, int>> tableData;
    for (int i = 0; i < regs->size(); i++){
        QPair <QString, int> p;
        p.first = registersMap[i];
        p.second = regs->at(i);

        tableData.append(p);
    }

    return tableData;
}

void RegistersModel::emitDataChanged(){
    emit dataChanged(QModelIndex(), QModelIndex());
}

QString getPaddedHex(int number, int padding){
    QString hexNum = QString::number(number, 16);
    return hexNum.rightJustified(padding/4, '0', true).toUpper();
}

QString getPaddedBinary(int number, int padding){
    QString binary;
    binary.setNum((int)number, 2); //= QString::number(number, 2,);
    /*if (padding == 4){
        if (binary.size() < 4)
            while(binary.size() < 4)
                binary.prepend("0");
    }else if (padding == 8){
        if (binary.size() < 8)
            while(binary.size() < 8)
                binary.prepend("0");
    }else if (padding == 16){
        if (binary.size() < 16)
            while(binary.size() < 16)
                binary.prepend("0");
    }else if (padding == 24){
        if (binary.size() < 24)
            while(binary.size() < 24)
                binary.prepend("0");
    }else if (padding == 32){
        if (binary.size() < 32)
            while(binary.size() < 32)
                binary.prepend("0");*/
        binary = binary.rightJustified(padding, '0', true);
    //}else
      //  return binary;


    if(binary.size() > 4)
        for(int i = 4; i < binary.size(); i += 5)
            binary.insert(i, ' ');

    return binary;
}

