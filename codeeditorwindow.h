#ifndef CODEEDITORWINDOW_H
#define CODEEDITORWINDOW_H

#include <QMdiSubWindow>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QList>
#include "codeeditor.h"
#include <QObject>

enum MirageFileType {DATA_FILE, TEXT_FILE};

class CodeEditorWindow : public QMdiSubWindow{
    Q_OBJECT
public:
    CodeEditorWindow(QWidget *parent);
    CodeEditorWindow(QWidget *parent, QFont editorFont, MirageFileType nType = TEXT_FILE);
    CodeEditorWindow(QWidget *parent, QFont editorFont, QList<CodeEditorWindow> *currentWins);
    QString getFilePath();
    bool operator==(CodeEditorWindow &window);
    void setFilePath(QString path);
    bool openFile(QString fileName, QString fileTitle);
    bool saveFile();
    void selectAll();
    void quickFind();
    void findAndReplace();

    void setFileType(MirageFileType nType);
    MirageFileType getFileType();

    void setOpened();

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
    MirageFileType fileType;
    bool edited;

    void init();
};

#endif // CODEEDITORWINDOW_H
