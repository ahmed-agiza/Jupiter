#include "codeeditor.h"
#include <QDebug>
#include <QAbstractItemView>

CodeEditor::CodeEditor(QWidget *parent) :
    QTextEdit(parent)
{


    SHL = new SyntaxHL(this); //Adding syntax highlighter.

    //Auto-complete setup:
    QStringList compList; //Completion list.
    compList << "add" << "addu" << "addi" << "sub" << "sll" << "slt" << "slti";
    codeCompleter = new QCompleter(compList, this);
    codeCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    codeCompleter->setWidget(this);
    codeCompleter->setCompletionMode(QCompleter::PopupCompletion);

    QObject::connect(codeCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(updateCounter()));
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(completerPop()));
    QObject::connect(this, SIGNAL(selectionChanged()), this, SLOT(highlightLine()));
}

void CodeEditor::insertCompletion(QString completion)
{
    QTextCursor currentPos = textCursor();
    int compLength = completion.length() - codeCompleter->completionPrefix().length();
    currentPos.movePosition(QTextCursor::Left);
    currentPos.movePosition(QTextCursor::EndOfWord);
    currentPos.insertText(completion.right(compLength));
    this->setTextCursor(currentPos);
}

void CodeEditor::highlightLine()
{
    QList<QTextEdit::ExtraSelection> linesHL;
    QTextEdit::ExtraSelection lineHL;
    lineHL.format.setBackground(QColor(Qt::lightGray).darker(180));
    lineHL.format.setProperty(QTextFormat::FullWidthSelection, true);
    lineHL.cursor = textCursor();
    lineHL.cursor.clearSelection();
    linesHL.append(lineHL);
    this->setExtraSelections(linesHL);
}


void CodeEditor::updateCounter()
{
    //Line count.
   if(lCounter)
   {
       lCounter->clear();
       lCounter->setAlignment(Qt::AlignCenter);
        int numLines = this->toPlainText().count("\n");
        for (int i = 0; i <= numLines; i++)
           lCounter->append(QString::number(i));

   }
}
void CodeEditor::completerPop()
{
    QTextCursor sel = textCursor();
                sel.select(QTextCursor::WordUnderCursor);
    codeCompleter->setCompletionPrefix(sel.selectedText());

    if(sel.selectedText().length() > 0)
    {
        if ((codeCompleter->completionCount() != 1) || ((codeCompleter->completionModel()->data(codeCompleter->model()->index(0, 0)).toString()) != sel.selectedText()))
        {
            QRect popRect = this->cursorRect();
            popRect.setWidth(50);
            codeCompleter->complete(popRect);
        }
        else
            codeCompleter->popup()->hide();
    }
    else
        codeCompleter->popup()->hide();



}

void CodeEditor::setCounter(QTextEdit *lc)
{
    lCounter = lc;
}
