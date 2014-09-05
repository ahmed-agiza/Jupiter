#ifndef IOCONSOLE_H
#define IOCONSOLE_H

#include <QTextEdit>

class IOConsole : public QTextEdit
{
    Q_OBJECT
public:
    explicit IOConsole(QWidget *parent = 0);
    void setLock(int);
    void addText(QString);
    int getLockPosition();
    void setLockAtEnd();
private:
    int lockPosition;
protected:
    bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *);

signals:
    void dataInput(QString);

private slots:
    void onCursorChanged();
    void onSelectionChanged();
};

#endif // IOCONSOLE_H
