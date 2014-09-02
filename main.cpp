#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(":qdarkstyle/style.qss");

    if (!f.exists())
    {
        qDebug() << "QSS Error";
        QMessageBox::critical(0, "Error", "Failed to load application theme");
        exit(1);
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    MainWindow w;
    if (argc > 1)
        w.setOpenWith(QString(argv[1]));

    w.showMaximized();



    return a.exec();
}
