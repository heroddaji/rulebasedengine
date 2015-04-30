#include "RbeVisualizeWidget_GraphicsScene.hpp"

#include "RbeVisualizeWidget_LineItem.hpp"
#include "RbeVisualizeWidget_PolygonItem.hpp"
#include "RbeGeneralContainer.hpp"
#include "RbeGeneralItem.hpp"

RbeVisualizeWidget_GraphicsScene::RbeVisualizeWidget_GraphicsScene(QObject *parent) :
  QGraphicsScene(parent)
{
  qreal sceneRectWidth = 384;
  qreal sceneRectHeight = 288;
  this->setSceneRect(0,0,sceneRectWidth,sceneRectHeight);
  
  itemMode = NOTHING_MODE;
  line = 0;
  
  connect(this,SIGNAL(sceneRectChanged(QRectF)),this,SLOT(keepSceneRect()));
}

void RbeVisualizeWidget_GraphicsScene::contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent )
{
  QGraphicsScene::contextMenuEvent(contextMenuEvent);
  
    if(contextMenuEvent->reason() == QGraphicsSceneContextMenuEvent::Mouse)
    {
      //check context item type, then provide the option
      if(itemAt(contextMenuEvent->scenePos()))
      {
        QGraphicsItem *gItem = itemAt(contextMenuEvent->scenePos());
        mScenePos = contextMenuEvent->scenePos();
        
        mDeleteItemId = mContextItems.key(itemAt(contextMenuEvent->scenePos()));
        QMenu menu;        
        
        QAction addPointAction("Add point",this);
        connect(&addPointAction,SIGNAL(triggered()),this,SLOT(addPoint()));
        
        QAction removePointAction("Remove Point",this);
        connect(&removePointAction,SIGNAL(triggered()),this,SLOT(removePoint()));
        
        QAction increaseSizeAction("Increase size",this);        
        connect(&increaseSizeAction,SIGNAL(triggered()),this,SLOT(increaseSize()));
        
        QAction decreaseSizeAction("Decrease size",this);
        connect(&decreaseSizeAction,SIGNAL(triggered()),this,SLOT(decreaseSize()));
        
        if(gItem->type() == POLYGON_ITEM)
        {          
          menu.addAction(&addPointAction);          
          menu.addAction(&removePointAction);
          menu.addAction(&increaseSizeAction);
          menu.addAction(&decreaseSizeAction);
        }
        
        QAction deleteAction("Delete",this);
        connect(&deleteAction,SIGNAL(triggered()),this,SLOT(deleteItem()));
        menu.addAction(&deleteAction);
        
        menu.exec(contextMenuEvent->screenPos()); 
        
        //gItem->clearFocus();
      }
    }
}

void RbeVisualizeWidget_GraphicsScene::deleteItem()
{  
  QGraphicsItem *graphicsItem = mContextItems.value(mDeleteItemId);
  this->removeItem(graphicsItem);
  this->sendDeleteItemActionFromGraphicsItem(graphicsItem,mDeleteItemId);  
}

void RbeVisualizeWidget_GraphicsScene::addPoint()
{  
  RbeVisualizeWidget_PolygonItem *pItem = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem*>(mContextItems.value(mDeleteItemId));
  //add point code here
  pItem->addPoint(mScenePos);
}

void RbeVisualizeWidget_GraphicsScene::removePoint()
{
  RbeVisualizeWidget_PolygonItem *pItem = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem*>(mContextItems.value(mDeleteItemId));
  //add point code here
  pItem->removePoint(mScenePos);
}

void RbeVisualizeWidget_GraphicsScene::increaseSize()
{
  RbeVisualizeWidget_PolygonItem *pItem = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem*>(mContextItems.value(mDeleteItemId));
  //add point code here
  pItem->increaseSize(mScenePos);
}

void RbeVisualizeWidget_GraphicsScene::decreaseSize()
{
  RbeVisualizeWidget_PolygonItem *pItem = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem*>(mContextItems.value(mDeleteItemId));
  //add point code here
  pItem->decreaseSize(mScenePos);
}

void RbeVisualizeWidget_GraphicsScene::setLinkToContainer(RbeGeneralContainer *container)
{
  this->mLinkToContainer = container;
}

RbeGeneralContainer * RbeVisualizeWidget_GraphicsScene::getLinkToContainer()
{
  return mLinkToContainer;
}

void RbeVisualizeWidget_GraphicsScene::setupGuiFromContextItems(RbeGeneralContainer *container)
{
  //clear thing first
  mContextItems.clear();
  this->items().clear();
  
  //draw items
  QList<uint>keys = container->getGeneralContextItems()->uniqueKeys();  
  for(int i = 0 ; i < keys.size(); i++)
  {
    uint aKey = keys[i];
    RbeGeneralItem *generalItem = container->getGeneralContextItems()->value(aKey);                 
    
    if(generalItem->getType() == RbeGeneralItem::TRIPWIRE)
    {
      Q_ASSERT(generalItem->getPoints().size() == 2);
      RbeVisualizeWidget_LineItem *lineItem = new RbeVisualizeWidget_LineItem(generalItem->getPoints()[0],generalItem->getPoints()[1]);
      lineItem->setID(generalItem->getID());
      mContextItems.insert(lineItem->getID(),lineItem);
      
      lineItem->setPen(QPen(QColor(Qt::red), 3));
      this->addItem(lineItem);          
    }
    
    if(generalItem->getType() == RbeGeneralItem::AREA)
    {
      Q_ASSERT(generalItem->getPoints().size() > 2);
      RbeVisualizeWidget_PolygonItem *polygonItem = new RbeVisualizeWidget_PolygonItem(generalItem->getPoints());
      polygonItem->setID(generalItem->getID());
      polygonItem->setColor(generalItem->getColor());
      mContextItems.insert(polygonItem->getID(),polygonItem);
      
      this->addItem(polygonItem);          
    }
  }
}

