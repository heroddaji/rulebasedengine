#include "RbeVisualizeWidget_RuleTree.hpp"

#include "RbeGeneralItem.hpp"
#include "RbeItemTree_WidgetItem.hpp"


RbeVisualizeWidget_RuleTree::RbeVisualizeWidget_RuleTree(QWidget *parent)
  :QTreeWidget(parent)
{
  //Column lables
  QStringList labels;  
  labels << " ";
  header()->setResizeMode(QHeaderView::Interactive);  
  setHeaderLabels(labels);
  
  connect(this,
          SIGNAL(itemPressed(QTreeWidgetItem*,int)),
          this,SLOT(getClickedItem(QTreeWidgetItem*)));  
  
  
  mSelectedItem = NULL;
  
}

void RbeVisualizeWidget_RuleTree::setEventContainer(EventContainerStruct *container)
{
  mEventContainer = container;
  
}

EventContainerStruct* RbeVisualizeWidget_RuleTree::getEventContainer()
{
  return mEventContainer;
}


void RbeVisualizeWidget_RuleTree::getClickedItem(QTreeWidgetItem* item)
{
  mSelectedItem = static_cast<RbeItemTree_WidgetItem *>(item);  
}

void RbeVisualizeWidget_RuleTree::showRuleInstruction()
{
  this->clear();
  RbeItemTree_WidgetItem *topItem = new RbeItemTree_WidgetItem(this,RbeItemTree_WidgetItem::TOP_ITEM);
  topItem->setText(0, "Click on each rule to view its detail");
}

void RbeVisualizeWidget_RuleTree::showEventsInsideContainer()
{
  this->clear();
  if(mEventContainer == NULL) 
    return;
  
  mTopItem = new RbeItemTree_WidgetItem(this,RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM);  
  mTopItem->dataItem = mEventContainer;
  mTopItem->setText(0,mEventContainer->combinationType);
  mTopItem->setExpanded(true);  
  
  this->showAllEventsContainer(mTopItem,mEventContainer->containers);
  this->showAllEvents(mTopItem,mEventContainer->events);
  this->showAllActions(mTopItem,mEventContainer->actions);
}

void RbeVisualizeWidget_RuleTree::showAllEventsContainer(RbeItemTree_WidgetItem *item, QList<EventContainerStruct> &list)
{
  for(int i = 0; i < list.size(); i++)
  { 
    EventContainerStruct *eventContainer = &list[i];    
    RbeItemTree_WidgetItem *containerItem = new RbeItemTree_WidgetItem(item,RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM);   
    containerItem->dataItem = eventContainer;
    containerItem->setText(0,QString("Container: ") + QString(eventContainer->combinationType));
    containerItem->setExpanded(true);  
    
    //show all sub-container
    this->showAllEventsContainer(containerItem,eventContainer->containers);
    
    //show all sub-events
    this->showAllEvents(containerItem,eventContainer->events);
    
    //show all sub-actions
    this->showAllActions(containerItem,eventContainer->actions);
  }
}

void RbeVisualizeWidget_RuleTree::showAllEvents(RbeItemTree_WidgetItem *item, QList<EventStruct> &list)
{
  for(int i = 0; i < list.size(); i++)
  { 
    EventStruct *event = &list[i];    
    RbeItemTree_WidgetItem *eventItem = new RbeItemTree_WidgetItem(item,RbeItemTree_WidgetItem::EVENT_ITEM);   
    eventItem->dataItem = event;
    eventItem->setText(0,QString("Event: ") + QString(event->eventType));
    eventItem->setExpanded(true);     
    
    this->showAllEventFitlers(eventItem,event->filters);
    this->showAllActions(eventItem,event->actionsInsideEvent);
  }
}

void RbeVisualizeWidget_RuleTree::showAllEventFitlers(RbeItemTree_WidgetItem *item, QList<EventFilterStruct> &list)
{
  for(int i = 0; i < list.size(); i++)
  { 
    EventFilterStruct *eventFilter = &list[i];    
    RbeItemTree_WidgetItem *eventFilterItem = new RbeItemTree_WidgetItem(item,RbeItemTree_WidgetItem::EVENT_FILTER_ITEM);   
    eventFilterItem->dataItem = eventFilter;
    eventFilterItem->setText(0,QString("Filter: ") + QString(eventFilter->filterType));
    eventFilterItem->setExpanded(true);           
  }
}

