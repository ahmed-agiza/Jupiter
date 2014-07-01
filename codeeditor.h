#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include "syntaxhl.h"
#include <QCompleter>

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    void setCounter(QTextEdit *lc);

private:
    SyntaxHL *SHL;
    QCompleter *codeCompleter;
    QTextEdit *lCounter;
    void focusInEvent(QFocusEvent *);
    void keyPressEvent(QKeyEvent *);


signals:

private slots:
    void insertCompletion(QString completion);
    void updateCounter();
    void completerPop();
    void highlightLine();


};

#endif // CODEEDITOR_H
