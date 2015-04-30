#include "src/gui/RbeXmlHandler.hpp"
#include "src/gui/RbeGeneralItem.hpp"
#include "src/gui/RbeGeneralContainer.hpp"

RbeXmlHandler::RbeXmlHandler()
{  
}

void RbeXmlHandler::read(QFile *file)
{      
  if(!mDomDocument.setContent(file))
  {    
    Q_ASSERT(false);        
  }  
  
  QDomElement root = mDomDocument.documentElement();  
  
  if(root.tagName() == "rbe_project")
  {
    mLinkToGeneralContainer->clearContextItems();
    mLinkToGeneralContainer->clearRuleItems();
    this->readProject(root);
    
    mLinkToGeneralContainer->update(RbeGeneralContainer::LOAD_CONTEXT);
    mLinkToGeneralContainer->update(RbeGeneralContainer::LOAD_RULE);
  }
  
  if(root.tagName() == "contexts")
  {     
    //before read new context content,
    //clear all contexts item first
    mLinkToGeneralContainer->clearContextItems();
    this->readContexts(root);
    
    //updateGUI
    mLinkToGeneralContainer->update(RbeGeneralContainer::LOAD_CONTEXT);
  }
  if(root.tagName() == "rules")
  {    
    //before read new rule content,
    //clear all contexts item first
    mLinkToGeneralContainer->clearRuleItems();
    this->readRules(root);
    
    //updateGUI
    mLinkToGeneralContainer->update(RbeGeneralContainer::LOAD_RULE);
  }  
}

void RbeXmlHandler::readProject(QDomElement root)
{  
  QDomElement contextsNode = root.firstChildElement("contexts");
  this->readContexts(contextsNode);
  
  QDomElement rulesNode = root.firstChildElement("rules");
  this->readRules(rulesNode);  
}

void RbeXmlHandler::readContexts(QDomElement root)
{  
  QDomElement contextNode = root.firstChildElement("context");
  
  while (!contextNode.isNull()) 
  {    
    Q_ASSERT(contextNode.hasAttribute("id") == true);
    Q_ASSERT(contextNode.hasAttribute("name") == true);
    Q_ASSERT(contextNode.hasAttribute("type") == true);
    Q_ASSERT(contextNode.hasAttribute("desc") == true);
    RbeGeneralItem *generalItem = NULL;
    
    QDomElement dataNode = contextNode.firstChildElement();
    while(!dataNode.isNull())
    {
      if(   (dataNode.tagName() == "line" && contextNode.attribute("type") == "TRIPWIRE")
            ||(dataNode.tagName() == "multipoints" && contextNode.attribute("type") == "AREA")
            )
      {
        generalItem =  new RbeGeneralItem();
        generalItem->setID(contextNode.attribute("id").toUInt());
        generalItem->setName(contextNode.attribute("name"));
        generalItem->setType(contextNode.attribute("type"));      
        generalItem->setDesc(contextNode.attribute("desc"));
        
        QDomElement pointsNode = dataNode.firstChildElement("point");
        while(!pointsNode.isNull())
        {
          Q_ASSERT(pointsNode.hasAttribute("x") == true);
          Q_ASSERT(pointsNode.hasAttribute("y") == true);
          
          int x = pointsNode.attribute("x").toInt();
          int y = pointsNode.attribute("y").toInt();
          QPoint p(x,y);
          generalItem->addPoint(p);
          
          pointsNode = pointsNode.nextSiblingElement("point");
        }
      }                  
      
      //color
      if(dataNode.tagName() == "color")
      {
        int r = dataNode.attribute("r").toInt();
        int g = dataNode.attribute("g").toInt();
        int b = dataNode.attribute("b").toInt();
        int a = dataNode.attribute("a").toInt();
        QColor color(r,g,b,a);
        generalItem->setColor(color);
      }
      
      if(generalItem != NULL)
      {
        mLinkToGeneralContainer->getGeneralContextItems()->insert(generalItem->getID(),generalItem); 
      }
      dataNode =  dataNode.nextSiblingElement();
    }
    
    contextNode = contextNode.nextSiblingElement("context");
  }  
}


