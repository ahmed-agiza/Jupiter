#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QDialog>
#include <QPushButton>
#include <QVector>
#include <QTableWidgetItem>
#include "memory.h"
#include "keyboardmanager.h"

namespace Ui {
class InputManager;
}

class InputManager : public QDialog
{
    Q_OBJECT

public:
    explicit InputManager(QWidget *parent, Memory *memory);
    bool eventFilter(QObject *o, QEvent *e);
    //void keyPressEvent(QKeyEvent *e);
    //void keyReleaseEvent(QKeyEvent *e);
    ~InputManager();

private slots:
    void on_pushButton_pressed();
    void refreshTable();
signals:
    void buttonPressed(int,int,bool);
private:
    QVector< QPushButton* > buttons;
    Ui::InputManager *ui;
    void handleKeyPress(int key, Qt::KeyboardModifiers modifiers);
    void handleKeyRelease(int key, Qt::KeyboardModifiers modifiers);
    Memory *mem;
    KeyboardManager keyman;\
    QTableWidgetItem* tablePointers[2][8];
    bool joystickNumber;
};

#endif // INPUTMANAGER_H
