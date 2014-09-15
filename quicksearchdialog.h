#ifndef QUICKSEARCHDIALOG_H
#define QUICKSEARCHDIALOG_H

#include <QDialog>

class CodeEditor;

namespace Ui {
class QuickSearchDialog;
}

class QuickSearchDialog : public QDialog
{
    Q_OBJECT

public:
    QuickSearchDialog(QWidget *parent = 0);
    ~QuickSearchDialog();
protected:
    void showEvent(QShowEvent *);

private slots:
    void on_btnFind_clicked();

private:
    Ui::QuickSearchDialog *ui;
    CodeEditor *editor;


};

#endif // QUICKSEARCHDIALOG_H
