#include "explorertreeitem.h"

ExplorerTreeItem::ExplorerTreeItem(QTreeWidget *parent) :
    QTreeWidgetItem(parent)
{
}

ExplorerTreeItem::ExplorerTreeItem(ExplorerTreeItem *parent):QTreeWidgetItem((QTreeWidgetItem *)parent)
{

}
