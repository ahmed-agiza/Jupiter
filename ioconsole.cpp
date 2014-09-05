#include "ioconsole.h"
#include <QDebug>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>

IOConsole::IOConsole(QWidget *parent) :
    QTextEdit(parent){
    installEventFilter(this);
    lockPosition = 0;
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorChanged()));
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
    selectionLocker = false;
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void IOConsole::setLock(int pos){
    lockPosition = pos;
    qDebug() << "Locked at " << lockPosition;
}

void IOConsole::addText(QString text, bool ro = true){
    setText(toPlainText() + text);
    lockPosition += text.length();
    setReadOnly(ro);
    qDebug() << "Locked at " << lockPosition;
    inputTrace.append(text);
}

int IOConsole::getLockPosition(){
    return lockPosition;
}

void IOConsole::setLockAtEnd(){
    lockPosition = toPlainText().length();
    qDebug() << "Locked at " << lockPosition;
}

QString IOConsole::getInputAt(int index){
    if (index < inputTrace.length())
        return inputTrace.at(index);
    return "";
}

void IOConsole::clearInputAt(int index){
    if (index < inputTrace.length())
        inputTrace.removeAt(index);
}

void IOConsole::reprint(){
    clear();
    foreach(QString line, inputTrace)
        append(line + QString("\n"));
}

bool IOConsole::eventFilter(QObject *o, QEvent *e){
    return QTextEdit::eventFilter(o, e);
}

void IOConsole::keyPressEvent(QKeyEvent *e){

    if (e->key() == Qt::Key_Return){
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfBlock);
        cursor.setPosition(lockPosition, QTextCursor::KeepAnchor);
        QString inputText = cursor.selectedText();
        emit dataInput(inputText);
        qDebug() << inputText;
        setReadOnly(true);
        setLock(lockPosition + inputText.length());
        inputTrace.append(inputText);

    }else if (e->key() == Qt::Key_Backspace){
        if (textCursor().position() <= lockPosition){
            e->ignore();
            return;
        }
    }
    QTextEdit::keyPressEvent(e);
}

void IOConsole::enableEditing(bool ro){
    setReadOnly(ro);
}

void IOConsole::getInput(){
    setReadOnly(true);
}

void IOConsole::clearConsole(){
    clear();
    lockPosition = 0;
}

void IOConsole::copyAll(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(toPlainText());
}

void IOConsole::onCursorChanged(){
    QTextCursor cursor = textCursor();
    int currentPosition = cursor.position();
    if(currentPosition < lockPosition)
        cursor.setPosition(lockPosition);
    setTextCursor(cursor);
}

void IOConsole::onSelectionChanged(){
    QTextCursor cursor = textCursor();
    int selectionStart = cursor.selectionStart();
    int selectionEnd = cursor.selectionEnd();
    if (selectionStart < lockPosition){
        if (selectionEnd > lockPosition){
            cursor.setPosition(selectionEnd);
            cursor.setPosition(lockPosition, QTextCursor::KeepAnchor);
        }else
            cursor.setPosition(lockPosition);
    }
    if (!selectionLocker){
        selectionLocker = true;
        setTextCursor(cursor);
        selectionLocker = false;
    }
}

\
