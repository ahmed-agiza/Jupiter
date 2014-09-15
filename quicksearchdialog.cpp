#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QMessageBox>
#include "quicksearchdialog.h"
#include "ui_quicksearchdialog.h"
#include "codeeditorwindow.h"

QuickSearchDialog::QuickSearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickSearchDialog)
{
    ui->setupUi(this);
    CodeEditorWindow *editorWindow = dynamic_cast<CodeEditorWindow*>(parent);
    if (editorWindow){
        editor = editorWindow->codeEditor();
    }else{
        QMessageBox::critical(this, "Error", "Unidentified error");
        close();
    }

    QRect screenGeo = QApplication::desktop()->screenGeometry(0);
    QPoint centerPoint = screenGeo.center();
    centerPoint.setX(centerPoint.x() - this->width()/2);
    centerPoint.setY(centerPoint.y() - this->height()/2);
    move(centerPoint);
}

QuickSearchDialog::~QuickSearchDialog()
{
    delete ui;
}


void QuickSearchDialog::showEvent(QShowEvent *e){
    QDialog::showEvent(e);
}



void QuickSearchDialog::on_btnFind_clicked(){
    QString editorText = editor->toPlainText();
    QString searchText = ui->txtSearch->text();
    int currentPosition = editor->textCursor().selectionEnd();
    int nextPosition = editorText.indexOf(searchText, currentPosition);
    if (nextPosition == -1 && editorText.indexOf(searchText) != -1)
        nextPosition = editorText.indexOf(searchText, 0);

    if (nextPosition != -1){
        QTextCursor currentCurs = editor->textCursor();
        currentCurs.setPosition(nextPosition);
        currentCurs.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, searchText.length());
        editor->setTextCursor(currentCurs);
    }
}
