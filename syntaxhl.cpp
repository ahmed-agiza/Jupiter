#include "syntaxhl.h"
#include <QDebug>

//    add
//    addu
//    sub
//    subu
//    and
//    or
//    nor
//    xor
//    srlv
//    sllv
//    srav
//    slt
//    sltu


//    addi
//    addiu
//    andi
//    ori
//    nori
//    xori
//    srl
//    sll
//    sra
//    slti
//    sltiu
//    beq
//    bne


//  instruction $register, $register, label
//      beq
//      bne


//  instruction $register, immediate
//      lui


//  instruction $register
//      jr
//      jalr
//      mfhi
//      mflo
//      mtlo
//      mthi


//  instruction $register, $register
//      mult
//      multu
//      div
//      divu


//  instruction label
//      j
//      jal


//  instruction
//      syscall
//      nop

SyntaxHL::SyntaxHL(QTextEdit *parent) :
    QSyntaxHighlighter(parent)
{
   //Syntax formats:
   instructionFormat.setForeground(QColor(Qt::blue).lighter(150));
   instructionFormat.setFontWeight(QFont::Bold);
   registerFormat.setForeground(Qt::yellow);
   registerFormat.setFontWeight(QFont::Bold);
   commentFormat.setForeground(Qt::green);
   stringFormat.setForeground(QColor(Qt::darkGreen).lighter(190));
   labelFormat.setForeground(Qt::darkBlue);
   macroFormat.setForeground(QColor(Qt::cyan).lighter(80));
   macroFormat.setFontItalic(true);
   pseudoFormat.setForeground(QColor(Qt::blue).lighter(150));
   pseudoFormat.setFontItalic(true);
   immFormat.setForeground(QColor(Qt::white).darker(110));
   immFormat.setFontWeight(QFont::Bold);

   //Instructions Syntax

   syntax tempSyn;




   tempSyn.pattern = QRegExp("0x[0-9a-fA-F]+|[\\-\\d]+|0b[01]+"); tempSyn.format = immFormat;
   syntaxes.append(tempSyn);

   instructionsList << "add" << "addu" << "sub" << "subu" << "and" << "or" << "xor"
              << "srlv" << "sllv" << "srav" << "slt" << "sltu" << "addi" <<"addiu"
              << "andi" << "ori" << "nori" << "xori" << "srl" << "sll" << "sra"
              << "slti" << "sltiu" << "beq" << "bne" << "lui" << "sb" << "lb" << "lbu"
              << "sh" << "lh" << "lhu" << "sw" << "lw" << "lwl" << "lwr" << "swl"
              <<"swr" <<"ll" << "sc" << "jr" << "jalr" << "mfhi" << "mflo"
              << "mthi" << "mtlo" << "mult" << "multu" << "div" << "divu"
              << "j" << "jal" << "syscall" << "nop";


   foreach(QString pattern, instructionsList)
   {
       tempSyn.pattern = QRegExp(QString("\\b" + pattern + "\\b"), Qt::CaseInsensitive); tempSyn.format = instructionFormat;
       syntaxes.append(tempSyn);
   }

   pseudoList << "blt" <<  "bgt"
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
                      // <<  "div"
                       <<  "rem"
                       <<  "clear"
                       <<  "subi"
                       <<  "la";



   foreach(QString pattern, pseudoList)
   {
       tempSyn.pattern = QRegExp(QString("\\b" + pattern + "\\b"), Qt::CaseInsensitive); tempSyn.format = pseudoFormat;
       syntaxes.append(tempSyn);
   }


   //Registers syntax.
   for (int i = 0; i < 32; i++)
        registersList.append(QString("\\$" + QString::number(i)));
   registersList << "\\$zero" << "\\$at" << "\\$v0" << "\\$v1" <<"\\$a0" << "\\$a1" << "\\$a2" << "\\$a3"
                 << "\\$t0" << "\\$t1" << "\\$t2" << "\\$t3" << "\\$t4" << "\\$t5" << "\\$t6" << "\\$t7"
                 << "\\$s0" << "\\$s1" << "\\$s2" << "\\$s3" << "\\$s4" << "\\$s5" << "\\$s6" << "\\$s7"
                 << "\\$t8" << "\\$t9" << "\\$gp" << "\\$fp" << "\\$ra";



   foreach(QString pattern, registersList)
   {
       tempSyn.pattern = QRegExp(pattern, Qt::CaseInsensitive); tempSyn.format = registerFormat;
       syntaxes.append(tempSyn);
   }

   macrosList << "\\.align" << "\\.ascii" << "\\.asciiz" << "\\.byte" << "\\.double" <<"\\.float" << "\\.half" << "\\.space" << "\\.word" << "\\.text" << "\\.data" << "\\.include";

   foreach(QString pattern, macrosList)
   {
       tempSyn.pattern = QRegExp(pattern, Qt::CaseInsensitive); tempSyn.format = macroFormat;
       syntaxes.append(tempSyn);
   }

   tempSyn.pattern = QRegExp("\\\".*\\\"", Qt::CaseInsensitive);
   tempSyn.format = stringFormat;
   syntaxes.append(tempSyn);

   //Comment's syntax.
   tempSyn.pattern = QRegExp("#[^\n]*"); tempSyn.format = commentFormat;
   syntaxes.append(tempSyn);


}

void SyntaxHL::highlightBlock(const QString &text)
{
    foreach (syntax tempSyn, syntaxes)
    {
             QRegExp formatRegEx(tempSyn.pattern);
             int index = formatRegEx.indexIn(text);
             while (index >= 0) {
                 int length = formatRegEx.matchedLength();
                 setFormat(index, length, tempSyn.format);
                 index = formatRegEx.indexIn(text, index + length);                
             }
   }
}
