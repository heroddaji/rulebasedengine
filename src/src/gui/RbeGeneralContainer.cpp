#include "RbeGeneralContainer.hpp"

#include "RbeGeneralItem.hpp"
#include "RbeItemTree.hpp"
#include "RbePropertyTree.hpp"
#include "RbeItemTree_WidgetItem.hpp"
#include "RbeVisualizeWidget.hpp"
#include "RbeVisualizeWidget_GraphicsScene.hpp"
#include "RbeVisualizeWidget_RuleTree.hpp"
#include "RbeVisualizeWidget_LineItem.hpp"
#include "RbeVisualizeWidget_PolygonItem.hpp"
#include "MainWindow.hpp"


RbeGeneralContainer::RbeGeneralContainer(QObject *parent) :
    QObject(parent)
{
  mLinkToPropertyTree = NULL;
  mLinkToVisualizeWidget = NULL;
  mLinkToItemTree = NULL;
}

QHash<uint,RbeGeneralItem *>* RbeGeneralContainer::getGeneralContextItems()
{
  return &mGeneralContextItems;
}

QHash<uint,RbeGeneralItem *>* RbeGeneralContainer::getGeneralRuleItems()
{
  return &mGeneralRuleItems;
}

void RbeGeneralContainer::setLinkToPropertyTree(RbePropertyTree *link)
{
  mLinkToPropertyTree = link;
}

void RbeGeneralContainer::setLinkToItemTree(RbeItemTree *link)
{
  mLinkToItemTree = link;
}

void RbeGeneralContainer::setLinkToVisualizeWidget(RbeVisualizeWidget *link)
{
  mLinkToVisualizeWidget = link;    
}

void RbeGeneralContainer::setLinkToVisualizeWidget_Scene(RbeVisualizeWidget_GraphicsScene *link)
{
  mLinkToVisualizeWidget_Scene = link;
}

void RbeGeneralContainer::setLinkToVisualizeWidget_RuleTree(RbeVisualizeWidget_RuleTree *link)
{
  mLinkToVisualizeWidget_RuleTree = link;
}

void RbeGeneralContainer::setLinkToMainWindow(MainWindow*link)
{
  mLinkToMainWindow = link;
}

void RbeGeneralContainer::update(UpdateType type)
{
  if(type == LOAD_CONTEXT)
  {
    Q_ASSERT(mLinkToPropertyTree != NULL);
    Q_ASSERT(mLinkToItemTree!= NULL);
    Q_ASSERT(mLinkToVisualizeWidget != NULL);    
    
    mLinkToItemTree->setupGuiFromContainer(this);    
    mLinkToVisualizeWidget->setupGuiFromContextItems(this);    
  }
  
  if(type == LOAD_RULE)
  {
    Q_ASSERT(mLinkToPropertyTree != NULL);
    Q_ASSERT(mLinkToItemTree!= NULL);
    Q_ASSERT(mLinkToVisualizeWidget != NULL);    
    
    mLinkToItemTree->setupGuiFromContainer(this);    
  }
  
  if(type == ADD_NEW_RULE)
  {
    if(!generateNewRule())
    {
      Q_ASSERT(false);
    }
  }
}

bool RbeGeneralContainer::setUpSignalSlot()
{
  if( mLinkToPropertyTree == NULL) return false;
  if( mLinkToVisualizeWidget == NULL) return false;
  if( mLinkToItemTree == NULL) return false;
  
  //signal to handle event from item tree
  this->connect(mLinkToItemTree,
                SIGNAL(itemPressed(QTreeWidgetItem*,int)),
                this,
                SLOT(handleEventsFromItemTree(QTreeWidgetItem*,int)));
  
  this->connect(mLinkToItemTree,
                SIGNAL(deleteItemSignal(QTreeWidgetItem *, int , QString)),
                this,
                SLOT(handleEventsFromItemTree(QTreeWidgetItem*,int,QString)));
      
  ////signal to handle event from ruleTree
  this->connect(mLinkToVisualizeWidget_RuleTree,
                SIGNAL(itemPressed(QTreeWidgetItem*,int)),
                this,
                SLOT(handleEventsFromRuleTree(QTreeWidgetItem*,int)));
  
  //signal to handle event from property tree
  this->connect(mLinkToPropertyTree,
                SIGNAL(somethingChangedInPropertyTree(GeneralDataFromPropertyTree *)),
                this,
                SLOT(handleEventsFromPropertyTree(GeneralDataFromPropertyTree *)));
  
  //for graphics change, cannot use singal/slots, so
  //the event handler is call from the pointer in graphicScene
  
  return true;
}

