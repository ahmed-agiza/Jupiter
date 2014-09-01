#include "codeeditorwindow.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>

CodeEditorWindow::CodeEditorWindow(QWidget *parent):QMdiSubWindow(parent){
    init();
}

CodeEditorWindow::CodeEditorWindow(QWidget *parent, QFont editorFont):QMdiSubWindow(parent){
    init();
    editor->setFont(editorFont);

}


QString CodeEditorWindow::getFilePath(){
    return filePath;
}

bool CodeEditorWindow::operator== (CodeEditorWindow &window){
    if (filePath == "NULL" || filePath.trimmed() == "" || window.getFilePath() == "NULL" || window.getFilePath().trimmed() == "")
        return false;
    if (filePath.trimmed() == window.getFilePath().trimmed())
        return true;
    return false;
}

void CodeEditorWindow::setFilePath(QString path){
    filePath = path;
}

bool CodeEditorWindow::openFile(QString fileName)
{
    QFile file(fileName);
    QString fileData;
    QTextStream stream (&file);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        title = fileName;
        setFilePath(fileName);
        while (!stream.atEnd()){
            fileData.append(stream.readLine() + "\n");
        }
        file.close();
        editor->setText(fileData);
        setWindowTitle(fileName);
        return true;

    }else
        return false;
}

void CodeEditorWindow::saveFile()
{

}

void CodeEditorWindow::saveFileAs()
{

}

void CodeEditorWindow::selectAll()
{
    editor->selectAll();
}

void CodeEditorWindow::quickFind()
{

}

void CodeEditorWindow::findAndReplace()
{

}

CodeEditor *CodeEditorWindow::codeEditor()
{
    return editor;
}

bool CodeEditorWindow::isEdited()
{
    return edited;
}

void CodeEditorWindow::cut()
{
    editor->cut();
}

void CodeEditorWindow::copy()
{
    editor->copy();
}

void CodeEditorWindow::paste()
{
    editor->paste();
}

void CodeEditorWindow::undo()
{
    editor->undo();
}

void CodeEditorWindow::redo()
{
    editor->redo();
}

void CodeEditorWindow::editedSlot()
{
    edited = true;
    setWindowTitle(title + "*");
}


void CodeEditorWindow::init()
{
    setObjectName("newW");
    setWindowTitle("Untitled");
    title = "Untitled";
    edited = false;
    filePath = "NULL";

    widgetsContainer = new QWidget(this);
    widgetsContainer->setObjectName("NW");
    editorLayout = new QHBoxLayout(widgetsContainer);
    editor = new CodeEditor(widgetsContainer);
    linesCounter = new QTextEdit(widgetsContainer);

    editor->setObjectName("CodeE");
    //newCode->setFont(editorFont);

    linesCounter->setEnabled(false);
    linesCounter->setMinimumSize(10, 10);
    linesCounter->setMaximumSize(40, linesCounter->maximumSize().height());
    linesCounter->setText("0");
    editor->setCounter(linesCounter);

    editorLayout->setObjectName("HL");
    editorLayout->addWidget(linesCounter);
    editorLayout->addWidget(editor);

    widgetsContainer->setLayout(editorLayout);

    setWidget(widgetsContainer);

    setAttribute(Qt::WA_DeleteOnClose, 1);
    widgetsContainer->showMaximized();
    connect(editor, SIGNAL(textChanged()), this, SLOT(editedSlot()));
}
