#ifndef RBEXMLHANDLER_HPP
#define RBEXMLHANDLER_HPP

#include <QFile>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QApplication>
#include <QDebug>
class RbeGeneralContainer;
class RbeGeneralItem;
struct EventContainerStruct;
struct EventStruct;
struct EventFilterStruct;
struct ActionStruct;

class RbeXmlHandler
{
public:
   
    RbeXmlHandler();     
    ~RbeXmlHandler();     
    
    void setGeneralContainer(RbeGeneralContainer *container){this->mLinkToGeneralContainer = container;}
    
    void read(QFile *file);     
    
    void writeProjectFile(QFile *file);
    void writeContextDataToFile(QFile *file, bool isSeparatedFile = true);        
    void writeRuleDataToFile(QFile *file, bool isSeparatedFile = true);        
    
private:    
    
    void readProject(QDomElement root);        
    void readContexts(QDomElement root);    
    void readRules(QDomElement root);            
    
    void readContainer(QDomElement &,QList<EventContainerStruct> &, EventContainerStruct *);
    void readSubContainer(QDomElement &,QList<EventContainerStruct> &, EventContainerStruct *);
    void readEvent(QDomElement &,QList<EventStruct> &, EventContainerStruct *);
    void readEventFilter(QDomElement &,QList<EventFilterStruct> &, EventStruct *);
    void readAction(QDomElement &, QList<ActionStruct> &, void *, QString type);
    
    void writeContainer(QList<EventContainerStruct> &list);
    void writeEvent(QList<EventStruct> &list);    
    void writeEventFilter(QList<EventFilterStruct> &list);    
    void writeAction(QList<ActionStruct> &list);
    
    QDomDocument mDomDocument;        
    QXmlStreamWriter mWriter;        
    RbeGeneralContainer *mLinkToGeneralContainer;
};

#endif // 