//handle click event from itemTree
void RbeGeneralContainer::handleEventsFromItemTree(QTreeWidgetItem* widgetItem,int column)
{   
  RbeItemTree_WidgetItem *clickedWidgetItem = static_cast<RbeItemTree_WidgetItem *>(widgetItem);
  mLinkToItemTree->mCurrentFocusItem = clickedWidgetItem;
  
  //if click on top item, show all items to the graphicScene, show no property
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::TOP_ITEM)
  {
    if(clickedWidgetItem->text(0) == "Contexts")
    {
      mLinkToVisualizeWidget->makeDrawViewVisile();
    }
    
    if(clickedWidgetItem->text(0) == "Rules")
    {      
      mLinkToVisualizeWidget->makeRuleTreeVisile();      
      mLinkToVisualizeWidget_RuleTree->showRuleInstruction();      
    }    
    mLinkToPropertyTree->clear();
  }
  
  //if click on context item, show drawScene , focus on drawItem, and show property  
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::CONTEXT_ITEM)
  {
    mLinkToVisualizeWidget->makeDrawViewVisile();
    mLinkToVisualizeWidget->getDrawScene()->focusOnItem(clickedWidgetItem->getID());    
    
    RbeGeneralItem *gItem = mGeneralContextItems.value(clickedWidgetItem->getID());
    
    if(gItem->getType() == RbeGeneralItem::AREA)
      mLinkToPropertyTree->showPropertyPolygon(gItem);
    
    if(gItem->getType() == RbeGeneralItem::TRIPWIRE)
      mLinkToPropertyTree->showPropertyLine(gItem);
    
  }
  
  //if click on rule item
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::RULE_ITEM)
  {
    mLinkToVisualizeWidget->makeRuleTreeVisile();    
    
    mLinkToPropertyTree->showPropertyRule(mGeneralRuleItems.value(clickedWidgetItem->getID()));
    
    mLinkToVisualizeWidget_RuleTree->setEventContainer(mGeneralRuleItems.value(clickedWidgetItem->getID())->getEventContainer());
    mLinkToVisualizeWidget_RuleTree->showEventsInsideContainer();
  }
}

//handle other events from item tree
void RbeGeneralContainer::handleEventsFromItemTree(QTreeWidgetItem* widgetItem,int column,QString action)
{   
  if(action == "DeleteItem")
  {        
    RbeItemTree_WidgetItem *wItem = static_cast<RbeItemTree_WidgetItem *>(widgetItem);            
    
    if(wItem->type() == RbeItemTree_WidgetItem::CONTEXT_ITEM)
    {      
      mGeneralContextItems.remove(wItem->getID());
      mLinkToVisualizeWidget_Scene->clear();
      mLinkToVisualizeWidget_Scene->setupGuiFromContextItems(this);
    }
    
    if(wItem->type() == RbeItemTree_WidgetItem::RULE_ITEM)
    {      
      mGeneralRuleItems.remove(wItem->getID());
      mLinkToVisualizeWidget_RuleTree->showRuleInstruction();
    }
    
    mLinkToPropertyTree->clear();
  }
}


void RbeGeneralContainer::handleEventsFromRuleTree(QTreeWidgetItem* widgetItem, int column)
{
  RbeItemTree_WidgetItem *clickedWidgetItem = static_cast<RbeItemTree_WidgetItem *>(widgetItem);
  
  //if click on eventContainer, show its property
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
  {
    EventContainerStruct *container = static_cast<EventContainerStruct *>(clickedWidgetItem->dataItem);
    mLinkToPropertyTree->showPropertyEventContainer(container);
  }
  
  //if click on event, show its property
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::EVENT_ITEM)
  {
    EventStruct *event;
    event = static_cast<EventStruct *>(clickedWidgetItem->dataItem);    
    mLinkToPropertyTree->showPropertyEvent(event);
  }
  
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::EVENT_FILTER_ITEM)
  {
    EventFilterStruct *filter;
    filter = static_cast<EventFilterStruct *>(clickedWidgetItem->dataItem);    
    mLinkToPropertyTree->showPropertyEventFilter(filter);
  }
  
  if(clickedWidgetItem->type() == RbeItemTree_WidgetItem::ACTION_ITEM)
  {
    ActionStruct *action = static_cast<ActionStruct *>(clickedWidgetItem->dataItem);    
    mLinkToPropertyTree->showPropertyAction(action);
  }
}

