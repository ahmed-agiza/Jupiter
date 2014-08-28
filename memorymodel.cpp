#include "memorymodel.h"

const QString WORD("Word");
const QString UWORD("Unsigned Word");
const QString BYTE("Byte");
const QString UBYTE("Unsigned Byte");
const QString B10("Decimal");
const QString B16("Hexadecimal");
const QString B02("Binary");
const QString BAS("Ascii");



MemoryModel::MemoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{}




MemoryModel::MemoryModel(Memory *m, QObject *parent=0, MemorySegment ms = DataSegment)
    : QAbstractTableModel(parent), memory(m), memoryType(ms)
{
    qDebug() << "Constructing..";

    qDebug() << memory;
    int totalSize = initMemorySegment();
    updateCount(totalSize);

    qDebug() << "Contructed..";
}

MemoryModel::MemoryModel(Memory *m, QObject *parent, MemorySegment ms, QComboBox *ab, QComboBox *mm, QComboBox *mb)
    : QAbstractTableModel(parent), memory(m), memoryType(ms), addressBase(ab), memoryMode(mm), memoryBase(mb)
{
    qDebug() << "Constructing..";

    qDebug() << memory;
    int totalSize = initMemorySegment();
    updateCount(totalSize);
    qDebug() << "Data: " <<memory->getDataSegmentSize();
    qDebug() << "Text: " <<memory->getTextSegmentSize();
    qDebug() << "Stack: " <<memory->getStackSegmentSize();
    qDebug() << "Heap: " <<memory->getHeapSegmentSize();
    qDebug() << "Contructed.." << rCount;
}





MemoryModel::~MemoryModel()
{}

int MemoryModel::initMemorySegment()
{
    int totalSize;
    switch (memoryType) {
    case DataSegment:
        totalSize = memory->getDataSegmentSize();
        baseAddress = memory->dataSegmentBaseAddress;
        break;
    case TextSegment:
        totalSize = memory->getTextSegmentSize();
        baseAddress = memory->textSegmentBaseAddress;
        break;
    case HeapSegment:
        totalSize = memory->getHeapSegmentSize();
        baseAddress = memory->heapSegmentBaseAddress;
        break;
    case StackSegment:
    default:
        baseAddress = memory->heapSegmentBaseAddress;;
        totalSize = memory->getStackSegmentSize();
        break;
    }
    return totalSize;
}

void MemoryModel::updateCount(int totalSize)
{
    if (memoryMode->currentText() == BYTE || memoryMode->currentText() == UBYTE){
        rCount = totalSize;
    }else{
        rCount = totalSize / 4;
    }
}


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
    if (!index.isValid() || index.row() >= rCount || index.row() < 0 || index.column() < 0 || index.column() > 1)
        return QVariant();

    if (role == Qt::DisplayRole){
         int adr;
         int rowNumber;
         if (memoryType == StackSegment)
             rowNumber = rCount - 1 - index.row();
         else
             rowNumber = index.row();
         if (memoryMode->currentText() == BYTE || memoryMode->currentText() == UBYTE){
             adr = baseAddress + rowNumber;
         }else{
             adr = baseAddress + 4*rowNumber;
         }

         if (index.column() == 0){
            if(addressBase->currentText() == B10)
                return adr;
            else if (addressBase->currentText() == B16)
                return "0x" + QString::number(adr, 16).toUpper();
            else if (addressBase->currentText() == B02)
                return "0b" + QString::number(adr, 2);
        }
        else if (index.column() == 1){
            int value;
            if (memoryMode->currentText() == UWORD)
                value = memory->loadWordU(adr);
            else if (memoryMode->currentText() == BYTE)
                value = memory->loadByte(adr);
            else if (memoryMode->currentText() == UBYTE)
                value = memory->loadByteU(adr);
            else
                value = memory->loadWord(adr);
            if (memoryBase->currentText() == B16){
                if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD){
                    return "0x" + QString::number(value, 16).toUpper();
                }else{
                    QString untrimmed = QString::number(value, 16);
                    return "0x" + untrimmed.mid(untrimmed.size() -8).toUpper();
                }
            }else if (memoryBase->currentText() == B02){
                return "0b" + QString::number(value, 2);
            }else if (memoryBase->currentText() == BAS){
                if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD){
                    QByteArray strByte(4, ' ');
                    strByte[3] = ((value >> 24) & 0xff);
                    strByte[2] = ((value >> 16) & 0xff);
                    strByte[1] = ((value >> 8) & 0xff);
                    strByte[0] = (value & 0xff);
                    QString mText = QString::fromLatin1(strByte);
                    return "\"" + mText + "\"";
                }else{
                    QByteArray strByte(1, ' ');
                    strByte[0] = (char) value;
                    return "'" + QString::fromLatin1(strByte) + "'";
                }
            }else
                return value;
        }
        else
            return QVariant();
    }

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

void MemoryModel::setModifiers(QComboBox *ab, QComboBox *mm, QComboBox *mb)
{
    addressBase = ab;
    memoryMode = mm;
    memoryBase = mb;
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


