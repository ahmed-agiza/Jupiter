#ifndef INSTRUCTIONSMODEL_H
#define INSTRUCTIONSMODEL_H

#include <QAbstractTableModel>

class InstructionsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit InstructionsModel(QObject *parent = 0);

signals:

public slots:

};

#endif // INSTRUCTIONSMODEL_H