void RbeGeneralContainer::handleEventsFromPropertyTree(GeneralDataFromPropertyTree *generalDataFromPropertyTree)
{
//  //update if eventcombination property changed
  if(generalDataFromPropertyTree->itemType == RbeGeneralItem::EVENTCONTAINTER )
  {
    //RECREATE THE EVENT_CONTAINER
    EventContainerStruct *eventContainer = static_cast<EventContainerStruct*>(generalDataFromPropertyTree->pointerToItem);    
    RbeGeneralItem item;
    item.generateEventContainer(eventContainer,eventContainer->combinationType);
        
    mLinkToVisualizeWidget_RuleTree->showEventsInsideContainer();
    mLinkToPropertyTree->showPropertyEventContainer(eventContainer);    
  }
  
  //update if event or action property changed
  if( generalDataFromPropertyTree->itemType == RbeGeneralItem::EVENT ||
      generalDataFromPropertyTree->itemType == RbeGeneralItem::EVENT_FILTER ||       
      generalDataFromPropertyTree->itemType == RbeGeneralItem::ACTION  )     
  {    
    mLinkToVisualizeWidget_RuleTree->showEventsInsideContainer();
  }
  
  if(generalDataFromPropertyTree->itemType == RbeGeneralItem::RULE)
  {
    //refresh itemTree
    mLinkToItemTree->setupGuiFromContainer(this);
  }
  
  //update if it is an area
  if(generalDataFromPropertyTree->itemType == RbeGeneralItem::AREA )
  {
    RbeGeneralItem *generalItem = static_cast<RbeGeneralItem *>(generalDataFromPropertyTree->pointerToItem);    
    RbeVisualizeWidget_PolygonItem *polygon = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem *>(mLinkToVisualizeWidget_Scene->getHashTable()->value(generalItem->getID()));
    polygon->setColor(generalItem->getColor());
    
    //refresh itemTree
    mLinkToItemTree->setupGuiFromContainer(this);
  }
  
  if(generalDataFromPropertyTree->itemType == RbeGeneralItem::TRIPWIRE)
  {
    //refresh itemTree
    mLinkToItemTree->setupGuiFromContainer(this);
  }
    
}

void RbeGeneralContainer::handleEventsFromDrawScene(QGraphicsItem *item, uint itemID, int eType)
{
  if(item == NULL)
    return;
    
  if(eType == RbeVisualizeWidget_GraphicsScene::MOUSE_PRESS)
  { 
    //show property of item when the graphic item get clicked
    if(item->type() == RbeVisualizeWidget_GraphicsScene::POLYGON_ITEM)
    {
      mLinkToPropertyTree->showPropertyPolygon(mGeneralContextItems.value(itemID));        
    }
    
    if(item->type() == RbeVisualizeWidget_GraphicsScene::LINE_ITEM)
    {
      mLinkToPropertyTree->showPropertyLine(mGeneralContextItems.value(itemID));        
    }    
  }
  
  if(eType == RbeVisualizeWidget_GraphicsScene::MOUSE_MOVE)
  {    
    //update property when graphic items move around
    if(item->type() == RbeVisualizeWidget_GraphicsScene::LINE_ITEM)
    {
      RbeVisualizeWidget_LineItem *line = qgraphicsitem_cast<RbeVisualizeWidget_LineItem *>(item);
      RbeGeneralItem *gItem = mGeneralContextItems.value(itemID);
      
      //update new item coordinate
      for(int i = 0 ; i < line->getMapToScenePoints().size(); i++)
      {
        gItem->setPoints(i,line->getMapToScenePoints()[i]);
      }          
    }
    
    if(item->type() == RbeVisualizeWidget_GraphicsScene::POLYGON_ITEM)
    {
      RbeVisualizeWidget_PolygonItem *polygon = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem *>(item);
      RbeGeneralItem *gItem = mGeneralContextItems.value(itemID);
      
      //update new item coordinate
      for(int i = 0 ; i < polygon->getMapToScenePoints().size(); i++)
      {
        gItem->setPoints(i,polygon->getMapToScenePoints()[i]);
      }
    }
  }
  
  if(eType == RbeVisualizeWidget_GraphicsScene::MOUSE_RELEASE)
  {    
    //mLinkToPropertyTree->showProperty(itemID,mGeneralContextItems.value(itemID),0);
    
    if(item->type() == RbeVisualizeWidget_GraphicsScene::POLYGON_ITEM)
    {
      mLinkToPropertyTree->showPropertyPolygon(mGeneralContextItems.value(itemID));  
    }
    
    if(item->type() == RbeVisualizeWidget_GraphicsScene::LINE_ITEM)
    {
      mLinkToPropertyTree->showPropertyLine(mGeneralContextItems.value(itemID));  
    }
  }
  
  if(eType == RbeVisualizeWidget_GraphicsScene::NEW_ITEM)
  {    
    bool value = false;
    //Add new general item here
    if(item->type() == RbeVisualizeWidget_GraphicsScene::LINE_ITEM)
    {
      RbeVisualizeWidget_LineItem *line = qgraphicsitem_cast<RbeVisualizeWidget_LineItem *>(item);
            
      RbeGeneralItem *gItem = new RbeGeneralItem();      
      gItem->setID(generateID(&mGeneralContextItems));
      gItem->setName("Tripwire " + QString::number(gItem->getID()) );
      gItem->setType(RbeGeneralItem::TRIPWIRE);
      gItem->setDesc("This is a Tripwire context");      
      
      for(int i = 0 ; i < line->getMapToScenePoints().size(); i++)
      {
        gItem->addPoint(line->getMapToScenePoints()[i]);
      }
      
      //add new item to container,itemTree and possible propertyTree      
      mGeneralContextItems.insert(gItem->getID(),gItem);
      mLinkToItemTree->generateNewTreeItem(gItem,gItem->getType());      
      mLinkToPropertyTree->showPropertyLine(gItem);
      
      //set correct ID for graphic Item
      line->setID(gItem->getID());
      
      value = true;
    }
    
    if(item->type() == RbeVisualizeWidget_GraphicsScene::POLYGON_ITEM)
    {
      RbeVisualizeWidget_PolygonItem *polygon = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem *>(item);
      
      RbeGeneralItem *gItem = new RbeGeneralItem();      
      gItem->setID(generateID(&mGeneralContextItems));
      gItem->setName("Area " + QString::number(gItem->getID()) );
      gItem->setType(RbeGeneralItem::AREA);
      gItem->setDesc("This is an Area context");      
      gItem->setColor(polygon->getColor());
      for(int i = 0 ; i < polygon->getMapToScenePoints().size(); i++)
      {
        gItem->addPoint(polygon->getMapToScenePoints()[i]);
      }
      
      //add new item to container,itemTree and possible propertyTree      
      mGeneralContextItems.insert(gItem->getID(),gItem);
      mLinkToItemTree->generateNewTreeItem(gItem,RbeGeneralItem::CONTEXT);
      
      mLinkToPropertyTree->showPropertyPolygon(gItem);
      
      //set correct ID for graphic Item
      polygon->setID(gItem->getID());
      
      value = true;
    }
    
    if(value == true)
    {
      //if success in adding a new graphic item, the 
      //window bar should change back to selection mode
      mLinkToMainWindow->restoreContextItemsButtonGroupToSelectionMode();      
    }
  }
  
  if(eType == RbeVisualizeWidget_GraphicsScene::BACK_TO_SELECTION_MODE)
  { 
    mLinkToMainWindow->restoreContextItemsButtonGroupToSelectionMode();      
  }
  
  if(eType == RbeVisualizeWidget_GraphicsScene::DELETE_ITEM)
  { 
    //delete item in the hash, clear the property and remove the itemTree
    mLinkToPropertyTree->cleanTheEditor();
    mGeneralContextItems.remove(itemID);
    mLinkToItemTree->setupGuiFromContainer(this);
  }
}

