#include "memorymodel.h"

const QString WORD("Word");
const QString UWORD("Unsigned Word");
const QString BYTE("Byte");
const QString UBYTE("Unsigned Byte");
const QString B10("Decimal");
const QString B16("Hexadecimal");
const QString B02("Binary");
const QString BAS("Ascii");

QString getPaddedBinary(int number, int padding);
QString getPaddedHex(int number, int padding);



MemoryModel::MemoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{}




MemoryModel::MemoryModel(Memory *m, QObject *parent=0, MemorySegment ms = DataSegment)
    : QAbstractTableModel(parent), memory(m), memoryType(ms)
{
    int totalSize = initMemorySegment();
    updateCount(totalSize);
}

MemoryModel::MemoryModel(Memory *m, QObject *parent, MemorySegment ms, QComboBox *ab, QComboBox *mm, QComboBox *mb)
    : QAbstractTableModel(parent), memory(m), memoryType(ms), addressBase(ab), memoryMode(mm), memoryBase(mb)
{
    int totalSize = initMemorySegment();
    updateCount(totalSize);
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
    case TMSegment:
        totalSize = memory->getTilemapSegmentSize();
        baseAddress = memory->tileMapBaseAddress;
        break;
    case BGTSSegment:
        totalSize = memory->getBGTilesetSegmentSize();
        baseAddress = memory->backgroundTileSetBaseAddress;
        break;
    case SPTSSegment:
        totalSize = memory->getSPTilesetSegmentSize();
        baseAddress = memory->spritesTileSetBaseAddress;
        break;
    case SPRamSegment:
        totalSize = memory->getSPRAMSegmentSize();
        baseAddress = memory->spriteRamBaseAddress;
        break;
    case PaletteSegment:
        totalSize = memory->getPaletteSegmentSize();
        baseAddress = memory->paletteBaseAddress;
        break;
    case InputSegment:
        totalSize = memory->getInputSegmentSize();
        baseAddress = memory->inputMemoryBaseAddress;
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
                return "0x" + getPaddedHex(adr, 32);
            else if (addressBase->currentText() == B02)
                return "0b" + getPaddedBinary(adr, 32);
        }
        else if (index.column() == 1){
            int value;
            unsigned int uvalue;
            if (memoryMode->currentText() == UWORD){
                uvalue = memory->loadWordU(adr);
                if (memoryBase->currentText() == B16){


                    if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD){
                        return "0x" + getPaddedHex(uvalue, 32);
                    }else{
                        return "0x" + getPaddedHex(uvalue, 8);
                    }
                }


            }else if (memoryMode->currentText() == BYTE){
                value = memory->loadByte(adr);
            }else if (memoryMode->currentText() == UBYTE){
                uvalue = memory->loadByteU(adr);
                if (memoryBase->currentText() == B16){


                    if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD){
                        return "0x" + getPaddedHex(uvalue, 32);
                    }else{
                        return "0x" + getPaddedHex(uvalue, 8);
                    }
                }
            }else{
                value = memory->loadWord(adr);
                //if (value)
            }
            if (memoryBase->currentText() == B16){
                if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD){
                    return "0x" + getPaddedHex(value, 32);
                }else{
                    return "0x" + getPaddedHex(value, 8); //+ untrimmed.mid(untrimmed.size() - 8).toUpper();
                }
            }else if (memoryBase->currentText() == B02){
                if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD)
                    return "0b" + getPaddedBinary(value, 32);
                else
                    return "0b" + getPaddedBinary(value, 8);
            }else if (memoryBase->currentText() == BAS){
                if (memoryMode->currentText() == WORD || memoryMode->currentText() == UWORD){
                    //QByteArray strByte(4, ' ');
                    char value3 = ((value >> 24) & 0xff);
                    char value2 = ((value >> 16) & 0xff);
                    char value1 = ((value >> 8) & 0xff);
                    char value0 = (value & 0xff);
                    QString charac3 = (value3 == 0)? "\\0":QString(value3);
                    QString charac2 = (value2 == 0)? "\\0":QString(value2);
                    QString charac1 = (value1 == 0)? "\\0":QString(value1);
                    QString charac0 = (value0 == 0)? "\\0":QString(value0);

                    QString mText = charac3 + charac2 + charac1 + charac0;

                    return "\"" + mText + "\"";
                }else{
                    QByteArray strByte(1, ' ');
                    strByte[0] = (char) value;
                    if (strByte[0] == '\0')
                        return "\\0";
                    else
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

void MemoryModel::setModifiers(QComboBox *ab, QComboBox *mm, QComboBox *mb){
    addressBase = ab;
    memoryMode = mm;
    memoryBase = mb;

}

void MemoryModel::emitDataChanged(){
    emit dataChanged(QModelIndex(), QModelIndex());
}

