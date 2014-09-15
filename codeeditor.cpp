#include "codeeditor.h"
#include <QDebug>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QListView>
#include <QTextDocumentFragment>

#include "completerlist.h"

//#define PlusR + QString(" $rd, $rs, $rt")
#define PlusR + QString("")
#define PlusI + QString(" $rt, $rs, immediate")
#define PlusJ + QString(" destination")



CodeEditor::CodeEditor(QWidget *parent) :
    QTextEdit(parent)
{


    SHL = new SyntaxHL(this); //Adding syntax highlighter.

    //Auto-complete setup:

    compList << "add"
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
             <<  "la"
             << ".align" << ".ascii" << ".asciiz" << ".byte" << ".double" <<".float" << ".half" << ".space" << ".word";

    lCounter = NULL;

    model = new QStringListModel(compList, this);

    codeCompleter = new QCompleter(model, this);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);
    CompleterList *cl = new CompleterList(this);
    codeCompleter->setPopup(cl);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWidget(this);

    QObject::connect(codeCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    //QObject::connect(this, SIGNAL(labelsUpdated()), SHL, SLOT(rehighlight()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(updateLabels()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(updateCounter()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(completerPop()));    
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightLine()));
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightLine()));
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateCounterFormat()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(updateCounterFormat()));
    selectionStart = selectionEnd = 0;
}



void CodeEditor::focusInEvent(QFocusEvent *e){
    if (codeCompleter)
        codeCompleter->setWidget(this);
    QTextEdit::focusInEvent(e);
}



void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if ((codeCompleter) && codeCompleter->popup()->isVisible() && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Backtab || e->key() == Qt::Key_Escape|| e->key() == Qt::Key_Return || e->key() == Qt::Key_Tab))
    {
        e->ignore();
        return;
    }
    else if (e->key() == Qt::Key_Tab)
    {
        QTextCursor cur = textCursor();
        cur.beginEditBlock();
        cur.insertText("\t");
        cur.endEditBlock();
        setTextCursor(cur);
        e->ignore();
        return;
    }
    else if (e->key() == Qt::Key_D && (e->modifiers() & Qt::ControlModifier)){
        deleteLine();
    } else if (e->key() == Qt::Key_Up && (e->modifiers() & Qt::AltModifier)){
        copyLineUp();
    }else if (e->key() == Qt::Key_Up && (e->modifiers() & Qt::ControlModifier)) {
        moveLineUp();
    } else if (e->key() == Qt::Key_Down && (e->modifiers() & Qt::AltModifier)){
        copyLineDown();
    }else if (e->key() == Qt::Key_Down && (e->modifiers() & Qt::ControlModifier)){
        moveLineDown();
    }else if (e->key() == Qt::Key_Space &&(e->modifiers() & Qt::ControlModifier)){
        popupSuggestions();

    }else if (e->key() == Qt::Key_3 && (e->modifiers() & Qt::ControlModifier)){
        toggleComments();
    }else if(e->key() == Qt::Key_Period){
         popupSuggestions();
         QTextEdit::keyPressEvent(e);
    }else
        QTextEdit::keyPressEvent(e);


}

QTextCursor CodeEditor::deleteCurrentLine()
{
    QTextCursor currentPos = textCursor();
    textCursor().beginEditBlock();
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

QString CodeEditor::getCurrentLine(){
    QTextCursor currentPos = textCursor();
    currentPos.select(QTextCursor::LineUnderCursor);
    return currentPos.selectedText();
}

QTextCursor CodeEditor::getSelectedLines(){
    QTextCursor startCursor = textCursor();
    QTextCursor endCursor = textCursor();
    int selStart = startCursor.selectionStart();
    int selEnd = endCursor.selectionEnd();
    startCursor.setPosition(selStart);
    endCursor.setPosition(selEnd);
    startCursor.movePosition(QTextCursor::StartOfLine);
    endCursor.movePosition(QTextCursor::EndOfLine);
    QTextCursor selectionCursor = textCursor();
    selectionCursor.clearSelection();
    selectionCursor.setPosition(startCursor.position());
    selectionCursor.setPosition(endCursor.position(), QTextCursor::KeepAnchor);
    return selectionCursor;
}

void CodeEditor::deleteSelection(){
    QTextCursor currentPos = textCursor();
    currentPos.removeSelectedText();

}

void CodeEditor::deleteLine(){
    QTextCursor selection = getSelectedLines();
    selection.beginEditBlock();
    selection.removeSelectedText();
    selection.deleteChar();
    selection.endEditBlock();
}

void CodeEditor::moveLineUp(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.removeSelectedText();
    selectionCursor.deleteChar();
    selectionCursor.movePosition(QTextCursor::Up);
    selectionCursor.insertText(QString(selectedLines + "\n"));
    selectionCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, selectedLines.split("\n").size());
    selectionCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();
}