void RbeVisualizeWidget_GraphicsScene::setMode(int mode)
{
  itemMode = Mode(mode);
}

void RbeVisualizeWidget_GraphicsScene::keepSceneRect()
{
  qreal sceneRectWidth = 384;
  qreal sceneRectHeight = 288;
  this->setSceneRect(0,0,sceneRectWidth,sceneRectHeight);
}

void RbeVisualizeWidget_GraphicsScene::focusOnItem(uint itemID)
{
  mContextItems.value(itemID)->setFocus();
}

void RbeVisualizeWidget_GraphicsScene::sendDeleteItemActionFromGraphicsItem(QGraphicsItem *item,uint itemID, int eType )
{
  mLinkToContainer->handleEventsFromDrawScene(item,itemID,eType);
}
void RbeVisualizeWidget_GraphicsScene::sendMousePressSignalFromGraphicsItem(QGraphicsItem *item,uint itemID, int eType)
{
  mLinkToContainer->handleEventsFromDrawScene(item,itemID,eType);
}

void RbeVisualizeWidget_GraphicsScene::sendMouseMoveSignalFromGraphicsItem(QGraphicsItem *item,uint itemID, int eType)
{
  mLinkToContainer->handleEventsFromDrawScene(item,itemID,eType);    
}

void RbeVisualizeWidget_GraphicsScene::sendMouseReleaseSignalFromGraphicsItem(QGraphicsItem *item,uint itemID, int eType)
{
  mLinkToContainer->handleEventsFromDrawScene(item,itemID,eType);
}

void RbeVisualizeWidget_GraphicsScene::sendAddNewItemSignal(QGraphicsItem *item,uint itemID, int eType)
{
  mLinkToContainer->handleEventsFromDrawScene(item,itemID,eType);
}

void RbeVisualizeWidget_GraphicsScene::sendBackToSelectionModeSignal(QGraphicsItem *item, uint itemID , int eType )
{
  mLinkToContainer->handleEventsFromDrawScene(item,itemID,eType);
}

void RbeVisualizeWidget_GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{   
  //if click with right-mouse button, only the contextMenu popup
  //nothing here to do
  if(mouseEvent->button() == Qt::RightButton)
    return;
  
  //qDebug() << "mouseGrabberItem: " << this->mouseGrabberItem();
  
  /* IMPORTANT:
     this code must be placed here so this mousePressEvent will
     delegate down to all other graphicsItems below (namely LineItem and PolygonItem)
     Don't remove this code
   */
  QGraphicsScene::mousePressEvent(mouseEvent);  
  
  /* If cannot grab any item below
     then do this set of code
    */
  if(this->mouseGrabberItem() == 0)
  {
    if(mouseEvent->button() ==  Qt::LeftButton)
    {      
      if(itemMode == INSERT_LINE_MODE)
      {       
        tempLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                                mouseEvent->scenePos()));
        tempLine->setPen(QPen(QColor(Qt::red), 3));
        this->addItem(tempLine);    
      }
      
      if(itemMode == INSERT_POLYGON_MODE)
      {     
        polygon = new RbeVisualizeWidget_PolygonItem(mouseEvent->scenePos().toPoint());    
        polygon->generateRandomColor();
        
        this->addItem(polygon);    
        
        //add new general item for polygon item here
        sendAddNewItemSignal(polygon);
        
        //add to hashtable, add after the sendsignal so the line item has an correct ID
        mContextItems.insert(polygon->getID(),polygon);
      }  
    }        
  }
  
  this->update();
}

void RbeVisualizeWidget_GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QGraphicsScene::mouseMoveEvent(mouseEvent);
  
  //qDebug() << "mouseGrabberItem: " << this->mouseGrabberItem();
  if(this->mouseGrabberItem() == 0)
  {
    if(itemMode == INSERT_LINE_MODE && tempLine != 0)
    { 
      QLineF newLine(tempLine->line().p1(), mouseEvent->scenePos());
      
      tempLine->setLine(newLine);       
    }
  }  
  
  this->update();
}

void RbeVisualizeWidget_GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QGraphicsScene::mouseReleaseEvent(mouseEvent); 
    
  if(this->mouseGrabberItem() == 0)
  {
    
    //BUGS.txt -> 1
    if(itemMode == INSERT_LINE_MODE && tempLine != 0)
    {        
      line = new RbeVisualizeWidget_LineItem(tempLine->line().p1().toPoint(),tempLine->line().p2().toPoint());
      line->setPen(QPen(QColor(Qt::red), 3));
      addItem(line);
      
      //add new general item for line item here
      sendAddNewItemSignal(line);
      
      //add to hashtable, add after the sendsignal so the line item has an correct ID
      mContextItems.insert(line->getID(),line);
      
      //clear tempLine
      removeItem(tempLine);
      delete tempLine;
      tempLine = 0;     
      
    }
  }
  
  //restore the selectionmode button
  itemMode = ITEM_SELECTION_MODE;  
  this->sendBackToSelectionModeSignal(line);
  
  this->update();
}

//QGraphicsScene RbeVisualizeWidget_GraphicsScene::renderToMask()
//{
//  this->setBackgroundBrush(QBrush(QColor(Qt::black)));
//}

void RbeVisualizeWidget_GraphicsScene::clearAllItems()
{
  mContextItems.clear();  
  this->clear();
}

void RbeVisualizeWidget_GraphicsScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
  QGraphicsScene::drawBackground(painter,rect);
}
