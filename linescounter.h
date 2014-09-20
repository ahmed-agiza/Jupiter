#ifndef LINESCOUNTER_H
#define LINESCOUNTER_H

#include <QTextEdit>
#include <QScrollBar>

class LinesCounter : public QTextEdit
{
    Q_OBJECT
public:
    explicit LinesCounter(QWidget *parent = 0);
    int getLineNumber(QPoint);
    void boldLines(int, int);
    QList<int> getBreakPoints();

private:
    QTextCharFormat defaultFormat;
    QTextCharFormat boldFormat;
    QScrollBar *vbar;
    QList<int> bps;
    int lastLine;

protected:
    bool eventFilter(QObject *, QEvent *);


signals:
    //void selectLine(int);
    //void selectLines(int, int);
    void startSelection(int);
    void selectLines(int);
    void endSelection(int);
    void sendBPs(QList<int>);


public slots:
    void scrollWithEditor(int);
    void clearBPs();
    void updateBPs();
    void toggleBPs(int);
    void highlightBPs(int, bool);
    void refreshAll();
    void setMaxLine(int);

private slots:
    void trimBPs();

};

#endif // LINESCOUNTER_H
