#ifndef CODEMDIAREA_H
#define CODEMDIAREA_H

#include <QMdiArea>
#include "codeeditorwindow.h"

class CodeMDIArea : public QMdiArea
{
    Q_OBJECT
public:
    explicit CodeMDIArea(QWidget *parent = 0);
    bool isMainFileOpen();
    void setMainFile(QString);
    void setMainWindow(CodeEditorWindow* );
    void setLabels(QStringList);
    CodeEditorWindow* getMainWindow();
    void activateMainWindow();

    QMdiSubWindow* addSubWindow(QWidget *, Qt::WindowFlags = 0);
private:
    CodeEditorWindow *mainWindow;
    QString filePath;
    QString fileName;
    QStringList globalLabels;
    bool hasMainFile;
    int lastLine;
    bool enableEditing;

signals:

public slots:
    void selectMainFileLine(int);
    void enableMainFileEditing();
    void disableMainFileEditing();
private slots:
    void mainFileRemoved();

};

#endif // CODEMDIAREA_H
