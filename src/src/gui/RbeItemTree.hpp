#ifndef RBEITEMTREE_HPP
#define RBEITEMTREE_HPP

#include <QTreeWidget>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QMenu>
#include <QMouseEvent>
#include <QDebug>

class RbeItemTree_WidgetItem;
class RbeGeneralContainer;
class RbeGeneralItem;

class RbeItemTree : public QTreeWidget
{
  Q_OBJECT
  
public:  
  RbeItemTree(QWidget *parent = 0);        
  ~RbeItemTree();
  void setupGuiFromContainer(RbeGeneralContainer *container);
  void generateNewTreeItem(RbeGeneralItem *, int type);  
      
  void clearContextItems();
  void clearRuleItems();
  void clearAllItems();  
  
  RbeItemTree_WidgetItem *mCurrentFocusItem;
  
  
protected:
  void mousePressEvent ( QMouseEvent * event );  

private:  
  RbeItemTree_WidgetItem *mTopContextItem;
  RbeItemTree_WidgetItem *mTopRuleItem;    
  
  QPoint mItemPos;
  
  void setupTopItems();
private slots:
  void deleteItem();
  
signals:
  void deleteItemSignal(QTreeWidgetItem *item, int column, QString action);
};

#endif // 
