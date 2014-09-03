#ifndef COMPLETERLIST_H
#define COMPLETERLIST_H

#include <QListView>
#include <QPaintEvent>

class CompleterList : public QListView
{
    Q_OBJECT
public:
    explicit CompleterList(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *e);

};

#endif // COMPLETERLIST_H
