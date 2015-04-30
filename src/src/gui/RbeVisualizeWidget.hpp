#ifndef RBEVISUALIZEWIDGET_HPP
#define RBEVISUALIZEWIDGET_HPP

#include <QtGui>

class RbeGeneralContainer;
class RbeVisualizeWidget_GraphicsScene;
class RbeVisualizeWidget_GraphicsView;
class RbeVisualizeWidget_RuleTree;
class RbeVisualizeWidget : public QWidget
{
  Q_OBJECT
  
public:
  
  RbeVisualizeWidget(QWidget *parent = 0);      
  
  void setLinkToContainer(RbeGeneralContainer *);
  RbeGeneralContainer * getLinkToContainer();
  
  RbeVisualizeWidget_GraphicsScene *getDrawScene();
  RbeVisualizeWidget_GraphicsView *getDrawView();
  RbeVisualizeWidget_RuleTree *getRuleTree();
    
  void setupGuiFromContextItems(RbeGeneralContainer *);
  void makeDrawViewVisile();
  void makeRuleTreeVisile();
  
private:  
  RbeVisualizeWidget_GraphicsScene *drawScene;
  RbeVisualizeWidget_GraphicsView *drawView;  
  RbeVisualizeWidget_RuleTree *ruleTree;   
  RbeGeneralContainer *mLinkToContainer;  
  
};

#endif // 
