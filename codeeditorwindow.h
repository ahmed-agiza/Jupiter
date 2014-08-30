#ifndef CODEEDITORWINDOW_H
#define CODEEDITORWINDOW_H

#include <QMdiSubWindow>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QList>
#include "codeeditor.h"

class CodeEditorWindow : public QMdiSubWindow{
public:
    CodeEditorWindow(QWidget *parent);
    CodeEditorWindow(QWidget *parent, QFont editorFont);
    CodeEditorWindow(QWidget *parent, QFont editorFont, QList<CodeEditorWindow> *currentWins);
    QString getFilePath();
    bool operator==(CodeEditorWindow &window);
    void setFilePath(QString path);
    bool openFile(QString fileName);
    void saveFile();
    void saveFileAs();
private:
    CodeEditor *editor;
    QWidget *widgetsContainer;
    QHBoxLayout *editorLayout;
    QTextEdit *linesCounter;
    QString filePath;

    void init();
};

#endif // CODEEDITORWINDOW_H
