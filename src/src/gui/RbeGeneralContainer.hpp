#ifndef RBEGENERALCONTAINER_HPP
#define RBEGENERALCONTAINER_HPP

#include <QObject>
#include <QHash>
#include <QTreeWidgetItem>
#include <QGraphicsItem>
#include <qtpropertybrowser.h>
#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>


class RbeGeneralItem;
class RbePropertyTree;
struct GeneralDataFromPropertyTree ;
class RbeItemTree;
class RbeVisualizeWidget;
class RbeVisualizeWidget_GraphicsScene;
class RbeVisualizeWidget_RuleTree;
class MainWindow;
    
class RbeGeneralContainer : public QObject
{
    Q_OBJECT
public:
    explicit RbeGeneralContainer(QObject *parent = 0);
  ~RbeGeneralContainer();
  
  enum UpdateType{LOAD_CONTEXT=80, LOAD_RULE, ADD_NEW_RULE};
  
  QHash<uint,RbeGeneralItem *>* getGeneralContextItems();
  QHash<uint,RbeGeneralItem *>* getGeneralRuleItems();
  
  void setLinkToPropertyTree(RbePropertyTree *link);
  void setLinkToItemTree(RbeItemTree *link);
  void setLinkToVisualizeWidget(RbeVisualizeWidget *link);  
  void setLinkToVisualizeWidget_Scene(RbeVisualizeWidget_GraphicsScene *link);  
  void setLinkToVisualizeWidget_RuleTree(RbeVisualizeWidget_RuleTree *link);  
  void setLinkToMainWindow(MainWindow*link);  
  
  void update(UpdateType type);  
  bool setUpSignalSlot();
  
  void handleEventsFromDrawScene(QGraphicsItem *item, uint itemID, int eType);

  void clearContextItems();
  void clearRuleItems();
  void clearAllItems();
  void clearRuleTree();
private:
  QHash<uint,RbeGeneralItem *> mGeneralContextItems;
  QHash<uint,RbeGeneralItem *> mGeneralRuleItems;
  RbePropertyTree *mLinkToPropertyTree;
  RbeVisualizeWidget *mLinkToVisualizeWidget;
  RbeVisualizeWidget_GraphicsScene *mLinkToVisualizeWidget_Scene;
  RbeVisualizeWidget_RuleTree *mLinkToVisualizeWidget_RuleTree;
  RbeItemTree *mLinkToItemTree;
  MainWindow *mLinkToMainWindow;
  
  uint generateID(QHash<uint,RbeGeneralItem *> *container);
  bool generateNewRule();
  
public slots:
  void handleEventsFromItemTree(QTreeWidgetItem *,int);
  void handleEventsFromItemTree(QTreeWidgetItem *,int,QString);
  void handleEventsFromRuleTree(QTreeWidgetItem*,int);    
  void handleEventsFromPropertyTree(GeneralDataFromPropertyTree *);  
  
};
#endif // RBEGENERALCONTAINER_HPP
