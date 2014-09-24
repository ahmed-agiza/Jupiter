#include "codeeditorwindow.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

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
    p = dynamic_cast<QMdiArea *>(parent);
}


QString CodeEditorWindow::getFilePath(){
    return filePath;
}

bool CodeEditorWindow::isModified(){
    return edited;
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
        editor->setPlainText(fileData);
        //editor->setText(fileData);
        setWindowTitle(title);
        edited = false;
        return true;

    }else
        return false;
}

bool CodeEditorWindow::saveFile(){
    if (filePath.trimmed() != "" && filePath.trimmed() != "NULL"){
        QFile file(filePath);

        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
                QTextStream writer(&file);
                QStringList lines = editor->toPlainText().split("\n");
                for(int i = 0; i < lines.size(); i++){
                    writer << lines.at(i);
                    if (i != lines.size() - 1)
                        writer << endl;
                }
                file.close();
                setWindowTitle(title);
                edited = false;
                return true;
            }else
                QMessageBox::critical(this, "Error", "Cannot open the file for saving");

    }
    return false;

}
void CodeEditorWindow::closeEvent(QCloseEvent *closeEvent){
    if (destroyed){
        closeEvent->accept();
        return;
    }
    if (edited){
        QMessageBox::StandardButton btn = QMessageBox::question(this, QString("Close"), QString("The changes to the file " + title.trimmed() + " have not be saved\nDo you want to save the file before closing?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (btn == QMessageBox::Yes){
            saveFile();
            closeEvent->accept();
            return;
        }else if (btn == QMessageBox::No){
            closeEvent->accept();
            return;
        }else{
            closeEvent->ignore();
            return;
        }
    }
    if(p)
        p->activateNextSubWindow();
    else
        qDebug() << "Parent QMdiArea not found";

    closeEvent->accept();
}


void CodeEditorWindow::selectAll(){
    editor->selectAll();

}

void CodeEditorWindow::deleteText(){
    editor->deleteSelection();
}

void CodeEditorWindow::quickFind(){
    quickSearch->show();
}

void CodeEditorWindow::findAndReplace(){
    findReplace->show();
}

void CodeEditorWindow::setDestryoed(bool value){
    destroyed = value;
}

bool CodeEditorWindow::isDestroyed(){
    return destroyed;
}

QString CodeEditorWindow::getContent(){
    return editor->toPlainText();
}

QString CodeEditorWindow::getUncommentedContent(){
    QRegExp commentsRegEx("#[^\n]*");
    return editor->toPlainText().remove(commentsRegEx);
}

QStringList CodeEditorWindow::getContentList(){
    return editor->toPlainText().split("\n");
}

QStringList CodeEditorWindow::getUncommentedContentList(){
    QRegExp commentsRegEx("#[^\n]*");
    return editor->toPlainText().remove(commentsRegEx).split("\n");
}

QStringList CodeEditorWindow::getStrippedContentList(){
    QStringList rawLines = editor->toPlainText().split("\n");
    int rawIterator = 0;
    if (rawLines.size() == 0)
        return QStringList();
    QRegExp commentsRegEx("#[^\n]*");
    QStringList lines = editor->toPlainText().remove(commentsRegEx).split("\n");
    for (int i = 0; i < lines.length(); i++)
        lines[i] = lines[i].trimmed();
    lines.removeAll("");
    linesMapping.clear();
    for (int i = 0; i < lines.length(); i++){
        for(int j = rawIterator; j < rawLines.size(); j++){
            if(lines.at(i) == rawLines.at(j).trimmed()){
                linesMapping[i] = j;
                rawIterator = j + 1;
                break;
            }
        }
    }

    return lines;
}

QString CodeEditorWindow::getTitle(){
      return title;
}

void CodeEditorWindow::setTitle(QString newTitle){
    title = newTitle;
    if(edited){
        setWindowTitle(title + "*");
    }else
        setWindowTitle(title);
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

bool CodeEditorWindow::isEdited(){
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

void CodeEditorWindow::toogleBP(){
    editor->toogleBP();
}

void CodeEditorWindow::clearBPs(){
    linesCounter->clearBPs();
}

QMap<int, int> CodeEditorWindow::getLineMapping(){
    return linesMapping;
}

QList<int> CodeEditorWindow::getBPs(){
    return linesCounter->getBreakPoints();
}

bool CodeEditorWindow::isMainFile(){
    return mainFile;
}

void CodeEditorWindow::setMainFile(bool value){
    mainFile = value;
}

CodeEditorWindow::~CodeEditorWindow(){
}

void CodeEditorWindow::editedSlot(){
    edited = true;
    setWindowTitle(title + "*");
}

void CodeEditorWindow::enableEditing(){
    editor->setReadOnly(false);
}

void CodeEditorWindow::disableEditing(){
    editor->setReadOnly(true);
}

void CodeEditorWindow::getBreakpoints(QVector<int> breakPs){
    bps = breakPs;
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
    linesCounter = new LinesCounter(widgetsContainer);
    QObject::connect(editor, SIGNAL(updateScroll(int)), linesCounter, SLOT(scrollWithEditor(int)));

    editor->setObjectName("CodeE");

    linesCounter->setEnabled(false);
    linesCounter->setMinimumSize(10, 10);
    linesCounter->setMaximumWidth(60);
    linesCounter->setMaximumHeight(linesCounter->maximumSize().height());
    //linesCounter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    linesCounter->setPlainText("0");
    editor->setCounter(linesCounter);
    QObject::connect(linesCounter, SIGNAL(startSelection(int)), editor, SLOT(startSelection(int)));
    QObject::connect(linesCounter, SIGNAL(selectLines(int)), editor, SLOT(addSelectedLines(int)));
    QObject::connect(linesCounter, SIGNAL(endSelection(int)), editor, SLOT(endSelection(int)));


    editorLayout->setObjectName("HL");
    editorLayout->addWidget(linesCounter);
    editorLayout->addWidget(editor);

    widgetsContainer->setLayout(editorLayout);

    setWidget(widgetsContainer);

    setAttribute(Qt::WA_DeleteOnClose, 1);
    widgetsContainer->showMaximized();
    connect(editor, SIGNAL(textChanged()), this, SLOT(editedSlot()));
    destroyed = false;
    quickSearch = new QuickSearchDialog(this);
    quickSearch->hide();
    findReplace = new FindAndReplaceDialog(this);
    findReplace->hide();
}
