#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QCompleter>
#include <QStringListModel>
#include "syntaxhl.h"
#include "linescounter.h"

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    void setCounter(LinesCounter *);
    void setOpened();

private:
    SyntaxHL *SHL;
    QCompleter *codeCompleter;
    LinesCounter *lCounter;
    void focusInEvent(QFocusEvent *);
    void keyPressEvent(QKeyEvent *);
    QTextCursor deleteCurrentLine();
    QString getCurrentLine();
    QTextCursor getSelectedLines();
    QStringListModel *model;
    QStringList compList; //Completion list.
    QStringList labelsList; //Labels list
    int selectionStart, selectionEnd;

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
    void toggleComments();
    void updateLabels();
    void selectLine(int);
    void startSelection(int);
    void addSelectedLines(int);
    void endSelection(int);

private slots:
    void insertCompletion(QString completion);
    void updateCounter();
    void completerPop();
    void highlightLine();
    void updateCounterFormat();



};

#endif // CODEEDITOR_H
