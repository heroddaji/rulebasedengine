#include "RbeItemTree_WidgetItem.hpp"
#include "RbeGeneralItem.hpp"

RbeItemTree_WidgetItem::RbeItemTree_WidgetItem(int type)
  :QTreeWidgetItem(type)
{
  mId = -1;
}

RbeItemTree_WidgetItem::RbeItemTree_WidgetItem(QTreeWidget * parent, int type)
 :QTreeWidgetItem(parent,type)
{
 mId = -1;
}

RbeItemTree_WidgetItem::RbeItemTree_WidgetItem(QTreeWidgetItem * parent, int type)
  :QTreeWidgetItem(parent,type)
{
  mId = -1;
}

RbeItemTree_WidgetItem::~RbeItemTree_WidgetItem()
{
  dataItem = NULL;
}