void CodeEditor::moveLineDown(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.removeSelectedText();
    selectionCursor.deleteChar();
    selectionCursor.movePosition(QTextCursor::EndOfLine);
    selectionCursor.insertText(QString("\n" + selectedLines));
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();

}

void CodeEditor::copyLineUp(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.setPosition(selectionCursor.selectionStart());
    selectionCursor.movePosition(QTextCursor::StartOfLine);
    selectionCursor.insertText(QString(selectedLines + "\n"));
    selectionCursor.movePosition(QTextCursor::PreviousCharacter);
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();
}

void CodeEditor::copyLineDown(){
    QTextCursor selectionCursor = getSelectedLines();
    QString selectedLines = selectionCursor.selection().toPlainText();
    selectionCursor.beginEditBlock();
    selectionCursor.movePosition(QTextCursor::EndOfLine);
    selectionCursor.insertText(QString("\n" + selectedLines));
    selectionCursor.movePosition(QTextCursor::EndOfBlock);
    selectionCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, selectedLines.size());
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
    if(codeCompleter->popup()->isVisible())
        codeCompleter->popup()->hide();
}

void CodeEditor::popupSuggestions(){
    completerPop();
}

void CodeEditor::toggleComments(){
    QTextCursor selectionCursor = getSelectedLines();
    QStringList selectedLines = selectionCursor.selection().toPlainText().split("\n");
    selectionCursor.beginEditBlock();
    selectionCursor.removeSelectedText();
    selectionCursor.deleteChar();
    for (int i = 0; i < selectedLines.size(); i++){
        if (selectedLines.at(i).trimmed().size() > 0){
            if(!selectedLines.at(i).startsWith("#"))
                selectedLines[i] = "#" + selectedLines[i];
            else
                selectedLines[i].remove(0, 1);
        }
    }
    foreach(QString line, selectedLines)
        selectionCursor.insertText(line + "\n");
    selectionCursor.movePosition(QTextCursor::PreviousCharacter);
    selectionCursor.endEditBlock();
    setTextCursor(selectionCursor);
}

void CodeEditor::insertCompletion(QString completion){
    QTextCursor currentPos = textCursor();
    int compLength = completion.length() - codeCompleter->completionPrefix().length();
    currentPos.movePosition(QTextCursor::Left);
    currentPos.movePosition(QTextCursor::EndOfWord);
    currentPos.insertText(completion.right(compLength));
    setTextCursor(currentPos);
    codeCompleter->popup()->hide();
}

void CodeEditor::highlightLine(){

    QList<QTextEdit::ExtraSelection> linesHL;
    QTextEdit::ExtraSelection lineHL;
    lineHL.format.setBackground(QColor(Qt::lightGray).darker(180));
    lineHL.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineHL.cursor = textCursor();

    lineHL.cursor.clearSelection();
    linesHL.append(lineHL);
    setExtraSelections(linesHL);
}

void CodeEditor::updateCounterFormat(){
    if (lCounter != NULL){
        QTextCursor selectedCur = getSelectedLines();
        int start = toPlainText().mid(0, selectedCur.selectionStart()).count("\n");
        int end = toPlainText().mid(0, selectedCur.selectionEnd()).count("\n");
        if (start > end)
            lCounter->boldLines(end, start);
        else
            lCounter->boldLines(start, end);
    }
}

