#include "codeeditor.h"
#include <QDebug>
#include <QAbstractItemView>
#include <QKeyEvent>

CodeEditor::CodeEditor(QWidget *parent) :
    QTextEdit(parent)
{


    SHL = new SyntaxHL(this); //Adding syntax highlighter.

    //Auto-complete setup:
    QStringList compList; //Completion list.
    compList << "add" << "addu" << "sub" << "subu" << "and" << "or" << "xor"
             << "srlv" << "sllv" << "srav" << "slt" << "sltu" << "addi" <<"addiu"
             << "andi" << "ori" << "nori" << "xori" << "srl" << "sll" << "sra"
             << "slti" << "sltiu" << "beq" << "bne" << "lui" << "sb" << "lb" << "lbu"
             << "sh" << "lh" << "lhu" << "sw" << "lw" << "lwl" << "lwr" << "swl"
             << "swr" <<"ll" << "sc" << "jr" << "jalr" << "mfhi" << "mflo"
             << "mthi" << "mtlo" << "mult" << "multu" << "div" << "divu"
             << "j" << "jal" << "syscal" << "nop";

    for (int i = 0; i < 32; i++) compList.append(QString("$" + QString::number(i)));
    compList << "$zero" << "$at" << "$v0" << "$v1" <<"$a0" << "$a1" << "$a2" << "$a3"
             << "$t0" << "$t1" << "$t2" << "$t3" << "$t4" << "$t5" << "$t6" << "$t7"
             << "$s0" << "$s1" << "$s2" << "$s3" << "$s4" << "$s5" << "$s6" << "$s7"
             << "$t8" << "$t9" << "$gp" << "$fp" << "$ra";





    codeCompleter = new QCompleter(compList, this);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWidget(this);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);
   // installEventFilter(this);
    QObject::connect(codeCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(updateCounter()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(completerPop()));
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightLine()));
}


/*
bool CodeEditor::eventFilter(QObject *O, QEvent *e)
{
    if(e->type() == QEvent::KeyPress)
    {
        if(((QKeyEvent*)e)->key() == Qt::Key_Return)
        {
            qDebug() << "-";
           // qDebug() << codeCompleter->popup()->->data(codeCompleter->popup()->indexAt(QPoint(0, 0)));
            e->ignore();
            return true;
        }
        qDebug() << codeCompleter->popup()->model()->data(codeCompleter->popup()->currentIndex());
        return false;
    }

}
*/
void CodeEditor::insertCompletion(QString completion)
{
    QTextCursor currentPos = textCursor();
    int compLength = completion.length() - codeCompleter->completionPrefix().length();
    currentPos.movePosition(QTextCursor::Left);
    currentPos.movePosition(QTextCursor::EndOfWord);
    currentPos.insertText(completion.right(compLength));
    this->setTextCursor(currentPos);
}

void CodeEditor::highlightLine()
{

    QList<QTextEdit::ExtraSelection> linesHL;
    QTextEdit::ExtraSelection lineHL;
    lineHL.format.setBackground(QColor(Qt::lightGray).darker(180));
    lineHL.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineHL.cursor = textCursor();
    lineHL.cursor.clearSelection();
    linesHL.append(lineHL);
    this->setExtraSelections(linesHL);
}


void CodeEditor::updateCounter()
{
    //Line count.
   if(lCounter)
   {
       lCounter->clear();
       lCounter->setAlignment(Qt::AlignCenter);
        int numLines = this->toPlainText().count("\n");
        for (int i = 0; i <= numLines; i++)
           lCounter->append(QString::number(i));

   }
}

void CodeEditor::completerPop()
{

    //qDebug() << this->parent()->objectName();
    //qDebug() << this->parent()->parent()->objectName();
    //qDebug() << this->parent()->parent()->parent()->objectName();

    QTextCursor sel = textCursor();
                sel.select(QTextCursor::WordUnderCursor);
    QTextCursor sel2 = textCursor();
                sel2.select(QTextCursor::LineUnderCursor);

    bool regS = (sel2.selectedText().mid(sel2.selectedText().lastIndexOf(QRegExp("( |,)+")) + 1, 1) == "$");
    if (regS)
        codeCompleter->setCompletionPrefix("$" + sel.selectedText());
    else
        codeCompleter->setCompletionPrefix(sel.selectedText());

    QString currenCom = codeCompleter->completionModel()->data(codeCompleter->model()->index(0, 0)).toString();


    if(sel.selectedText().length() > 0 || regS)
    {
        if ((codeCompleter->completionCount() != 1) || (currenCom != sel.selectedText() && !regS) || (regS && currenCom != "$" + sel.selectedText()))
        {
            QRect popRect = this->cursorRect();
            popRect.setWidth(50);
            codeCompleter->complete(popRect);
            codeCompleter->popup()->setCurrentIndex(codeCompleter->popup()->indexAt(QPoint(0, 0)));
           this->setFocus();
            // codeCompleter->popup()->setFocus();
           // qDebug() << codeCompleter->popup()->currentIndex().data();
        }
        else
            codeCompleter->popup()->hide();
    }
    else
        codeCompleter->popup()->hide();

    //qDebug() << codeCompleter->popup()->isVisible();



}

void CodeEditor::setCounter(QTextEdit *lc)
{
    lCounter = lc;
}
