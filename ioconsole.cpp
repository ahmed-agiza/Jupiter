#include "ioconsole.h"
#include <QDebug>
#include <QKeyEvent>

IOConsole::IOConsole(QWidget *parent) :
    QTextEdit(parent){
    installEventFilter(this);
    lockPosition = 0;
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorChanged()));
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

void IOConsole::setLock(int pos){
    lockPosition = pos;
    qDebug() << "Locked at " << lockPosition;
}

void IOConsole::addText(QString text){
    setText(toPlainText() + text);
    lockPosition += text.length();
    qDebug() << "Locked at " << lockPosition;
}

int IOConsole::getLockPosition(){
    return lockPosition;
}

void IOConsole::setLockAtEnd(){
    lockPosition = toPlainText().length();
    qDebug() << "Locked at " << lockPosition;
}

bool IOConsole::eventFilter(QObject *o, QEvent *e){


    return QTextEdit::eventFilter(o, e);
}

void IOConsole::keyPressEvent(QKeyEvent *e){

    if (e->key() == Qt::Key_Return){
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfBlock);
        cursor.setPosition(lockPosition, QTextCursor::KeepAnchor);
        emit dataInput(cursor.selectedText());
        qDebug() << cursor.selectedText();
        setLock(lockPosition + cursor.selectedText().length() + 1);
    }else if (e->key() == Qt::Key_Backspace){
        if (textCursor().position() <= lockPosition){
            e->ignore();
            return;
        }
    }
    QTextEdit::keyPressEvent(e);
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
        }
        else
            cursor.setPosition(lockPosition);
    }
    setTextCursor(cursor);
}
