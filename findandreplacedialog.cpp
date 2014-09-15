#include "findandreplacedialog.h"
#include "ui_findandreplacedialog.h"
#include "codeeditorwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDebug>
#include <QTextDocumentFragment>

FindAndReplaceDialog::FindAndReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindAndReplaceDialog)
{
    ui->setupUi(this);
    CodeEditorWindow *editorWindow = dynamic_cast<CodeEditorWindow *>(parent);
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

FindAndReplaceDialog::~FindAndReplaceDialog(){
    delete ui;
}

void FindAndReplaceDialog::on_btnClose_clicked(){
    hide();
}

void FindAndReplaceDialog::on_btnReplace_clicked(){
    bool isRegEx = ui->chkRegEx->isChecked();
    bool isCS = ui->chkCS->isChecked();
    bool isWordBound = ui->chkWord->isChecked();
    bool isUp = ui->radUp->isChecked();
    QString editorText = editor->toPlainText();
    QString searchText = ui->txtFind->text();
    QString replaceText = ui->txtReplace->text();

    QString escapedText = isRegEx? searchText:QRegExp::escape(searchText);
    Qt::CaseSensitivity sensitivity = isCS? Qt::CaseSensitive : Qt::CaseInsensitive;
    QString wordBoundary = isWordBound? "\\b":"";
    QRegExp searchRegEx(QString(wordBoundary + escapedText + wordBoundary));
    searchRegEx.setCaseSensitivity(sensitivity);
    if (isRegEx && !searchRegEx.isValid()){
        QMessageBox::critical(this, "Error", "Invalid regular expression");
        return;
    }
    int currentPosition = editor->textCursor().position();
    int nextPosition;
    if (isUp){
        nextPosition = editorText.lastIndexOf(searchRegEx, currentPosition - editor->textCursor().selection().toPlainText().length() - 1);
    }else{
        currentPosition = editor->textCursor().selectionEnd();
        nextPosition = editorText.indexOf(searchRegEx, currentPosition == 0? 0 : currentPosition + 1);
    }
    if (nextPosition != -1){
        QTextCursor currentCurs = editor->textCursor();
        currentCurs.setPosition(nextPosition);
        currentCurs.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, isRegEx? searchRegEx.matchedLength() : searchText.length());
        editor->setTextCursor(currentCurs);
        currentCurs.beginEditBlock();
        currentCurs.removeSelectedText();
        currentCurs.insertText(replaceText);
        currentCurs.endEditBlock();
    }else{
        QMessageBox::information(this, "No Matches", "No more matches to replace");
    }

}

void FindAndReplaceDialog::on_btnReplaceAll_clicked(){
    bool isRegEx = ui->chkRegEx->isChecked();
    bool isCS = ui->chkCS->isChecked();
    bool isWordBound = ui->chkWord->isChecked();
    //bool isUp = ui->radUp->isChecked();
    QString editorText = editor->toPlainText();
    QString searchText = ui->txtFind->text();
    QString replaceText = ui->txtReplace->text();

    QString escapedText = isRegEx? searchText:QRegExp::escape(searchText);
    Qt::CaseSensitivity sensitivity = isCS? Qt::CaseSensitive : Qt::CaseInsensitive;
    QString wordBoundary = isWordBound? "\\b":"";
    QRegExp searchRegEx(QString(wordBoundary + escapedText + wordBoundary));
    searchRegEx.setCaseSensitivity(sensitivity);
    if (isRegEx && !searchRegEx.isValid()){
        QMessageBox::critical(this, "Error", "Invalid regular expression");
        return;
    }
    while (editorText.indexOf(searchRegEx) != -1){
        editorText.replace(searchRegEx, replaceText);
    }
    QTextCursor curs = editor->textCursor();
    curs.select(QTextCursor::Document);
    curs.beginEditBlock();
    curs.removeSelectedText();
    curs.insertText(editorText);
    curs.endEditBlock();
    editor->setTextCursor(curs);
    QMessageBox::information(this, "Replaced", "All matches have been replaced");

}

void FindAndReplaceDialog::on_btnFind_clicked(){
    findNextWord();
}

void FindAndReplaceDialog::findNextWord(){
    bool isRegEx = ui->chkRegEx->isChecked();
    bool isCS = ui->chkCS->isChecked();
    bool isWordBound = ui->chkWord->isChecked();
    bool isUp = ui->radUp->isChecked();
    QString editorText = editor->toPlainText();
    QString searchText = ui->txtFind->text();

    QString escapedText = isRegEx? searchText:QRegExp::escape(searchText);
    Qt::CaseSensitivity sensitivity = isCS? Qt::CaseSensitive : Qt::CaseInsensitive;
    QString wordBoundary = isWordBound? "\\b":"";
    QRegExp searchRegEx(QString(wordBoundary + escapedText + wordBoundary));
    searchRegEx.setCaseSensitivity(sensitivity);
    if (isRegEx && !searchRegEx.isValid()){
        QMessageBox::critical(this, "Error", "Invalid regular expression");
        return;
    }
    int currentPosition = editor->textCursor().position();
    int nextPosition;
    if (isUp){
        nextPosition = editorText.lastIndexOf(searchRegEx, currentPosition - editor->textCursor().selection().toPlainText().length() - 1);
    }else{
        currentPosition = editor->textCursor().selectionEnd();
        nextPosition = editorText.indexOf(searchRegEx, currentPosition);
    }
    if (nextPosition == -1 && editorText.indexOf(searchRegEx) != -1){
        if (isUp){
            nextPosition = editorText.lastIndexOf(searchRegEx, editorText.size() - 1);
        }else
            nextPosition = editorText.indexOf(searchRegEx, 0);
    }

    if (nextPosition != -1){
        QTextCursor currentCurs = editor->textCursor();
        currentCurs.setPosition(nextPosition);
        currentCurs.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, isRegEx? searchRegEx.matchedLength() : searchText.length());
        editor->setTextCursor(currentCurs);
    }else{
        QMessageBox::information(this, "No Matches", "No matches found");
    }
}
