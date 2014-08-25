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

    //for (int i = 0; i < 32; i++) compList.append(QString("$" + QString::number(i)));
    compList << "$0" << "$zero" << "$at" << "$v0" << "$v1" <<"$a0" << "$a1" << "$a2" << "$a3"
             << "$t0" << "$t1" << "$t2" << "$t3" << "$t4" << "$t5" << "$t6" << "$t7"
             << "$s0" << "$s1" << "$s2" << "$s3" << "$s4" << "$s5" << "$s6" << "$s7"
             << "$t8" << "$t9" << "$gp" << "$fp" << "$ra";





    codeCompleter = new QCompleter(compList, this);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWidget(this);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    QObject::connect(codeCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(updateCounter()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(completerPop()));
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightLine()));
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightLine()));
}



void CodeEditor::focusInEvent(QFocusEvent *e)
 {
     if (codeCompleter)
         codeCompleter->setWidget(this);
     QTextEdit::focusInEvent(e);
 }



void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if ((codeCompleter) && codeCompleter->popup()->isVisible() && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Tab))
    {
        e->ignore();
        return;
    }
    else if (e->key() == Qt::Key_Tab)
    {
        this->insertPlainText("    ");
        e->ignore();
        return;
    }
    else if (e->key() == Qt::Key_D && (e->modifiers() & Qt::ControlModifier)){
            qDebug() << "Delete line.";
            deleteCurrentLine();

    } else if (e->key() == Qt::Key_Up && (e->modifiers() & Qt::AltModifier)){
            qDebug() << "Copy line up.";
            QTextCursor currentPos = textCursor();
            textCursor().setKeepPositionOnInsert(true);
            currentPos.select(QTextCursor::LineUnderCursor);
            QString line = currentPos.selectedText();
            currentPos = textCursor();
            currentPos.movePosition(QTextCursor::EndOfLine);
            currentPos.insertText('\n' + line);
            currentPos.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, line.length() + 1);
            setTextCursor(currentPos);

    }else if (e->key() == Qt::Key_Up && (e->modifiers() & Qt::ControlModifier)) {
            qDebug() << "Move line up.";
            QString line = getCurrentLine();
            QTextCursor currentPos = deleteCurrentLine();
            currentPos.movePosition(QTextCursor::StartOfLine);
            currentPos.insertText(line + '\n');
            currentPos.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, line.length() + 1);
            setTextCursor(currentPos);

    } else if (e->key() == Qt::Key_Down && (e->modifiers() & Qt::AltModifier)){
            qDebug() << "Copy line down.";
            QTextCursor currentPos = textCursor();
            textCursor().setKeepPositionOnInsert(true);
            currentPos.select(QTextCursor::LineUnderCursor);
            QString line = currentPos.selectedText();
            currentPos = textCursor();
            currentPos.movePosition(QTextCursor::EndOfLine);
            currentPos.insertText('\n' + line);
            currentPos.movePosition(QTextCursor::EndOfBlock);
            setTextCursor(currentPos);
    }else if (e->key() == Qt::Key_Down && (e->modifiers() & Qt::ControlModifier)){
            qDebug() << "Move line down.";
            QString line = getCurrentLine();
            QTextCursor cursCopy = textCursor();
            QTextCursor currentPos = deleteCurrentLine();
            //QTextCursor currentPos = textCursor();
            currentPos.movePosition(QTextCursor::EndOfLine);
            if (!cursCopy.selectionStart() == 0)
                currentPos.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, line.length() + 1);
            currentPos.insertText('\n' + line);
            setTextCursor(currentPos);
    }else if (e->key() == Qt::Key_Space &&(e->modifiers() & Qt::ControlModifier)){
            qDebug() << "Pop suggestions";
            QRect popRect = this->cursorRect();
            popRect.setWidth(50);
            codeCompleter->complete(popRect);
            codeCompleter->setCurrentRow(3);
            codeCompleter->popup()->setCurrentIndex(codeCompleter->popup()->indexAt(QPoint(0, 0)));
    }else if (e->key() == Qt::Key_3 && (e->modifiers() & Qt::ControlModifier)){
        QTextCursor currentPos = textCursor();
        currentPos.movePosition(QTextCursor::StartOfLine);
        QTextCursor tempCursor(currentPos);
        tempCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        if (tempCursor.selectedText() == "#")
            tempCursor.removeSelectedText();
        else
             currentPos.insertText("#");
    } else
        QTextEdit::keyPressEvent(e);




}

QTextCursor CodeEditor::deleteCurrentLine()
{
    QTextCursor currentPos = textCursor();
    currentPos.select(QTextCursor::BlockUnderCursor);

    if (currentPos.selectedText() == ""){
        if (currentPos.selectionStart() == 0){
            currentPos.deleteChar();
        }else{
            currentPos.deletePreviousChar();
         }
        currentPos.removeSelectedText();
    }
    else{
        currentPos.removeSelectedText();
        if (currentPos.selectionStart() == 0)
            currentPos.deleteChar();
    }
    return currentPos;
}

QString CodeEditor::getCurrentLine()
{
    QTextCursor currentPos = textCursor();
    currentPos.select(QTextCursor::LineUnderCursor);
    return currentPos.selectedText();
}

void CodeEditor::insertCompletion(QString completion)
{
    QTextCursor currentPos = textCursor();
    int compLength = completion.length() - codeCompleter->completionPrefix().length();
    currentPos.movePosition(QTextCursor::Left);
    currentPos.movePosition(QTextCursor::EndOfWord);
    currentPos.insertText(completion.right(compLength));
    setTextCursor(currentPos);
    codeCompleter->popup()->hide();
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
    setExtraSelections(linesHL);
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
            codeCompleter->setCurrentRow(3);
            codeCompleter->popup()->setCurrentIndex(codeCompleter->popup()->indexAt(QPoint(0, 0)));
        }
        else
            codeCompleter->popup()->hide();
    }
    else
        codeCompleter->popup()->hide();


}

void CodeEditor::setCounter(QTextEdit *lc)
{
    lCounter = lc;
}
