#include "keyboardmanager.h"

KeyboardManager::KeyboardManager()
{
    keyboardCode.resize(10);
    defaultValue.resize(10);
    keyName.resize(10);
    this->inputSettings = new QSettings("Mirage", "Input Settings");

    keyName[DOWN_KEY_INDEX] = "Down";
    keyName[LEFT_KEY_INDEX] = "Left";
    keyName[RIGHT_KEY_INDEX] = "Right";
    keyName[UP_KEY_INDEX] = "Up";
    keyName[A_KEY_INDEX] = "A";
    keyName[B_KEY_INDEX] = "B";
    keyName[START_KEY_INDEX] = "Start";
    keyName[SELECT_KEY_INDEX] = "Select";
    keyName[L_KEY_INDEX] = "L";
    keyName[R_KEY_INDEX] = "R";

    defaultValue[DOWN_KEY_INDEX] = Qt::Key_Down;
    defaultValue[LEFT_KEY_INDEX] = Qt::Key_Left;
    defaultValue[RIGHT_KEY_INDEX] = Qt::Key_Right;
    defaultValue[UP_KEY_INDEX] = Qt::Key_Up;
    defaultValue[A_KEY_INDEX] = Qt::Key_Z;
    defaultValue[B_KEY_INDEX] = Qt::Key_X;
    defaultValue[START_KEY_INDEX] = Qt::Key_Return;
    defaultValue[SELECT_KEY_INDEX] = Qt::Key_Backspace;
    defaultValue[L_KEY_INDEX] = Qt::Key_A;
    defaultValue[R_KEY_INDEX] = Qt::Key_D;

    LoadSettings();

}

KeyboardManager::~KeyboardManager()
{
    delete inputSettings;
}

QString KeyboardManager::getKeyName(int joystickButton)
{
    return keyName[joystickButton];
}

void KeyboardManager::LoadSettings()
{
    inputSettings->beginReadArray("joystick");
    for(int i=0; i<10; i++){
        inputSettings->setArrayIndex(i);
        keyboardCode[i] = inputSettings->value(QString::number(i), defaultValue[i]).toInt();
        joystickCode[keyboardCode[i]] = i;
    }
    inputSettings->endArray();
}

void KeyboardManager::SaveSettings()
{
    inputSettings->beginWriteArray("joystick",10);
    for(int i=0; i<10; i++){
        inputSettings->setArrayIndex(i);
        inputSettings->setValue(QString::number(i), keyboardCode[i]);
    }
    inputSettings->endArray();
}


bool KeyboardManager::contains(int keyboardButton)
{
    return (this->joystickCode.contains(keyboardButton));
}

int KeyboardManager::getJoystickKeyCode(int keyboardButton)
{
    return (joystickCode[keyboardButton]);
}

int KeyboardManager::getKeyboardKeyCode(int joystickButton)
{
    return keyboardCode[joystickButton];
}

void KeyboardManager::changeButton(int joystickButton, int keyboardButton)
{
    joystickCode.remove(keyboardCode[joystickButton]);
    keyboardCode[joystickButton] = keyboardButton;
    joystickCode[keyboardButton] = joystickButton;
    SaveSettings();
}
