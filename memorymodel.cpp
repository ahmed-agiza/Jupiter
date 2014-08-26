#include "memorymodel.h"

MemoryModel::MemoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{}




MemoryModel::MemoryModel(Memory *m, QObject *parent=0, MemorySegment ms = DataSegment, DisplayMode dispMode = Word, MemoryBase memBase = IntegerBase)
    : QAbstractTableModel(parent), memory(m), memoryType(ms), dm(dispMode), mb(memBase)
{
    int totalSize;
    switch (memoryType) {
    case DataSegment:
        totalSize = memory->getDataSegmentSize();
        break;
    case TextSegment:
        totalSize = memory->getTextSegmentSize();
        break;
    case HeapSegment:
        totalSize = memory->getHeapSegmentSize();
        break;
    case StackSegment:
    default:
        totalSize = memory->getStackSegmentSize();
        break;
    }
    if (dm == Word || dm == UnsignedWord){
        rCount = totalSize/4;
    }else{
        rCount = totalSize;
    }
    qDebug() << rCount;
}





MemoryModel::~MemoryModel()
{}


int MemoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rCount;

}
int MemoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant MemoryModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
 //   qDebug() << "Getting Data at (" << index.row() << ", " << index.column() << ")";
    if (!index.isValid() || index.row() >= rCount || index.row() < 0 || index.column() < 0 || index.column() > 2)
        return QVariant();

    if (role == Qt::DisplayRole){
        if (index.column() == 0){
            int baseAddress;
            switch (memoryType) {
            case DataSegment:
                baseAddress = memory->dataSegmentBaseAddress;
                break;
            case TextSegment:
                baseAddress = memory->textSegmentBaseAddress;
                break;
            case HeapSegment:
                baseAddress = memory->heapSegmentBaseAddress;
                break;
            case StackSegment:
            default:
                baseAddress = 0; //memory->StackSegmentBaseAddress
                break;
            }
            return baseAddress + 4*index.row();
        }
        else if (index.column() == 1){
            switch(dm){
                 case(Word):

                    /* try{
                        return memory->loadWord(index.row());
                    }catch(std::exception e){
                        qDebug() << e.what();
                        return "-1";
                    }

                    if (index.row() < 16)
                       return memory->loadWord(index.row());
                    else */
                    //qDebug() << index.row();  // I commented this out
                    return "-1";

                    break;
                 case(UnsignedWord):
                    return memory->loadWordU(index.row());
                    break;
                 case (Byte):
                    return memory->loadByte(index.row());
                    break;
                 case (UnsignedByte):
                 default:
                    return memory->loadByteU(index.row());
                    break;
                }
        }
        else
         return QVariant();
    }

        //return "3";
    return QVariant();
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

    if (orientation == Qt::Horizontal)
    {
             switch (section) {
                 case 0:
                     return QVariant("Address");

                 case 1:
                     return QVariant("Value");

                 default:
                     return QVariant();
             }
  }

    return QVariant();
}
Qt::ItemFlags MemoryModel::flags(const QModelIndex &index) const
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


