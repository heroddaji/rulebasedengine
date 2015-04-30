#ifndef RBEITEMTREE_WIDGETITEM_HPP
#define RBEITEMTREE_WIDGETITEM_HPP

#include <QTreeWidget>
#include <QTreeWidgetItem>

class RbeGeneralItem;
class RbeItemTree_WidgetItem : public QTreeWidgetItem
{  
 
public:
  enum {TOP_ITEM = QTreeWidgetItem::UserType +1, CONTEXT_ITEM, RULE_ITEM, EVENT_CONTAINER_ITEM, EVENT_ITEM,ACTION_ITEM, EVENT_FILTER_ITEM };
  
  RbeItemTree_WidgetItem(int type = Type );
  RbeItemTree_WidgetItem(QTreeWidget * parent, int type = Type );
  RbeItemTree_WidgetItem(QTreeWidgetItem * parent, int type = Type );
  ~RbeItemTree_WidgetItem();
  void setID(uint id){mId = id;}
  uint getID(){return mId;}
  void *dataItem;
  
private:  
  uint mId;
  ItemType mType;
  
};

#endif // RBEITEMTREE_WIDGETITEM_HPP
