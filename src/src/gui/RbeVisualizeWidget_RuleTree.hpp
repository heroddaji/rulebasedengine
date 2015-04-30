#ifndef RBEVISUALIZEWIDGET_RULETREE_HPP
#define RBEVISUALIZEWIDGET_RULETREE_HPP

#include <QTreeWidget>
#include <QHeaderView>
#include <QString>
#include <QAction>
#include <QObject>
#include <QMouseEvent>
#include <QPoint>
#include <QMenu>
#include <QMessageBox>

struct EventContainerStruct; ///< Fo
struct ActionStruct;
struct EventStruct;
struct EventFilterStruct;
class RbeItemTree_WidgetItem;

class RbeVisualizeWidget_RuleTree: public QTreeWidget
{
  Q_OBJECT
public:    
  /**
    * Constructor.
    */
    explicit RbeVisualizeWidget_RuleTree(QWidget *parent = 0);
  
  /**
    * Destructor.
    */
    ~RbeVisualizeWidget_RuleTree();
  
  /**
    * Show the instruction text when user click on Rule widgetItem.
    */
    void showRuleInstruction();
    
    /**
      * get variable mEventContainer.
      * \return mEventContainer.
      */
    EventContainerStruct* getEventContainer();
    
    /**
      * set varible mEventContainer.
      * \param container the container to be set.
      */
    void setEventContainer(EventContainerStruct *container);        
    
    /**
      * show all content inside a rule in this widget.
      */
    void showEventsInsideContainer();
    
private:    
    RbeItemTree_WidgetItem *mSelectedItem; ///< the RbeItemTree_WidgetItem when user press mouse on.
    RbeItemTree_WidgetItem *mTopItem; ///< the RbeItemTree_WidgetItem on top most in this view.
    EventContainerStruct *mEventContainer;  ///< top EventContainer this rule view.
    QPoint mItemPos; ///< position of RbeItemTree_WidgetItem when user press mouse on.
    
    void mousePressEvent(QMouseEvent *event);        
    void showAllEventsContainer(RbeItemTree_WidgetItem *item, QList<EventContainerStruct> &list);
    void showAllEvents(RbeItemTree_WidgetItem *item, QList<EventStruct> &list);
    void showAllEventFitlers(RbeItemTree_WidgetItem *item, QList<EventFilterStruct> &list);
    void showAllActions(RbeItemTree_WidgetItem *item, QList<ActionStruct> &list);        
    
public slots:
    void getClickedItem(QTreeWidgetItem*);
    void addNewEvent();
    void addNewAction();
    void addNewContainers();
    void addFilterAction();
    void deleteItem();    
};

#endif // RBEVISUALIZEWIDGET_RULETREE_HPP
