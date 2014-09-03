#include "completerlist.h"
#include <QStringListModel>
#include <QPainter>
#include <QDebug>

CompleterList::CompleterList(QWidget *parent) :
    QListView(parent){


    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAutoFillBackground(true);
    setMinimumWidth(70);

    /*QStringList compList; //Completion list.
    compList << "aaa" //"add"
             << "addu"
             << "sub"
             << "subu"
             << "and"
             << "or"
             << "xor"
             << "srlv"
             << "sllv"
             << "srav"
             << "slt"
             << "sltu"
             << "addi"
             << "addiu"
             << "andi"
             << "ori"
             << "nori"
             << "xori"
             << "srl"
             << "sll"
             << "sra"
             << "slti"
             << "sltiu"
             << "beq"
             << "bne"
             << "lui"
             << "sb"
             << "lb"
             << "lbu"
             << "sh"
             << "lh"
             << "lhu"
             << "sw"
             << "lw"
             << "lwl"
             << "lwr"
             << "swl"
             << "swr"
             <<"ll"
             << "sc"
             << "jr"
             << "jalr"
             << "mfhi"
             << "mflo"
             << "mthi"
             << "mtlo"
             << "mult"
             << "multu"
             << "div"
             << "divu"
             << "j"
             << "jal"
             << "syscall"
             << "nop";

    //for (int i = 0; i < 32; i++) compList.append(QString("$" + QString::number(i)));
    compList << "$0"
             << "$zero"
             << "$at"
             << "$v0"
             << "$v1"
             <<"$a0"
             << "$a1"
             << "$a2"
             << "$a3"
             << "$t0"
             << "$t1"
             << "$t2"
             << "$t3"
             << "$t4"
             << "$t5"
             << "$t6"
             << "$t7"
             << "$s0"
             << "$s1"
             << "$s2"
             << "$s3"
             << "$s4"
             << "$s5"
             << "$s6"
             << "$s7"
             << "$t8"
             << "$t9"
             << "$gp"
             << "$fp"
             << "$ra";

    compList << "blt"  <<  "bgt"
                        <<  "ble"
                        <<  "bge"
                        <<  "bltu"
                        <<  "bgtu"
                        <<  "bleu"
                        <<  "bgeu"
                        <<  "blti"
                        <<  "bgti"
                        <<  "blei"
                        <<  "bgei"
                        <<  "bltiu"
                        <<  "bgtiu"
                        <<  "bleiu"
                        <<  "bgeiu"
                        <<  "beqz"
                        <<  "bnez"
                        <<  "bltz"
                        <<  "bgtz"
                        <<  "blez"
                        <<  "bgez"
                        <<  "li"
                        <<  "ror"
                        <<  "rol"
                        <<  "not"
                        <<  "neg"
                        <<  "move"
                        <<  "abs"
                        <<  "mul"
                        <<  "div"
                        <<  "rem"
                        <<  "clear"
                        <<  "subi"
                        <<  "la";


    QStringListModel *model = new QStringListModel(compList, this);
    setModel(model);*/

}

void CompleterList::paintEvent(QPaintEvent *e){
    QListView::paintEvent(e);

}


