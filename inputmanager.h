#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QDialog>

namespace Ui {
class InputManager;
}

class InputManager : public QDialog
{
    Q_OBJECT

public:
    explicit InputManager(QWidget *parent = 0);
    ~InputManager();

private:
    Ui::InputManager *ui;
};

#endif // INPUTMANAGER_H
