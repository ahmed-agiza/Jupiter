#ifndef FILELOADER_H
#define FILELOADER_H

#include <QDialog>

enum FileDialogType{AddFile, CreateFile};

class FileLoader : public QDialog
{
    Q_OBJECT
public:
    explicit FileLoader(QWidget *parent = 0, FileDialogType type = AddFile);

private:
    FileDialogType dialogType;

signals:

public slots:

};

#endif // FILELOADER_H
