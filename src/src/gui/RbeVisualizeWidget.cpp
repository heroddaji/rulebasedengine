#include "RbeVisualizeWidget.hpp"

#include "RbeVisualizeWidget_RuleTree.hpp"
#include "RbeVisualizeWidget_GraphicsScene.hpp"
#include "RbeVisualizeWidget_GraphicsView.hpp"
#include "RbeGeneralItem.hpp"
#include "RbeGeneralContainer.hpp"

RbeVisualizeWidget::RbeVisualizeWidget(QWidget *parent):QWidget(parent) 
{  
 
  drawView = new RbeVisualizeWidget_GraphicsView(parent);  
  drawView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
  drawScene = new RbeVisualizeWidget_GraphicsScene();    
  drawView->setScene(drawScene);    
      
  ruleTree = new RbeVisualizeWidget_RuleTree(this);    
  ruleTree->setHidden(true);//set hidden at first, to avoid window show both drawView and rule tree
  ruleTree->setMinimumWidth(drawScene->width());
}

void RbeVisualizeWidget::setLinkToContainer(RbeGeneralContainer *container)
{
  this->mLinkToContainer = container;
  
  //send also link to drawScene
  drawScene->setLinkToContainer(container);
}

RbeGeneralContainer * RbeVisualizeWidget::getLinkToContainer()
{
  return mLinkToContainer;
}


RbeVisualizeWidget_GraphicsScene* RbeVisualizeWidget::getDrawScene()
{
  return drawScene;
}

RbeVisualizeWidget_GraphicsView* RbeVisualizeWidget::getDrawView()
{
  return drawView;
}

RbeVisualizeWidget_RuleTree* RbeVisualizeWidget::getRuleTree()
{
    return ruleTree;
}

void RbeVisualizeWidget::setupGuiFromContextItems(RbeGeneralContainer *container)
{
  this->getDrawScene()->setupGuiFromContextItems(container);
}

void RbeVisualizeWidget::makeDrawViewVisile()
{
  this->drawView->setVisible(true);
  this->ruleTree->setHidden(true);
}

void RbeVisualizeWidget::makeRuleTreeVisile()
{
  this->ruleTree->setVisible(true);
  this->drawView->setHidden(true);  
}
