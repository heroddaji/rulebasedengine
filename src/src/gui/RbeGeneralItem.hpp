#ifndef RBEGENERALITEM_HPP
#define RBEGENERALITEM_HPP

#include <QObject>
#include <QPoint>
#include <QHash>
#include <QColor>
#include <QStringList>

struct EventContainerStruct;
struct EventStruct;
struct EventFilterStruct;
struct ActionStruct;

struct EventContainerStruct{
  EventContainerStruct *linkToMama; 
  QString combinationType;
  double combinationSecond;
  
  QList<EventStruct> events;
  QList<ActionStruct> actions;
  QList<EventContainerStruct> containers;  
};

struct EventStruct{  
  EventContainerStruct *linkToMama; 
  QString eventType;
  QList<ActionStruct> actionsInsideEvent;
  QList<EventFilterStruct> filters;
};

struct EventFilterStruct{
  EventStruct *linkToPapa; 
  QString filterType;
  QString value;
};

struct ActionStruct{  
  EventContainerStruct *linkToMama; 
  EventStruct *linkToPapa; 
  QString actionType;  
  QString value;
};

class RbeGeneralItem : public QObject
{
    Q_OBJECT
public:
  explicit RbeGeneralItem(QObject *parent = 0);
  
  enum GeneralType{TRIPWIRE=100, AREA, RULE, CONTEXT, EVENT,EVENT_FILTER, EVENTCONTAINTER, ACTION, NOTYPE};    
  
  void setID(uint id){mId = id;}
  uint getID() const {return mId;}  
  
  void setName(QString name){mName = name;}
  QString getName() const {return mName;}  
  
  void setType(GeneralType type){mType = type;}
  void setType(QString type);    
  
  GeneralType getType()const {return mType;
                             }  
  void setDesc(QString desc){mDesc = desc;}
  QString getDesc() const {return mDesc;}  
  
  void addPoint(QPoint p){mPoints.push_back(p);}
  QList<QPoint> getPoints() {return mPoints;}
  void setPoints(int index, QPoint p);    
  
  void generateEventContainer(EventContainerStruct*,QString type);
  EventContainerStruct *getEventContainer(){return &eventContainer;}    
  
  void setColor(QColor color){mColor = color;}
  QColor getColor(){return mColor;}
  
  EventContainerStruct eventContainer;
  
private:
  uint mId;
  QString mName;  
  QString mDesc;  
  GeneralType mType;
  QList<QPoint>mPoints;
  QColor mColor;
};




#endif // RBEGENERALITEM_HPP
