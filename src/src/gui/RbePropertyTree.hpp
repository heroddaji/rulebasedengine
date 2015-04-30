#ifndef RBEPROPERTYTREE_HPP
#define RBEPROPERTYTREE_HPP

#include "QMenu"
#include "QContextMenuEvent"
#include "QAction"
#include "QStringList"

#include <qtpropertybrowser.h>
#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>
#include <qteditorfactory.h>

class RbeGeneralContainer;
class RbeGeneralItem;
struct EventContainerStruct;
struct EventStruct;
struct EventFilterStruct;
struct ActionStruct;

struct GeneralDataFromPropertyTree{
  int itemType;
  void *pointerToItem;
};

class RbePropertyTree : public QtTreePropertyBrowser
{
  Q_OBJECT
  friend class QtEnumPropertyManager;
public:
  explicit RbePropertyTree (QWidget *parent = 0);  
  ~RbePropertyTree();  
    
  void showPropertyRule(RbeGeneralItem *);
  void showPropertyLine(RbeGeneralItem *);
  void showPropertyPolygon(RbeGeneralItem *);
  void showPropertyEventContainer(EventContainerStruct *container);  
  void showPropertyEvent(EventStruct *event);  
  void showPropertyEventFilter(EventFilterStruct *filter);
  void showPropertyAction(ActionStruct *action);    
  
  void cleanTheEditor();    
    
private:      
  //new
  GeneralDataFromPropertyTree generalData;
  QMap<QtProperty *, QString> propertyToId;
  QMap<QString, QtProperty *> idToProperty;    
  
  void addPropertyToMap(QtProperty *property, const QString &id);  
  
private slots:  
  void valueChanged(QtProperty *, int);
  void valueChanged(QtProperty* property,QVariant value);
  
signals:
  void somethingChangedInPropertyTree(GeneralDataFromPropertyTree *);
};

#endif // 
