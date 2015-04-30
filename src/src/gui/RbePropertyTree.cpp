#include "RbePropertyTree.hpp"

#include "RbeItemTree_WidgetItem.hpp"
#include "RbeGeneralContainer.hpp"
#include "RbeGeneralItem.hpp"
#include "RbeVisualizeWidget_GraphicsScene.hpp"


RbePropertyTree::RbePropertyTree(QWidget *parent) :
  QtTreePropertyBrowser(parent)
{
  generalData.pointerToItem = NULL;
  generalData.itemType = RbeGeneralItem::NOTYPE;
}

void RbePropertyTree::addPropertyToMap(QtProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;    
}

void RbePropertyTree::showPropertyLine(RbeGeneralItem *generalItem)
{
  this->cleanTheEditor();
  
  QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
  QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);  
  this->setFactoryForManager(variantManager, variantFactory);  
  
  //this->setPropertiesWithoutValueMarked(true);??
  //this->setRootIsDecorated(false);    ??
  
  //values
  QtVariantProperty *variantProperty;  
  
  variantProperty = variantManager->addProperty(QVariant::Int,QString("ID"));
  variantProperty->setValue(generalItem->getID());  
  variantProperty->setEnabled(false);
  this->addProperty(variantProperty);
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Name"));
  variantProperty->setValue(generalItem->getName()); 
  this->addProperty(variantProperty);  
  this->addPropertyToMap(variantProperty,"Name");
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Type"));  
  variantProperty->setValue("TRIPWIRE"); 
  variantProperty->setEnabled(false);
  this->addProperty(variantProperty);
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Description"));
  variantProperty->setValue(generalItem->getDesc()); 
  this->addProperty(variantProperty);  
  this->addPropertyToMap(variantProperty,"Description");
  
  generalData.itemType = RbeGeneralItem::TRIPWIRE;
  generalData.pointerToItem = generalItem;
  
  this->connect(variantManager,SIGNAL(valueChanged(QtProperty*,QVariant)),
                this,SLOT(valueChanged(QtProperty*,QVariant)));
}

void RbePropertyTree::showPropertyPolygon(RbeGeneralItem *generalItem)
{
  this->cleanTheEditor();
  
  QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
  QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);  
  this->setFactoryForManager(variantManager, variantFactory);  
//  this->setPropertiesWithoutValueMarked(true);??
//  this->setRootIsDecorated(false);    ??
  
  //values
  QtVariantProperty *variantProperty;  
  
  variantProperty = variantManager->addProperty(QVariant::Int,QString("ID"));
  variantProperty->setValue(generalItem->getID());  
  variantProperty->setEnabled(false);
  this->addProperty(variantProperty);
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Name"));
  variantProperty->setValue(generalItem->getName()); 
  this->addProperty(variantProperty);  
  this->addPropertyToMap(variantProperty,"Name");
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Type"));  
  variantProperty->setValue("AREA"); 
  variantProperty->setEnabled(false);
  this->addProperty(variantProperty);
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Description"));
  variantProperty->setValue(generalItem->getDesc()); 
  this->addProperty(variantProperty);  
  this->addPropertyToMap(variantProperty,"Description");
  
  //color
  variantProperty = variantManager->addProperty(QVariant::Color, QString("Color"));
  variantProperty->setValue(QColor(generalItem->getColor()));
  this->addPropertyToMap(variantProperty,"Color");
  addProperty(variantProperty);
  
  generalData.itemType = RbeGeneralItem::AREA;
  generalData.pointerToItem = generalItem;
  
  
  this->connect(variantManager,SIGNAL(valueChanged(QtProperty*,QVariant)),
                this,SLOT(valueChanged(QtProperty*,QVariant)));
}

