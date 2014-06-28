#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringListModel>
#include <QLineEdit>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    ui->subwindowMain->showMaximized();
    SHL = new SyntaxHL(ui->textCodeEditor); //Adding syntax highlighter.

    //Auto-complete setup:
    QStringList compList; //Completion list.
    compList << "add" << "addu" << "addi" << "sub" << "sll" << "slt" << "slti";
    codeCompleter = new QCompleter(compList, this);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    //qDebug() << "ff" << codeCompleter->widget();
    codeCompleter->setWidget(ui->textCodeEditor);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);

    //qDebug() << codeCompleter->widget();
    QObject::connect(codeCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::insertCompletion(QString completion)
{
   // qDebug() << "Here.";

    QTextCursor currentPos = ui->textCodeEditor->textCursor();
    int compLength = completion.length() - codeCompleter->completionPrefix().length();
    currentPos.movePosition(QTextCursor::Left);
    currentPos.movePosition(QTextCursor::EndOfWord);
    currentPos.insertText(completion.right(compLength));
    ui->textCodeEditor->setTextCursor(currentPos);

}

void MainWindow::on_textCodeEditor_textChanged()
{
    QTextCursor sel = ui->textCodeEditor->textCursor();
                sel.select(QTextCursor::WordUnderCursor);
    codeCompleter->setCompletionPrefix(sel.selectedText());

    if(sel.selectedText().length() > 0)
    {
        if ((codeCompleter->completionCount() != 1) || ((codeCompleter->completionModel()->data(codeCompleter->model()->index(0, 0)).toString()) != sel.selectedText()))
        {
            QRect popRect = ui->textCodeEditor->cursorRect();
            popRect.setWidth(50);
            codeCompleter->complete(popRect);
        }
        else
            codeCompleter->popup()->hide();
    }
    else
        codeCompleter->popup()->hide();


    //Line count.
    ui->codeLineNumber->clear();
    ui->codeLineNumber->setAlignment(Qt::AlignCenter);
    int numLines = ui->textCodeEditor->toPlainText().count("\n");
    for (int i = 0; i <= numLines; i++)
        ui->codeLineNumber->append(QString::number(i));

}

void MainWindow::on_textCodeEditor_selectionChanged()
{

}
