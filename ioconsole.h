#ifndef IOCONSOLE_H
#define IOCONSOLE_H

#include <QTextEdit>

class IOConsole : public QTextEdit
{
    Q_OBJECT
public:
    explicit IOConsole(QWidget *parent = 0);
    void setLock(int);
    void addText(QString, bool ro);
    int getLockPosition();
    void setLockAtEnd();
    QString getInputAt(int index);
    void clearInputAt(int index);
    void reprint();
private:
    int lockPosition;
    bool selectionLocker;
    QStringList inputTrace;
    QAction *clearAction;
    QAction *copyAction;
    QAction *exportAction;
protected:
    bool eventFilter(QObject *, QEvent *);
    void keyPressEvent(QKeyEvent *);

signals:
    void dataInput(QString);

public slots:
    void enableEditing(bool);
    void getInput();
    void clearConsole();
    void copyAll();


private slots:
    void onCursorChanged();
    void onSelectionChanged();
};

#endif // IOCONSOLE_H
