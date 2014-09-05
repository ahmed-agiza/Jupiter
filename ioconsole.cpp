#include "ioconsole.h"
#include <QDebug>
#include <QKeyEvent>

IOConsole::IOConsole(QWidget *parent) :
    QTextEdit(parent){
    installEventFilter(this);
    lockPosition = 0;
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorChanged()));
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
    selectionLocker = false;
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
        setReadOnly(true);
        setLock(lockPosition + cursor.selectedText().length());

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
