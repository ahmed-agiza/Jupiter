#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#ifndef KEYBOARD_BUTTONS
#define KEYBOARD_BUTTONS
#define DOWN_KEY_INDEX 0
#define LEFT_KEY_INDEX 1
#define RIGHT_KEY_INDEX 2
#define UP_KEY_INDEX 3
#define A_KEY_INDEX 4
#define B_KEY_INDEX 5
#define R_KEY_INDEX 6
#define L_KEY_INDEX 7
#define START_KEY_INDEX 8
#define SELECT_KEY_INDEX 9
#endif

#include <QMap>
#include <QVector>
#include <QSettings>

class KeyboardManager
{
public:
    KeyboardManager();
    ~KeyboardManager();
    bool contains(int);
    int getJoystickKeyCode(int);
    int getKeyboardKeyCode(int);
    QString getKeyName(int);
    void changeButton(int, int);
    void LoadSettings();
    void SaveSettings();
private:
    QMap< int, int > joystickCode;
    QVector<int> keyboardCode;
    QVector<int> defaultValue;
    QVector<QString> keyName;
    QSettings *inputSettings;
};

#endif // KEYBOARDMANAGER_H
