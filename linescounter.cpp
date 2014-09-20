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
    lastLine = 0;

    //QObject::connect(this, SIGNAL(textChanged()), this, SLOT(setMaxLine()));
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
    updateBPs();

}

QList<int> LinesCounter::getBreakPoints(){
    return bps;
}

bool LinesCounter::eventFilter(QObject *o, QEvent *e)
{

    if (e->type() == QEvent::MouseButtonDblClick){
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(e);
        if (mouseEvent){
            int line = getLineNumber(mouseEvent->pos());
            toggleBPs(line);
        }
    }else if (e->type() == QEvent::MouseButtonPress){
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

        }

    }

    return QTextEdit::eventFilter(o, e);

}

void LinesCounter::scrollWithEditor(int val){
    if (vbar){
        vbar->setValue(val);
    }

}

void LinesCounter::clearBPs(){
    bps.clear();
    refreshAll();
}

void LinesCounter::updateBPs(){
    foreach(int line, bps){
        highlightBPs(line, true);
    }
}

void LinesCounter::toggleBPs(int line){
    if (bps.contains(line)){
        bps.removeOne(line);
        highlightBPs(line, false);
    }else{
        bps.append(line);
    }

    std::sort(bps.begin(), bps.end(), qLess<int>());
    updateBPs();
}

void LinesCounter::highlightBPs(int line, bool hl){
   QTextCursor currentCur = textCursor();

   currentCur.setPosition(0);
   bool validMove = currentCur.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, line);
   if (!validMove)
       return;
   currentCur.select(QTextCursor::LineUnderCursor);
   QTextCharFormat tempFormat = currentCur.charFormat();
   if (hl){
        tempFormat.setForeground(QColor(Qt::red).lighter(135));
   }else
       tempFormat.setForeground(defaultFormat.foreground());
   currentCur.setCharFormat(tempFormat);

}

void LinesCounter::refreshAll(){
    QTextCursor cur = textCursor();
    for(int i = 0; i < lastLine; i++){
        cur.setPosition(0);
        cur.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, i);
        cur.select(QTextCursor::LineUnderCursor);
        bool hl = bps.contains(cur.selection().toPlainText().trimmed().toInt());
        highlightBPs(i, hl);
    }

}

void LinesCounter::setMaxLine(int num){
    lastLine = num;
    trimBPs();

}

void LinesCounter::trimBPs(){
    bool trimmed = false;
    while (!bps.isEmpty() && bps.last() > lastLine){
        trimmed = true;
        bps.removeLast();
    }

     if (trimmed)
         refreshAll();
     updateBPs();
}


