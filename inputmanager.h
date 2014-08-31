#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QDialog>
#include "memory.h"

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

private:
    Ui::InputManager *ui;
    void handleKeyPress(int key, Qt::KeyboardModifiers modifiers);
    void handleKeyRelease(int key, Qt::KeyboardModifiers modifiers);
    Memory *mem;
};

#endif // INPUTMANAGER_H
