#ifndef REGISTERSMODEL_H
#define REGISTERSMODEL_H

#include <QAbstractTableModel>

class RegistersModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RegistersModel(QObject *parent = 0);

signals:

public slots:

};

#endif // REGISTERSMODEL_H
