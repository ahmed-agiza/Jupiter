#include "syntaxhl.h"
#include <QDebug>

SyntaxHL::SyntaxHL(QTextEdit *parent) :
    QSyntaxHighlighter(parent)
{
   //Syntax formats:
   instructionFormat.setForeground(Qt::darkRed);
   instructionFormat.setFontWeight(QFont::Bold);
   registerFormat.setForeground(Qt::blue);
   commentFormat.setForeground(Qt::darkGreen);
   stringFormat.setForeground(Qt::green);
   labelFormat.setForeground(Qt::darkBlue);
   macroFormat.setForeground(Qt::magenta);


    //Instructions Syntax
   instructionsList << "\\badd\\b" << "\\baddu\\b" << "\\bsub\\b";
   syntax tempSyn;

   foreach(QString pattern, instructionsList)
   {
       tempSyn.pattern = QRegExp(pattern); tempSyn.format = instructionFormat;
       syntaxes.append(tempSyn);
   }


   //Registers syntax.
   for (int i = 0; i < 32; i++)
        registersList.append(QString("\\$" + QString::number(i)));


   foreach(QString pattern, registersList)
   {
       tempSyn.pattern = QRegExp(pattern); tempSyn.format = registerFormat;
       syntaxes.append(tempSyn);
   }

   //Comment's syntax.

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
