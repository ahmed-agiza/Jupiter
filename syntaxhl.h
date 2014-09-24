#ifndef SYNTAXHL_H
#define SYNTAXHL_H

#include <QSyntaxHighlighter>
#include <QTextEdit>
#include <QVector>


class SyntaxHL : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SyntaxHL(QTextEdit *parent = 0);
    SyntaxHL(QObject *parent = 0);
    SyntaxHL(QTextDocument *parent = 0);
    void setLabelsList(QStringList&);


protected:
    void highlightBlock (const QString &text);

private:
    void init();
    struct syntax{
             QRegExp pattern;
             QTextCharFormat format;
             bool labelSyntax;
    };

    QVector<syntax> syntaxes;


    QTextCharFormat instructionFormat;
    QTextCharFormat pseudoFormat;
    QTextCharFormat registerFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat labelFormat;
    QTextCharFormat macroFormat;
    QTextCharFormat immFormat;
    QTextCharFormat openStringFormat;

    QStringList instructionsList;
    QStringList pseudoList;
    QStringList registersList;
    QStringList macrosList;

    //QStringList labelsList;

signals:

public slots:

};

#endif // SYNTAXHL_H
