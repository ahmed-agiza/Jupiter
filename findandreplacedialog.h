#ifndef FINDANDREPLACEDIALOG_H
#define FINDANDREPLACEDIALOG_H

#include <QDialog>

class CodeEditor;

namespace Ui {
class FindAndReplaceDialog;
}

class FindAndReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindAndReplaceDialog(QWidget *parent = 0);
    ~FindAndReplaceDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnReplace_clicked();

    void on_btnReplaceAll_clicked();

    void on_btnFind_clicked();

private:
    Ui::FindAndReplaceDialog *ui;
    CodeEditor *editor;
    void findNextWord();
    bool exists;
    bool replacing;
};

#endif // FINDANDREPLACEDIALOG_H