void RbeVisualizeWidget_RuleTree::showAllActions(RbeItemTree_WidgetItem *item, QList<ActionStruct> &list)
{
  for(int i = 0; i < list.size(); i++)
  { 
    ActionStruct *action = &list[i];    
    RbeItemTree_WidgetItem *actionItem = new RbeItemTree_WidgetItem(item,RbeItemTree_WidgetItem::ACTION_ITEM);   
    actionItem->dataItem = action;
    actionItem->setText(0,QString("Action: ") + QString(action->actionType));
  }
}

void RbeVisualizeWidget_RuleTree::mousePressEvent(QMouseEvent *event)
{
  mItemPos = event->pos();
  //delegate, must be here
  QTreeWidget::mousePressEvent(event);  
  
  QMenu menu;  
  menu.clear();
  
  
  //if detect item with the right-click, show pop-up menu    
  if(event->button() == Qt::RightButton)
  {
    if(itemAt(event->pos()) != 0)
    {      
      RbeItemTree_WidgetItem *wItem = static_cast<RbeItemTree_WidgetItem *>(itemAt(event->pos()));      
      wItem->setSelected(true);
      
      QAction newContainer("Add new Container",this);  
      this->connect(&newContainer,SIGNAL(triggered()),this,SLOT(addNewContainers()));
      
      QAction newEvent("Add new Event",this);  
      this->connect(&newEvent,SIGNAL(triggered()),this,SLOT(addNewEvent()));      
      
      QAction newAction("Add new Action",this);  
      this->connect(&newAction,SIGNAL(triggered()),this,SLOT(addNewAction()));
      
      QAction newFilterAction("Add Event filter",this);
      connect(&newFilterAction,SIGNAL(triggered()),this,SLOT(addFilterAction()));
      
      QAction deleteAction("Delete",this);
      connect(&deleteAction,SIGNAL(triggered()),this,SLOT(deleteItem()));      
      
      if(wItem->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
      {        
        menu.addAction(&newContainer);
        menu.addAction(&newEvent);
        menu.addAction(&newAction);
      }      
      if(wItem->type() == RbeItemTree_WidgetItem::EVENT_ITEM)
      {        
        menu.addAction(&newAction);
        menu.addAction(&newFilterAction);
      } 
      
      if(wItem->type() == RbeItemTree_WidgetItem::ACTION_ITEM)
      {
        ;//nothing  
      } 
      
      menu.addAction(&deleteAction);
      menu.exec(event->globalPos());
    }
  } 
}

void RbeVisualizeWidget_RuleTree::deleteItem()
{
  RbeItemTree_WidgetItem *wItem = static_cast<RbeItemTree_WidgetItem *>(itemAt(mItemPos));    
  
  int index;
  if(wItem->parent() != NULL)
    index = wItem->parent()->indexOfChild(wItem);
  
  if(wItem->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
  {
    if(wItem->parent() == NULL)
    {
      //top item, delete only the events and actions and other container
      EventContainerStruct *container = static_cast<EventContainerStruct *>(wItem->dataItem);
      container->events.clear();
      container->actions.clear();
      container->containers.clear();
    }
    else
    {
      //delte the whole container
      EventContainerStruct *container = static_cast<EventContainerStruct *>(wItem->dataItem);
      container->events.clear();
      container->actions.clear();
      container->containers.clear();
      
      EventContainerStruct *containerMama = container->linkToMama;      
      containerMama->containers.removeAt(index);        
    }
  }
  
  if(wItem->type() == RbeItemTree_WidgetItem::EVENT_ITEM)
  {       
    //remove all actions inside event
    EventStruct *event = static_cast<EventStruct *>(wItem->dataItem);    
    event->actionsInsideEvent.clear();
    event->filters.clear();
    
    //get the event_container and remove this event from it
    EventContainerStruct *containerMama = event->linkToMama;    
    
    //need to minus the container size because they containers add up its index    
    containerMama->events.removeAt(index - containerMama->containers.size());    
  }
  
  if(wItem->type() == RbeItemTree_WidgetItem::EVENT_FILTER_ITEM)
  {
    EventFilterStruct *filter = static_cast<EventFilterStruct *>(wItem->dataItem);    
    
    if(filter->linkToPapa != NULL)
    {
      EventStruct *event = filter->linkToPapa;              
      event->filters.removeAt(index);    
    }
  }
  
  if(wItem->type() == RbeItemTree_WidgetItem::ACTION_ITEM)
  {
    ActionStruct *action = static_cast<ActionStruct *>(wItem->dataItem);    
    
    if(wItem->parent()->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
    {
      if(action->linkToMama != NULL)
      {
        EventContainerStruct *containerMama = action->linkToMama;              
        containerMama->actions.removeAt(index - containerMama->containers.size() - containerMama->events.size());          
      }       
    }
    
    if(wItem->parent()->type() == RbeItemTree_WidgetItem::EVENT_ITEM)
    {
      if(action->linkToPapa != NULL)
      {
        EventStruct *event = action->linkToPapa;              
        event->actionsInsideEvent.removeAt(index - event->filters.size());          
      }       
    }
  }
  
  showEventsInsideContainer();
}


void RbeVisualizeWidget_RuleTree::addNewContainers()
{
  if(mSelectedItem != NULL)
  {
    if(mSelectedItem->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
    { 
      EventContainerStruct *container = static_cast<EventContainerStruct *>(mSelectedItem->dataItem);
      if(container->combinationType == "ONE_EVENT")
      {       
        QMessageBox::warning(this, "Error","Cannot have multiple container in this container type"     );
        return;
        
      }
      
      EventContainerStruct childContainer;
      childContainer.linkToMama = NULL;
      childContainer.combinationType = "AND";
      childContainer.linkToMama = container;
      container->containers.push_back(childContainer);
    }    
    showEventsInsideContainer();
  }
  mSelectedItem = NULL;
  this->clearFocus();
}

void RbeVisualizeWidget_RuleTree::addNewEvent()
{
  if(mSelectedItem != NULL)
  {
    if(mSelectedItem->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
    { 
      EventContainerStruct *container = static_cast<EventContainerStruct *>(mSelectedItem->dataItem);      
      
      //validation, container type "ONE_EVENT" that already has more than 1 one event cannot add more events      
      if(container->combinationType == "ONE_EVENT")
      {
        if(container->events.size() == 1)  
        {
          QMessageBox::warning(this, "Error","Cannot have more than one even in this container type"     );
          return;
        }
      }
      
      EventStruct event;
      event.linkToMama = NULL;
      event.eventType = "ENTER_AREA";
      event.linkToMama = container;
      container->events.push_back(event);                  
    }
    
    showEventsInsideContainer();
  }
  mSelectedItem = NULL;
  this->clearFocus();
}

void RbeVisualizeWidget_RuleTree::addNewAction()
{
  if(mSelectedItem != NULL)
  {
    ActionStruct action;
    action.linkToMama = NULL;
    action.linkToPapa = NULL;
    action.actionType = "PRINT";     
    action.value = "perform action Print";
    
    if(mSelectedItem->type() == RbeItemTree_WidgetItem::EVENT_ITEM )
    {
      EventStruct *event = static_cast<EventStruct *>(mSelectedItem->dataItem);      
      action.linkToPapa = event;
      event->actionsInsideEvent.push_back(action);
    }
    
    if(mSelectedItem->type() == RbeItemTree_WidgetItem::EVENT_CONTAINER_ITEM)
    {
      EventContainerStruct *container = static_cast<EventContainerStruct *>(mSelectedItem->dataItem);      
      action.linkToMama = container;      
      container->actions.push_back(action);
    }
    
    showEventsInsideContainer();
  }
  
  mSelectedItem = NULL;  
  this->clearFocus();
}

void RbeVisualizeWidget_RuleTree::addFilterAction()
{
  if(mSelectedItem != NULL)
  {
    if(mSelectedItem->type() == RbeItemTree_WidgetItem::EVENT_ITEM )
    {
      EventStruct *event = static_cast<EventStruct *>(mSelectedItem->dataItem);      
      
      EventFilterStruct eventFilter;
      eventFilter.linkToPapa = NULL;      
      eventFilter.filterType = "CONTEXT_ID";
      eventFilter.value = 1;
      eventFilter.linkToPapa = event;
      
      event->filters.push_back(eventFilter);
    }      
    showEventsInsideContainer();
  }  
  
  mSelectedItem = NULL;
  this->clearFocus();
}

RbeVisualizeWidget_RuleTree::~RbeVisualizeWidget_RuleTree()
{  
}