void RbeXmlHandler::readRules(QDomElement root)
{
  QDomElement ruleNode = root.firstChildElement("rule");
  
  while (!ruleNode.isNull()) 
  {    
    Q_ASSERT(ruleNode.hasAttribute("id") == true);
    Q_ASSERT(ruleNode.hasAttribute("name") == true);
    Q_ASSERT(ruleNode.hasAttribute("desc") == true);
    RbeGeneralItem *generalItem = NULL;
    
    QDomElement dataNode = ruleNode.firstChildElement();
    if(dataNode.tagName() == "event_container" )
    {
      generalItem =  new RbeGeneralItem();
      generalItem->setID(ruleNode.attribute("id").toUInt());
      generalItem->setName(ruleNode.attribute("name"));      
      generalItem->setType("RULE");
      generalItem->setDesc(ruleNode.attribute("desc"));
      
      //get combination type
      QString combinationType = dataNode.attribute("type");
      generalItem->getEventContainer()->combinationType = combinationType;
      
      //if combination type is a sequence, take attribute second
      QString combinationSecond = dataNode.attribute("second");
      if(combinationSecond != "")
      {
        generalItem->getEventContainer()->combinationSecond = combinationSecond.toFloat();
      }
      
      this->readSubContainer(dataNode,generalItem->getEventContainer()->containers,generalItem->getEventContainer());
      this->readEvent(dataNode,generalItem->getEventContainer()->events,generalItem->getEventContainer());
      this->readAction(dataNode,generalItem->getEventContainer()->actions,generalItem->getEventContainer(),"mama");
      
    } 
    
    if(generalItem != NULL)
    {      
      mLinkToGeneralContainer->getGeneralRuleItems()->insert(generalItem->getID(),generalItem);      
    }
    ruleNode = ruleNode.nextSiblingElement("rule");
  }
}



void RbeXmlHandler::readSubContainer(QDomElement &dataNode,QList<EventContainerStruct> &list,EventContainerStruct *item)
{
  QDomElement containerNode = dataNode.firstChildElement("event_container");   
  
  while(!containerNode.isNull())
  {
    EventContainerStruct container;
    container.combinationType = dataNode.attribute("type");
    
    QString combinationSecond = dataNode.attribute("second");
    if(combinationSecond != "")
    {
      container.combinationSecond = combinationSecond.toFloat();
    }
    
    container.linkToMama = item;
    
    this->readSubContainer(containerNode,container.containers,&container);
    this->readEvent(containerNode,container.events,&container);
    this->readAction(containerNode,container.actions,&container,"mama");
    
    list.push_back(container);
    containerNode = containerNode.nextSiblingElement("event_container");
  }
    
}

void RbeXmlHandler::readEvent(QDomElement &dataNode, QList<EventStruct> &list, EventContainerStruct *item)
{
  QDomElement eventNode = dataNode.firstChildElement("event"); //get events inside
  while(!eventNode.isNull())
  {
    EventStruct anEvent;
    anEvent.eventType = eventNode.attribute("type");
    anEvent.linkToMama = item;
    
    this->readEventFilter(eventNode,anEvent.filters,&anEvent);
    this->readAction(eventNode, anEvent.actionsInsideEvent,&anEvent,"papa");
    
    
    list.push_back(anEvent);
    
    eventNode = eventNode.nextSiblingElement("event");
  }
}

void RbeXmlHandler::readEventFilter(QDomElement &dataNode, QList<EventFilterStruct> &list, EventStruct *item)
{
  QDomElement filterNode = dataNode.firstChildElement("event_filter"); 
  while(!filterNode.isNull())
  {
    EventFilterStruct filter;    
    filter.filterType = filterNode.attribute("type");
    filter.value = filterNode.attribute("value");
    filter.linkToPapa = item;
    
    list.push_back(filter);    
    filterNode = filterNode.nextSiblingElement("event_filter");
  }
}