void RbePropertyTree::showPropertyRule(RbeGeneralItem *generalItem)
{
  this->cleanTheEditor();
  
  QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
  QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);  
  this->setFactoryForManager(variantManager, variantFactory);  
  
  //values
  QtVariantProperty *variantProperty;  
  
  variantProperty = variantManager->addProperty(QVariant::Int,QString("ID"));
  variantProperty->setValue(generalItem->getID());  
  variantProperty->setEnabled(false);
  this->addProperty(variantProperty);
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Name"));
  variantProperty->setValue(generalItem->getName()); 
  this->addProperty(variantProperty);  
  this->addPropertyToMap(variantProperty,"Name");
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Type"));  
  variantProperty->setValue("RULE"); 
  variantProperty->setEnabled(false);
  this->addProperty(variantProperty);
  
  variantProperty = variantManager->addProperty(QVariant::String,QString("Description"));
  variantProperty->setValue(generalItem->getDesc()); 
  this->addProperty(variantProperty);  
  this->addPropertyToMap(variantProperty,"Description");
  
  generalData.itemType = RbeGeneralItem::RULE;
  generalData.pointerToItem = generalItem;
  
  this->connect(variantManager,SIGNAL(valueChanged(QtProperty*,QVariant)),
                this,SLOT(valueChanged(QtProperty*,QVariant)));
}

void RbePropertyTree::showPropertyEventContainer(EventContainerStruct *container)
{
  this->cleanTheEditor();     
  
  QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
  QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(this);
  this->setFactoryForManager(enumManager,enumFactory);
  
  QtProperty *property = enumManager->addProperty("Combination");;  
  QStringList enumNames;
  enumNames << "AND" << "OR" << "SEQUENCE" << "ONE_EVENT" ;  
  enumManager->setEnumNames(property, enumNames);  
  this->addPropertyToMap(property,"Combination");
  this->addProperty(property);  
    
  //store pointer
  generalData.itemType = RbeGeneralItem::EVENTCONTAINTER;
  generalData.pointerToItem = container;  
  
  //set correct enum value
  int value = 0;
  if(container->combinationType == "AND") value = 0;
  if(container->combinationType == "OR") value = 1;
  if(container->combinationType == "SEQUENCE") value = 2;
  if(container->combinationType == "ONE_EVENT") value = 3;          
  enumManager->setValue(property,value);
  
  if(value == 2)
  {
    //setup the second value for SEQUENCE type
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);
    this->setFactoryForManager(variantManager,variantFactory);  
    
    QtVariantProperty *variantProperty = variantManager->addProperty(QVariant::Double,"Second");
    
    if(container->combinationSecond == 0)
      container->combinationSecond = 1;
    
    variantProperty->setValue(container->combinationSecond);    
    
    this->addPropertyToMap(variantProperty,"Second");
    this->addProperty(variantProperty);
    
    this->connect(variantManager,SIGNAL(valueChanged(QtProperty*,QVariant)),
                  this,SLOT(valueChanged(QtProperty*,QVariant)));
  }
  
  //note: must put this connect at the end, otherwise cause loop
  //beetween ruletree and this 
  this->connect(enumManager,SIGNAL(valueChanged(QtProperty*,int)),
                this,SLOT(valueChanged(QtProperty*,int)));  
  
}

void RbePropertyTree::showPropertyEvent(EventStruct *event)
{
  this->cleanTheEditor();    
  
  QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
  QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(this);
  this->setFactoryForManager(enumManager,enumFactory);
    
  QtProperty *property = enumManager->addProperty("Event");
  QStringList enumNames;
  enumNames << "ENTER_AREA" << "LEAVE_AREA" << "CROSSING_TRIPWIRE" << "CROSSING_TRIPWIRE_LEFT2RIGHT" << "CROSSING_TRIPWIRE_RIGHT2LEFT";
  enumManager->setEnumNames(property, enumNames);  
  this->addPropertyToMap(property,"Event");
  this->addProperty(property);
  
  generalData.itemType = RbeGeneralItem::EVENT;
  generalData.pointerToItem = event;
  
  //set correct enum value
  int value = 0;
  if(event->eventType == "ENTER_AREA") value = 0;
  if(event->eventType == "LEAVE_AREA") value = 1;    
  if(event->eventType == "CROSSING_TRIPWIRE") value = 2;    
  if(event->eventType == "CROSSING_TRIPWIRE_LEFT2RIGHT") value = 3;    
  if(event->eventType == "CROSSING_TRIPWIRE_RIGHT2LEFT") value = 4;    
  enumManager->setValue(property,value);
  
  this->connect(enumManager,SIGNAL(valueChanged(QtProperty*,int)),
                this,SLOT(valueChanged(QtProperty*,int)));
}

