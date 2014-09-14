#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include "syntaxhl.h"
#include <QCompleter>
#include <QStringListModel>

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    void setCounter(QTextEdit *lc);
    void setOpened();

private:
    SyntaxHL *SHL;
    QCompleter *codeCompleter;
    QTextEdit *lCounter;
    void focusInEvent(QFocusEvent *);
    void keyPressEvent(QKeyEvent *);
    QTextCursor deleteCurrentLine();
    QString getCurrentLine();
    QTextCursor getSelectedLines();
    QStringListModel *model;
    QStringList compList; //Completion list.
    QStringList labelsList; //Labels list

signals:
    void labelsUpdated();
public slots:
    void deleteSelection();
    void deleteLine();
    void moveLineUp();
    void moveLineDown();
    void copyLineUp();
    void copyLineDown();
    void popupSuggestions();
    void commentLine();
    void updateLabels();

private slots:
    void insertCompletion(QString completion);
    void updateCounter();
    void completerPop();
    void highlightLine();



};

#endif // CODEEDITOR_H
