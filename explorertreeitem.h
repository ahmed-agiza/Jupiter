#ifndef EXPLORERTREEITEM_H
#define EXPLORERTREEITEM_H

#include <QTreeWidgetItem>
#include <QObject>

enum ExplorerItemType{PROJECT_TITLE, TEXT_ROOT, TEXT_CHILD, TEXT_MAIN, DATA_ROOT, DATA_CHILD, RES_ROOT, RES_CHILD, UNKOWN_TYPE};

class ExplorerTreeItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    ExplorerTreeItem(QTreeWidget *parent = 0, ExplorerItemType nType = UNKOWN_TYPE);
    ExplorerTreeItem(ExplorerTreeItem *parent = 0, ExplorerItemType nType = UNKOWN_TYPE);
    ExplorerItemType getItemType();
    void setItemType(ExplorerItemType);

private:
    ExplorerItemType type;

signals:

public slots:

};

#endif // EXPLORERTREEITEM_H