void RbePropertyTree::showPropertyEventFilter(EventFilterStruct *filter)
{
  this->cleanTheEditor();    
  
  QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
  QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(this);
  this->setFactoryForManager(enumManager,enumFactory);
    
  //show filter type
  QtProperty *property = enumManager->addProperty("Filter");
  QStringList enumNames;
  enumNames << "CONTEXT_ID" << "OBJECT_TYPE";
  enumManager->setEnumNames(property, enumNames);  
  this->addPropertyToMap(property,"Filter");
  this->addProperty(property);
  
  //set correct enum value
  int value = 0;
  if(filter->filterType == "CONTEXT_ID") value = 0;
  if(filter->filterType == "OBJECT_TYPE") value = 1;
  enumManager->setValue(property,value);  
  
  
     
    //setup the second value for CONTEXT_ID type
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);
    this->setFactoryForManager(variantManager,variantFactory);    
    QtVariantProperty *variantProperty = variantManager->addProperty(QVariant::Int,"Filter value");    
    
    variantProperty->setValue(filter->value);    
    
    this->addPropertyToMap(variantProperty,"Filter value");
    this->addProperty(variantProperty);
    
    this->connect(variantManager,SIGNAL(valueChanged(QtProperty*,QVariant)),
                  this,SLOT(valueChanged(QtProperty*,QVariant))); 
  
  
  
  generalData.itemType = RbeGeneralItem::EVENT_FILTER;
  generalData.pointerToItem = filter;   
  
  this->connect(enumManager,SIGNAL(valueChanged(QtProperty*,int)),
                this,SLOT(valueChanged(QtProperty*,int)));
}

void RbePropertyTree::showPropertyAction(ActionStruct *action)
{  
  this->cleanTheEditor();    
  
  //setup the enum value
  QtEnumPropertyManager *enumManager = new QtEnumPropertyManager(this);
  QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(this);
  this->setFactoryForManager(enumManager,enumFactory);  
  
  QtProperty *property = enumManager->addProperty("Action");
  QStringList enumNames;
  enumNames << "PRINT" << "ALARM";
  enumManager->setEnumNames(property, enumNames);  
  this->addPropertyToMap(property,"Action");
  this->addProperty(property);
  
  //setup the string value
  QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
  QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);
  this->setFactoryForManager(variantManager,variantFactory);  
  
  QtVariantProperty *variantProperty = variantManager->addProperty(QVariant::String,"Value");
  variantProperty->setValue(action->value);
  this->addPropertyToMap(variantProperty,"Value");
  this->addProperty(variantProperty);
  
  //save that info the this struct, so can pass it to other class
  generalData.itemType = RbeGeneralItem::ACTION;
  generalData.pointerToItem = action;
  
  //set correct enum value
  int value = 0;
  if(action->actionType == "PRINT") value = 0;
  if(action->actionType == "ALARM") value = 1;  
  enumManager->setValue(property,value);
    
  this->connect(enumManager,SIGNAL(valueChanged(QtProperty*,int)),
                this,SLOT(valueChanged(QtProperty*,int)));
  
  this->connect(variantManager,SIGNAL(valueChanged(QtProperty*,QVariant)),
                this,SLOT(valueChanged(QtProperty*,QVariant)));
}