void CodeEditor::updateLabels(){
    static QRegExp labelsRegEx("(\\S+)(?=:)");
    QStringList contentList = toPlainText().split("\n");

    labelsList.clear();
    foreach(QString line, contentList){
        if (line.trimmed() == "")
            continue;
        labelsRegEx.indexIn(line);
        labelsList << labelsRegEx.cap(0);
    }
    labelsList.removeDuplicates();
    labelsList.removeAll("");

    model->setStringList(compList + labelsList);
    SHL->setLabelsList(labelsList);
    blockSignals(true);
    SHL->rehighlight();
    blockSignals(false);

}

void CodeEditor::selectLine(int num){
    QTextCursor cur = textCursor();
    cur.clearSelection();
    cur.setPosition(0);
    cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, num);
    cur.select(QTextCursor::LineUnderCursor);
    setTextCursor(cur);
}

void CodeEditor::startSelection(int lineNum){
    //qDebug() << "Start " << lineNum;
    selectionStart = lineNum;
}

void CodeEditor::addSelectedLines(int lineNum){
    //qDebug() << "Add: " << lineNum;
    selectionEnd = lineNum;
    QTextCursor cur = textCursor();
    cur.setPosition(0);
    cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, selectionStart);
    if (selectionStart == lineNum)
        cur.select(QTextCursor::LineUnderCursor);
    else if (selectionStart > lineNum)
        cur.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor, selectionStart - lineNum);
    else if (selectionStart < lineNum)
        cur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, lineNum - selectionStart);
    setTextCursor(cur);

}

void CodeEditor::endSelection(int lineNum){
    //qDebug() << "End: " << lineNum;
    if (lineNum == selectionStart)
        selectLine(lineNum);
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
    if (sel2.selection().toPlainText().trimmed().startsWith("#")){
        codeCompleter->popup()->hide();
        return;
    }
    if (sel2.selection().toPlainText().contains("\"") && sel2.selection().toPlainText().count("\"") % 2 != 0){
        QTextCursor stringChecker(sel);
        stringChecker.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        QString firstHalf = sel2.selection().toPlainText().mid(0, stringChecker.selectionStart());
        if (firstHalf.count("\"")%2 != 0){
            codeCompleter->popup()->hide();
            return;
        }


    }


    bool regS = (sel2.selectedText().mid(sel2.selectedText().lastIndexOf(QRegExp("( |,)+")) + 1, 1) == "$");

    QTextCursor tempCursor = textCursor();
    tempCursor.select(QTextCursor::WordUnderCursor);
    if (tempCursor.selectionStart() != 0){
        int length = tempCursor.selection().toPlainText().length();
        tempCursor.setPosition(tempCursor.selectionStart() - 1);
        tempCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, length == 0? 1:length);
    }

    QRegExp directivesRegEx("\\.\\w*");
    directivesRegEx.setPatternSyntax(QRegExp::RegExp2);



    bool dirS = tempCursor.selectedText().lastIndexOf(directivesRegEx) != -1;


    if (regS)
        codeCompleter->setCompletionPrefix("$" + sel.selectedText());
    else if (dirS){
        codeCompleter->setCompletionPrefix("." + sel.selectedText());
    }else
        codeCompleter->setCompletionPrefix(sel.selectedText());


    QString currenCom = codeCompleter->completionModel()->data(codeCompleter->model()->index(0, 0)).toString();


    if(sel.selectedText().length() > 0 || regS || dirS)
    {
        if ((codeCompleter->completionCount() != 1) || (currenCom != sel.selectedText() && !regS && !dirS) || (regS && currenCom != "$" + sel.selectedText()) || (dirS && currenCom != "." + sel.selectedText()))
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

void CodeEditor::setCounter(LinesCounter *lc){
    lCounter = lc;
}

void CodeEditor::setOpened(){
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
    codeCompleter->popup()->hide();
}