uint RbeGeneralContainer::generateID(QHash<uint,RbeGeneralItem *> *container)
{
  if(container->keys().size() == 0)
  {
    return 1;
  }
  
  Q_ASSERT(container->keys().size() == container->uniqueKeys().size());  
  QList<uint>keyList = container->uniqueKeys();
  uint finalKey = keyList[0];
  
  for(int i = 0 ; i < keyList.size(); i++)
  {
    uint value = keyList[i];
    if(finalKey <= value)
    {
      finalKey = value;
    }
  }
    
  return finalKey+1;
}

bool RbeGeneralContainer::generateNewRule()
{
  RbeGeneralItem *ruleItem = new RbeGeneralItem();
  ruleItem->setType(RbeGeneralItem::RULE);
  ruleItem->setID(this->generateID(&mGeneralRuleItems));
  ruleItem->setName("Rule " + QString::number(ruleItem->getID()));
  ruleItem->setDesc("This is a Rule");
  ruleItem->generateEventContainer(&ruleItem->eventContainer,"ONE_EVENT");
    
  mGeneralRuleItems.insert(ruleItem->getID(),ruleItem);  
  
  mLinkToItemTree->generateNewTreeItem(ruleItem,RbeGeneralItem::RULE);
  
  return true;
}

void RbeGeneralContainer::clearRuleTree()
{
  mLinkToVisualizeWidget_RuleTree->clear();
}


void RbeGeneralContainer::clearContextItems()
{
  mGeneralContextItems.clear();  
  mLinkToVisualizeWidget->getDrawScene()->clearAllItems();
}

void RbeGeneralContainer::clearRuleItems()
{
  mGeneralRuleItems.clear();  
}

void RbeGeneralContainer::clearAllItems()
{
  mGeneralContextItems.clear();
  mGeneralRuleItems.clear();
    
  mLinkToItemTree->clearAllItems();
  mLinkToVisualizeWidget->getDrawScene()->clearAllItems();
}

RbeGeneralContainer::~RbeGeneralContainer()
{
  mLinkToPropertyTree = NULL;
  mLinkToVisualizeWidget = NULL;
  mLinkToItemTree = NULL;
    
}