void RbePropertyTree::valueChanged(QtProperty *property, int value)
{
  if (!propertyToId.contains(property))
      return;
  
  QString id = propertyToId[property];
  if(id == "Combination")
  {
    generalData.itemType = RbeGeneralItem::EVENTCONTAINTER;
    EventContainerStruct *container = static_cast<EventContainerStruct *>(generalData.pointerToItem);
    if(value == 0)
      container->combinationType = "AND";
    if(value == 1)
      container->combinationType = "OR";
    if(value == 2)
    {
      container->combinationType = "SEQUENCE";
      container->combinationSecond = 1;
    }
    if(value == 3)
      container->combinationType = "ONE_EVENT";
    
    emit somethingChangedInPropertyTree(&generalData);
  } 
  
  if(id == "Event")
  {
    generalData.itemType = RbeGeneralItem::EVENT;
    EventStruct *event = static_cast<EventStruct *>(generalData.pointerToItem);
    if(value == 0)
      event->eventType = "ENTER_AREA";
    if(value == 1)
      event->eventType = "LEAVE_AREA";        
    if(value == 2)
      event->eventType = "CROSSING_TRIPWIRE";        
    if(value == 3)
      event->eventType = "CROSSING_TRIPWIRE_LEFT2RIGHT";        
    if(value == 4)
      event->eventType = "CROSSING_TRIPWIRE_RIGHT2LEFT";        
    emit somethingChangedInPropertyTree(&generalData);
  }
  
  if(id == "Filter")
  {
    generalData.itemType = RbeGeneralItem::EVENT_FILTER;
    EventFilterStruct *filter = static_cast<EventFilterStruct *>(generalData.pointerToItem);
    if(value == 0)
      filter->filterType = "CONTEXT_ID";
    if(value == 1)
      filter->filterType = "OBJECT_TYPE";       
           
    emit somethingChangedInPropertyTree(&generalData);
  }
  
   if(id == "Action")
  {
    generalData.itemType = RbeGeneralItem::ACTION;
    ActionStruct *action = static_cast<ActionStruct *>(generalData.pointerToItem);
    if(value == 0)
      action->actionType = "PRINT";
    if(value == 1)
      action->actionType = "ALARM";   
    
    emit somethingChangedInPropertyTree(&generalData);
  }     
}

void RbePropertyTree::valueChanged(QtProperty* property,QVariant variant)
{
  if (!propertyToId.contains(property))
      return;
  
  QString id = propertyToId[property];
  
  if(id == "Filter value")
  {
    generalData.itemType = RbeGeneralItem::EVENT_FILTER;
    EventFilterStruct *filter = static_cast<EventFilterStruct *>(generalData.pointerToItem);
    
    filter->value = variant.toInt();           
    emit somethingChangedInPropertyTree(&generalData);
  }  
  
  if(id == "Value")
  {
    generalData.itemType = RbeGeneralItem::ACTION;//not nessary
    ActionStruct *action = static_cast<ActionStruct *>(generalData.pointerToItem);
    action->value = variant.toString();    
  }
  
  if(id == "Color")
  {
    generalData.itemType = RbeGeneralItem::AREA;
    RbeGeneralItem *generalItem = static_cast<RbeGeneralItem *>(generalData.pointerToItem);
    
    QColor color = variant.value<QColor>();
    generalItem->setColor(color);
    emit somethingChangedInPropertyTree(&generalData);
  }
  
  if(id == "Second")
  {
    generalData.itemType = RbeGeneralItem::EVENTCONTAINTER;
    EventContainerStruct *container = static_cast<EventContainerStruct *>(generalData.pointerToItem);
    container->combinationSecond = variant.toDouble();
  }
  
  if(id=="Name")
  {    
    RbeGeneralItem *generalItem = static_cast<RbeGeneralItem *>(generalData.pointerToItem);
    generalItem->setName(variant.toString());    
    
    emit somethingChangedInPropertyTree(&generalData);
  }
  if(id=="Description")
  {
    RbeGeneralItem *generalItem = static_cast<RbeGeneralItem *>(generalData.pointerToItem);
    generalItem->setDesc(variant.toString());    
    
    emit somethingChangedInPropertyTree(&generalData);
  }
  
}

void RbePropertyTree::cleanTheEditor()
{
  this->clear();
  QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
  while (itProp != propertyToId.constEnd()) 
  {
      delete itProp.key();
      itProp++;
  }  
  propertyToId.clear();
  idToProperty.clear();
}

RbePropertyTree::~RbePropertyTree()
{  
}


