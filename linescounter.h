#ifndef LINESCOUNTER_H
#define LINESCOUNTER_H

#include <QTextEdit>

class LinesCounter : public QTextEdit
{
    Q_OBJECT
public:
    explicit LinesCounter(QWidget *parent = 0);
    int getLineNumber(QPoint);
    void boldLines(int, int);

private:
    QTextCharFormat defaultFormat;
    QTextCharFormat boldFormat;

protected:
    bool eventFilter(QObject *, QEvent *);

signals:
    //void selectLine(int);
    //void selectLines(int, int);
    void startSelection(int);
    void selectLines(int);
    void endSelection(int);


public slots:

};

#endif // LINESCOUNTER_H
