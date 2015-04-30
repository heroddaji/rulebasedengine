#include "RbeGeneralItem.hpp"

#include "src/gui/RbeVisualizeWidget_GraphicsScene.hpp"
#include "src/gui/RbeItemTree_WidgetItem.hpp"
#include "src/gui/RbePropertyTree.hpp"


RbeGeneralItem::RbeGeneralItem(QObject *parent) :
  QObject(parent)
{
  mType = NOTYPE;  
  mColor = QColor(Qt::green);
  eventContainer.combinationType = "AND";
  eventContainer.combinationSecond = 0;
  eventContainer.linkToMama = NULL;
}

void RbeGeneralItem::setType(QString type)
{
  if(type == "AREA")
    mType = AREA;
  else if(type == "TRIPWIRE")
    mType = TRIPWIRE;
  else if(type == "RULE")
    mType = RULE;
  else if(type == "NOTYPE")
    mType = NOTYPE;
  else    
    Q_ASSERT(false);
}

 void RbeGeneralItem::setPoints(int index, QPoint p)
 {
   //bug: if polygon increase size (keyEvent), then error
   if(index >= mPoints.size())
   {
     addPoint(p);
   }
   
   mPoints[index] = p;
 }

 void RbeGeneralItem::generateEventContainer(EventContainerStruct *eventContainer,QString type)
 {
  
     if(type != "ONE_EVENT")
     {
       eventContainer->events.clear();
       eventContainer->actions.clear();     
       eventContainer->combinationType = type;       
       
       EventStruct event1;
       EventStruct event2;
       event1.linkToMama = NULL;
       event1.eventType = "ENTER_AREA";
       event1.linkToMama = eventContainer;
       
       event2.linkToMama = NULL;
       event2.eventType = "CROSSING_TRIPWIRE";
       event2.linkToMama = eventContainer;
       
       eventContainer->events.push_back(event1);
       eventContainer->events.push_back(event2);
      
       ActionStruct action;
       action.linkToMama = NULL;
       action.linkToPapa = NULL;
       action.actionType = "PRINT";
       action.value = "perform Print action";
       action.linkToMama = eventContainer;
           
       eventContainer->actions.push_back(action);
     }
     
     if(type == "ONE_EVENT")
     {
       eventContainer->events.clear();
       eventContainer->actions.clear();     
       eventContainer->combinationType = type;
       
       EventStruct event1;     
       event1.linkToMama = NULL;
       event1.eventType = "ENTER_AREA";     
       event1.linkToMama = eventContainer;
       eventContainer->events.push_back(event1);     
      
       ActionStruct action;
       action.linkToMama = NULL;
       action.linkToPapa = NULL;
       action.actionType = "PRINT";
       action.value = "perform Print action";
       action.linkToMama = eventContainer;
       
       eventContainer->actions.push_back(action);
     }
 }
