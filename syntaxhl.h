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

protected:
    void highlightBlock (const QString &text);

private:
    struct syntax
    {
             QRegExp pattern;
             QTextCharFormat format;
    };

    QVector<syntax> syntaxes;


    QTextCharFormat instructionFormat;
    QTextCharFormat registerFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat labelFormat;
    QTextCharFormat macroFormat;

    QStringList instructionsList;
    QStringList registersList;
    QStringList macrosList;

signals:

public slots:

};

#endif // SYNTAXHL_H