void RbeXmlHandler::readAction(QDomElement &dataNode, QList<ActionStruct> &list, void *item, QString type)
{
  //load actions of the container
  QDomElement actionNode = dataNode.firstChildElement("action");
  while(!actionNode.isNull())
  {         
    QString actionType = actionNode.attribute("type");        
    QString actionValue = actionNode.attribute("value");        
    ActionStruct action;        
    action.actionType = actionType;   
    action.value = actionValue;
    
    if(type == "mama")
      action.linkToMama = static_cast<EventContainerStruct*>(item);
    if(type == "papa")
      action.linkToPapa = static_cast<EventStruct*>(item);
    
    list.push_back(action);        
    
    actionNode = actionNode.nextSiblingElement("action");                
  }
}

void RbeXmlHandler::writeContextDataToFile(QFile *file, bool isSeparatedFile)
{   
  if(mLinkToGeneralContainer->getGeneralContextItems()->size() == 0)
  {
    return;
  }
  
  Q_ASSERT(mLinkToGeneralContainer->getGeneralContextItems()->keys().size() ==  mLinkToGeneralContainer->getGeneralContextItems()->uniqueKeys().size());  
  
  QList<uint>keys = mLinkToGeneralContainer->getGeneralContextItems()->uniqueKeys();  
  
  if(isSeparatedFile == true)
  {
    mWriter.setDevice(file);
    mWriter.setAutoFormatting(true);
    mWriter.writeStartDocument();  
  }
  mWriter.writeStartElement("contexts");
  
  //write mask value  
  mWriter.writeStartElement("mask");      
  QString patha = QApplication::applicationDirPath();  
  QString path = patha + "/data/.temp/mask.png";  
  mWriter.writeAttribute("path",path);  
  mWriter.writeEndElement();//end mask
  
  //write each context data to file
  for(int i = 0 ; i < keys.size(); i++)
  {    
    RbeGeneralItem *item = mLinkToGeneralContainer->getGeneralContextItems()->value(keys[i]);
    if(item->getType() != RbeGeneralItem::RULE) 
    {      
      mWriter.writeStartElement("context");
      mWriter.writeAttribute("id",QString::number(item->getID()));      
      mWriter.writeAttribute("name",item->getName());
      if(item->getType() == RbeGeneralItem::TRIPWIRE)
      {
        mWriter.writeAttribute("type","TRIPWIRE");
      }
      if(item->getType() == RbeGeneralItem::AREA)
      {
        mWriter.writeAttribute("type","AREA");
      }
      mWriter.writeAttribute("desc",item->getDesc());
      
      if(item->getType() == RbeGeneralItem::TRIPWIRE)
      {
        mWriter.writeStartElement("line");
      }
      if(item->getType() == RbeGeneralItem::AREA)
      {
        mWriter.writeStartElement("multipoints");      
      }      
      
      for(int j=0; j <item->getPoints().size();j++)
      {
        QPointF p = item->getPoints()[j];
        mWriter.writeStartElement("point");
        mWriter.writeAttribute("x",QString::number(p.x()));
        mWriter.writeAttribute("y",QString::number(p.y()));
        mWriter.writeEndElement();
      }
      mWriter.writeEndElement();
      
      if(item->getType() == RbeGeneralItem::AREA)
      {
        QColor color = item->getColor();
        QRgb rgb = color.rgba();
        mWriter.writeStartElement("color");
        mWriter.writeAttribute("r",QString::number(qRed(rgb)));
        mWriter.writeAttribute("g",QString::number(qGreen(rgb)));
        mWriter.writeAttribute("b",QString::number(qBlue(rgb)));      
        mWriter.writeAttribute("a",QString::number(qAlpha(rgb)));
        mWriter.writeEndElement();
      }
      
      mWriter.writeEndElement();//end context
    }    
  }   
  
  mWriter.writeEndElement();//end contexts
  
  if(isSeparatedFile == true)
  {
    mWriter.writeEndDocument();  
  }
}

