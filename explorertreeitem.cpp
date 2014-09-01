#include "explorertreeitem.h"

ExplorerTreeItem::ExplorerTreeItem(QTreeWidget *parent, ExplorerItemType nType) :
    QTreeWidgetItem(parent){
    type = nType;
}

ExplorerTreeItem::ExplorerTreeItem(ExplorerTreeItem *parent, ExplorerItemType nType):QTreeWidgetItem((QTreeWidgetItem *)parent){
    type = nType;
}

ExplorerItemType ExplorerTreeItem::getItemType(){
    return type;
}

void ExplorerTreeItem::setItemType(ExplorerItemType nType){
    type = nType;
}
