#include "RbeItemTree.hpp"
#include "RbeItemTree_WidgetItem.hpp"
#include "RbeGeneralContainer.hpp"
#include "RbeGeneralItem.hpp"

RbeItemTree::RbeItemTree(QWidget *parent)
  : QTreeWidget(parent)
{
  mTopContextItem = NULL;
  mTopRuleItem = NULL;
  //Column lables
  QStringList labels;  
  labels << " ";
  header()->setResizeMode(QHeaderView::Interactive);  
  setHeaderLabels(labels);
  
  //setupTopItems();
}

void RbeItemTree::setupTopItems()
{ 
  if(this->topLevelItemCount() == 0)
  {
    mTopContextItem = NULL;
    mTopRuleItem = NULL;  
    
    mTopContextItem = new RbeItemTree_WidgetItem(this,RbeItemTree_WidgetItem::TOP_ITEM);
    mTopContextItem->setText(0,"Contexts");
    mTopContextItem->setExpanded(true);  
    
    mTopRuleItem = new RbeItemTree_WidgetItem(this,RbeItemTree_WidgetItem::TOP_ITEM);
    mTopRuleItem->setText(0,"Rules");
    mTopRuleItem->setExpanded(true);    
  }
}

void RbeItemTree::setupGuiFromContainer(RbeGeneralContainer *container)
{      
  this->clear();
  setupTopItems();
  
  QHash<uint,RbeGeneralItem *> *containerHash;
    
  //push new items to tree
  {
    containerHash = container->getGeneralRuleItems();
    QList<uint>keys = containerHash->uniqueKeys();  
    for(int i = 0 ; i < keys.size(); i++)
    {
      uint aKey = keys[i];
      RbeGeneralItem *generalItem = containerHash->value(aKey);        
      
      this->generateNewTreeItem(generalItem,generalItem->getType());    
    }  
  }
  
  {
    containerHash = container->getGeneralContextItems();
    QList<uint>keys = containerHash->uniqueKeys();  
    for(int i = 0 ; i < keys.size(); i++)
    {
      uint aKey = keys[i];
      RbeGeneralItem *generalItem = containerHash->value(aKey);        
      
      this->generateNewTreeItem(generalItem,generalItem->getType());    
    }  
  }
}

void RbeItemTree::generateNewTreeItem(RbeGeneralItem *generalItem, int type)
{
  setupTopItems();
  RbeItemTree_WidgetItem *widgetItem;
  
  if(type == RbeGeneralItem::RULE)
  {
    widgetItem = new RbeItemTree_WidgetItem(mTopRuleItem,RbeItemTree_WidgetItem::RULE_ITEM);         
    widgetItem->setID(generalItem->getID());
    widgetItem->dataItem = generalItem;
  }
  
  if(type == RbeGeneralItem::AREA || type == RbeGeneralItem::TRIPWIRE || type == RbeGeneralItem::CONTEXT)
  {
    widgetItem = new RbeItemTree_WidgetItem(mTopContextItem,RbeItemTree_WidgetItem::CONTEXT_ITEM);         
    widgetItem->setID(generalItem->getID());
    widgetItem->dataItem = generalItem;
  }
  
  //set value to display the widgetItem name and ID on widgettree    
  widgetItem->setText(0,generalItem->getName());    
  widgetItem->setText(1,QString::number(widgetItem->getID()));   
  
  mCurrentFocusItem = widgetItem;
}

void RbeItemTree::mousePressEvent ( QMouseEvent * event )
{
  //delegate, must be here
  QTreeWidget::mousePressEvent(event);
  
  //qDebug() << itemAt(event->pos());
  
  //if detect item with the right-click, show pop-up menu
  if(itemAt(event->pos()) != 0)
  {
    mItemPos = event->pos();
        
    if(event->button() == Qt::RightButton)
    {
      QMenu menu;        
      
      QAction deleteAction("Delete",this);
      connect(&deleteAction,SIGNAL(triggered()),this,SLOT(deleteItem()));
      
      menu.addAction(&deleteAction);
      menu.exec(event->globalPos());
    }
  }
}

void RbeItemTree::deleteItem()
{
  RbeItemTree_WidgetItem *wItem = static_cast<RbeItemTree_WidgetItem *>(itemAt(mItemPos));
  qDebug() << wItem->type();
  
  if(wItem->type() == RbeItemTree_WidgetItem::TOP_ITEM)
    return;
  
  if(wItem->type() == RbeItemTree_WidgetItem::CONTEXT_ITEM)
  {
    wItem->dataItem = NULL;
    mTopContextItem->removeChild(wItem);
    
    //notify to general controller to delete on other view
    emit deleteItemSignal(wItem,0,"DeleteItem");
    
    delete wItem;    
  }
  
  if(wItem->type() == RbeItemTree_WidgetItem::RULE_ITEM)
  {
    wItem->dataItem = NULL;
    mTopRuleItem->removeChild(wItem);
    
    //notify to general controller to delete on other view
    emit deleteItemSignal(wItem,0,"DeleteItem");
    
    delete wItem;        
  }
}

void RbeItemTree::clearAllItems()
{
  this->clear();
}

RbeItemTree::~RbeItemTree()
{  
  mTopContextItem = NULL;
  mTopRuleItem = NULL;
}
