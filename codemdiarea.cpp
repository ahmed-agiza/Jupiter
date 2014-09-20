#include <QDebug>
#include "codemdiarea.h"

CodeMDIArea::CodeMDIArea(QWidget *parent) :
    QMdiArea(parent){
    setViewMode(QMdiArea::TabbedView);
    setActivationOrder(QMdiArea::ActivationHistoryOrder);
    setDocumentMode(true);
    setTabsClosable(true);
    setTabsMovable(true);
    hasMainFile = false;
    lastLine = 0;
    enableEditing = true;

}

bool CodeMDIArea::isMainFileOpen(){
    return hasMainFile;
}

void CodeMDIArea::setMainFile(QString file){
    filePath = file;
}

void CodeMDIArea::setMainWindow(CodeEditorWindow *window){
    if (window){
        mainWindow = window;
        hasMainFile = true;
        QObject::connect(mainWindow, SIGNAL(destroyed()), this, SLOT(mainFileRemoved()));
        filePath = mainWindow->getFilePath();
        fileName = mainWindow->getTitle();
        lastLine = 0;
        if (enableEditing)
            enableMainFileEditing();
        else
            disableMainFileEditing();
    }
}

void CodeMDIArea::setLabels(QStringList labels){
    globalLabels = labels;
    foreach(QMdiSubWindow *window, subWindowList()){
        CodeEditorWindow* currentWindow = dynamic_cast<CodeEditorWindow *>(window);
        if (currentWindow){
            currentWindow->codeEditor()->updateGlobalLabel(globalLabels);
        }
    }

}

CodeEditorWindow *CodeMDIArea::getMainWindow(){
    return mainWindow;
}

void CodeMDIArea::activateMainWindow(){
    if (hasMainFile && mainWindow)
        setActiveSubWindow(mainWindow);
}

bool CodeMDIArea::isAllSaved(){
    foreach(QMdiSubWindow *window, subWindowList()){
        CodeEditorWindow* currentWindow = dynamic_cast<CodeEditorWindow *>(window);
        if (currentWindow)
            if (currentWindow->isModified())
                return false;
    }
    return true;
}

QMdiSubWindow *CodeMDIArea::addSubWindow(QWidget *widget, Qt::WindowFlags flags){
    QMdiSubWindow *tempWindow = QMdiArea::addSubWindow(widget, flags);
    setLabels(globalLabels);
    return tempWindow;
}

void CodeMDIArea::selectMainFileLine(int lineNumber){
    lastLine = lineNumber;
    if (hasMainFile && mainWindow){
        QTextCursor curs = mainWindow->codeEditor()->textCursor();
        curs.clearSelection();
        curs.setPosition(0);
        curs.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber);
        mainWindow->codeEditor()->setTextCursor(curs);
        if(!mainWindow->isActiveWindow()){
            mainWindow->repaint();
            mainWindow->codeEditor()->repaint();
        }

    }

}

void CodeMDIArea::enableMainFileEditing(){
    if (hasMainFile)
        mainWindow->codeEditor()->setReadOnly(false);
    enableEditing = true;
}

void CodeMDIArea::disableMainFileEditing(){
    if (hasMainFile)
        mainWindow->codeEditor()->setReadOnly(true);
    enableEditing = false;
}




void CodeMDIArea::mainFileRemoved(){
    hasMainFile = false;
    fileName = "";
    filePath = "";
    lastLine = 0;
}




