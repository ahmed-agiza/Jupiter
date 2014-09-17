#include "linescounter.h"
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QApplication>
#include <QDrag>
#include <QDebug>
#include <QTextDocumentFragment>


LinesCounter::LinesCounter(QWidget *parent) :
    QTextEdit(parent){
    installEventFilter(this);

    QFont defaultFont = font();
    defaultFont.setFamily("Consolas");
    defaultFont.setPointSize(10);
    QFont boldFont = defaultFont;
    boldFont.setWeight(QFont::DemiBold);
    defaultFont.setWeight(QFont::Normal);
    setFont(defaultFont);
    defaultFormat = currentCharFormat();
    defaultFormat.setFont(defaultFont);
    setCurrentCharFormat(defaultFormat);

    boldFormat = defaultFormat;
    boldFormat.setFont(boldFont);
    vbar = verticalScrollBar();
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

int LinesCounter::getLineNumber(QPoint linePos){
    QTextCursor cur = cursorForPosition(linePos);
    cur.select(QTextCursor::LineUnderCursor);
    return cur.selectedText().trimmed().toInt();
}

void LinesCounter::boldLines(int start, int end){
    QTextCursor defaultCursor = textCursor();
    defaultCursor.select(QTextCursor::Document);
    defaultCursor.setCharFormat(defaultFormat);
    QTextCursor selectionCursor = textCursor();
    selectionCursor.setPosition(0);
    selectionCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, start);
   if (start == end)
        selectionCursor.select(QTextCursor::LineUnderCursor);
    else{
        selectionCursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, end - start);
        selectionCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    }

    selectionCursor.setCharFormat(boldFormat);

}

bool LinesCounter::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress){
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(e);
        if (mouseEvent)
            if (mouseEvent->buttons() & Qt::LeftButton){
                emit startSelection(getLineNumber(mouseEvent->pos()));
            }


    }else if (e->type() == QEvent::MouseMove){
        QMouseEvent *mouseDrag = dynamic_cast<QMouseEvent *>(e);
        if (mouseDrag)
            if (mouseDrag->buttons() & Qt::LeftButton){
                emit selectLines(getLineNumber(mouseDrag->pos()));
            }

    }else if  (e->type() == QEvent::MouseButtonRelease){
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(e);
        if (mouseEvent){
                emit endSelection(getLineNumber(mouseEvent->pos()));
            int line = getLineNumber(mouseEvent->pos());

        }

    }

    return QTextEdit::eventFilter(o, e);

}

void LinesCounter::scrollWithEditor(int val){
    if (vbar){
        vbar->setValue(val);
    }

}