void RbeXmlHandler::writeRuleDataToFile(QFile *file, bool isSeparatedFile)
{  
  if(mLinkToGeneralContainer->getGeneralRuleItems()->size() == 0)
    return;
  
  QList<uint>keys = mLinkToGeneralContainer->getGeneralRuleItems()->uniqueKeys();  
  
  if(isSeparatedFile == true)
  {
    mWriter.setDevice(file);
    mWriter.setAutoFormatting(true);
    mWriter.writeStartDocument();  
  }
  mWriter.writeStartElement("rules");
  
  for(int i = 0 ; i < keys.size(); i++)
  {
    RbeGeneralItem *rule = mLinkToGeneralContainer->getGeneralRuleItems()->value(keys[i]);
    mWriter.writeStartElement("rule");
    mWriter.writeAttribute("id",QString::number(rule->getID()));      
    mWriter.writeAttribute("name",rule->getName());
    mWriter.writeAttribute("desc",rule->getDesc());
    
    EventContainerStruct *container = rule->getEventContainer();                
    mWriter.writeStartElement("event_container");
    mWriter.writeAttribute("type",container->combinationType);
    if(container->combinationType == "SEQUENCE")
      mWriter.writeAttribute("second",QString::number(container->combinationSecond));
    
    this->writeContainer(container->containers);    
    this->writeEvent(container->events);
    this->writeAction(container->actions);        
    
    mWriter.writeEndElement();//end event_container    
    mWriter.writeEndElement();//end rule
  }
  
  mWriter.writeEndElement();//end rules
  
  if(isSeparatedFile == true)
  {
    mWriter.writeEndDocument();  
  }
}

void RbeXmlHandler::writeContainer(QList<EventContainerStruct> &list)
{
  for(int i = 0 ; i < list.size(); i++)
  {
    EventContainerStruct *container = &list[i];
    
    mWriter.writeStartElement("event_container");
    mWriter.writeAttribute("type",container->combinationType);
    if(container->combinationType == "SEQUENCE")
      mWriter.writeAttribute("second",QString::number(container->combinationSecond));
    
    this->writeContainer(container->containers);    
    this->writeEvent(container->events);
    this->writeAction(container->actions);
    
    mWriter.writeEndElement();
  } 
}

void RbeXmlHandler::writeEvent(QList<EventStruct> &list)
{
  for(int i = 0 ; i < list.size(); i++)
  {
    EventStruct *event = &list[i];
    
    mWriter.writeStartElement("event");
    mWriter.writeAttribute("type",event->eventType);    
    
    this->writeEventFilter(event->filters);
    this->writeAction(event->actionsInsideEvent);
    
    mWriter.writeEndElement();
  }  
}

void RbeXmlHandler::writeEventFilter(QList<EventFilterStruct> &list)
{
  for(int i = 0 ; i < list.size(); i++)
  {
    EventFilterStruct *filter = &list[i];
    
    mWriter.writeStartElement("event_filter");
    
    mWriter.writeAttribute("type",filter->filterType);    
    mWriter.writeAttribute("value",filter->value);
    
    mWriter.writeEndElement();
  }
}

void RbeXmlHandler::writeAction(QList<ActionStruct> &list)
{
  for(int i = 0 ; i < list.size(); i++)
  {
    ActionStruct *action = &list[i];
    
    mWriter.writeStartElement("action");
    
    mWriter.writeAttribute("type",action->actionType);    
    mWriter.writeAttribute("value",action->value);
    
    mWriter.writeEndElement();
  }  
}

void RbeXmlHandler::writeProjectFile(QFile *file)
{
  if(mLinkToGeneralContainer->getGeneralRuleItems()->size() == 0 &&
     mLinkToGeneralContainer->getGeneralContextItems()->size() == 0)
    return;
  
  mWriter.setDevice(file);
  mWriter.setAutoFormatting(true);
  mWriter.writeStartDocument();  
  mWriter.writeStartElement("rbe_project");
  
  this->writeContextDataToFile(file,false);
  this->writeRuleDataToFile(file,false);
  
  mWriter.writeEndElement();
  mWriter.writeEndDocument();  
  
}

RbeXmlHandler::~RbeXmlHandler()
{
  mDomDocument.clear();        
  //mWriter
  mLinkToGeneralContainer = NULL;
}

