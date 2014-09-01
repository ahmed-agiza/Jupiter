#include "codeeditorwindow.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

CodeEditorWindow::CodeEditorWindow(QWidget *parent):QMdiSubWindow(parent){
    init();
}

CodeEditorWindow::CodeEditorWindow(QWidget *parent, QFont editorFont, MirageFileType nType):QMdiSubWindow(parent){
    init();
    editor->setFont(editorFont);
    linesCounter->setFont(editorFont);
    QPalette palette = linesCounter->palette();
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(Qt::white).darker(50));
    linesCounter->setPalette(palette);
    fileType = nType;

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

bool CodeEditorWindow::openFile(QString fileName, QString fileTitle){
    QFile file(fileName);
    QString fileData;
    QTextStream stream (&file);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if (fileTitle.trimmed() == "")
            title = fileName;
        else
            title = fileTitle;
        filePath = fileName;
        setFilePath(fileName);
        while (!stream.atEnd()){
            fileData.append(stream.readLine() + "\n");
        }
        file.close();
        editor->setText(fileData);
        setWindowTitle(title);
        return true;

    }else
        return false;
}

bool CodeEditorWindow::saveFile(){
    if (filePath.trimmed() != "" && filePath.trimmed() != "NULL"){
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)){
            file.close();
            if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
                QTextStream writer(&file);
                QStringList lines = editor->toPlainText().split("\n");
                for(int i = 0; i < lines.size(); i++)
                    writer << lines.at(i) << endl;
                file.close();
                setWindowTitle(title);
                edited = false;
                return true;
            }else{
                QMessageBox::critical(this, "Error", "Cannot open the file for saving");
                return false;
            }
        }
    }else
        return false;

}



void CodeEditorWindow::selectAll()
{
    editor->selectAll();
}

void CodeEditorWindow::quickFind(){

}

void CodeEditorWindow::findAndReplace(){

}

void CodeEditorWindow::setFileType(MirageFileType nType){
    fileType = nType;
}

MirageFileType CodeEditorWindow::getFileType(){
    return fileType;
}

void CodeEditorWindow::setOpened(){
    editor->setOpened();
}

CodeEditor *CodeEditorWindow::codeEditor(){
    return editor;
}

bool CodeEditorWindow::isEdited()
{
    return edited;
}

void CodeEditorWindow::cut(){
    editor->cut();
}

void CodeEditorWindow::copy(){
    editor->copy();
}

void CodeEditorWindow::paste(){
    editor->paste();
}

void CodeEditorWindow::undo(){
    editor->undo();
}

void CodeEditorWindow::redo(){
    editor->redo();
}

void CodeEditorWindow::editedSlot(){
    edited = true;
    setWindowTitle(title + "*");
}


void CodeEditorWindow::init(){
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
