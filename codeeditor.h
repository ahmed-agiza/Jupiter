#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QScroller>
#include "syntaxhl.h"
#include "linescounter.h"

class CodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    void setCounter(LinesCounter *);
    void setOpened();
    void setEmitLabels(bool);
    void sendLabelsSignal();
    QStringList getLabels();

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
    QStringList localLabelsList; //Labels list
    QStringList globalLabelList;
    QStringList externalLabelsList;
    int selectionStart, selectionEnd;
    QScroller *scroll;
    bool emitLabels;

protected:
    void scrollContentsBy(int, int);

signals:
    void labelsUpdated();
    void refreshLabels();
    void updateScroll(int);

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
    void updateGlobalLabel(QStringList);
    void toogleBP();


private slots:
    void insertCompletion(QString completion);
    void updateCounter();
    void completerPop();
    void highlightLine();
    void updateCounterFormat();
    void refreshScroll();
    void checkLabels();



};

#endif // CODEEDITOR_H
