#ifndef CODEEDITORWINDOW_H
#define CODEEDITORWINDOW_H

#include <QMdiSubWindow>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QList>
#include "codeeditor.h"
#include <QObject>

class CodeEditorWindow : public QMdiSubWindow{
    Q_OBJECT
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
    void selectAll();
    void quickFind();
    void findAndReplace();

    CodeEditor *codeEditor();
    bool isEdited();

    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
private slots:
    void editedSlot();

private:
    CodeEditor *editor;
    QWidget *widgetsContainer;
    QHBoxLayout *editorLayout;
    QTextEdit *linesCounter;
    QString filePath;
    QString title;
    bool edited;

    void init();
};

#endif // CODEEDITORWINDOW_H
