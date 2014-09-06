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
    int requestNumber;
    int readingLimt;
protected:
    void keyPressEvent(QKeyEvent *);

signals:
    void sendInt(int);
    void sendString(QString);
    void sendChar(QString);

public slots:
    void inputRequest(int);
    void enableEditing(bool);
    void setReadingLimit(int);
    void getInput();
    void clearConsole();
    void copyAll();


private slots:
    void onCursorChanged();
    void onSelectionChanged();
};

#endif // IOCONSOLE_H
