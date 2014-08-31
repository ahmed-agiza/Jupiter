#ifndef EXPLORERTREEITEM_H
#define EXPLORERTREEITEM_H

#include <QTreeWidgetItem>
#include <QObject>

class ExplorerTreeItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    ExplorerTreeItem(QTreeWidget *parent = 0);
    ExplorerTreeItem(ExplorerTreeItem *parent = 0);

signals:

public slots:

};

#endif // EXPLORERTREEITEM_H
