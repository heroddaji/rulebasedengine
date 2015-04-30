#include "RbeVisualizeWidget_LineItem.hpp"
  
#include "RbeVisualizeWidget_GraphicsScene.hpp"

RbeVisualizeWidget_LineItem::RbeVisualizeWidget_LineItem(QPoint p1, QPoint p2,
                                                         QGraphicsItem *parent, 
                                                         QGraphicsScene *scene)
  :QGraphicsLineItem(parent,scene)
{ 
 
  m_p1 = p1;
  m_p2 = p2;
  m_points << m_p1;
  m_points << m_p2;
  
  m_pointSize = 4;
  
  setLine(QLine(p1,p2));
  
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  setFlag(QGraphicsItem::ItemIsFocusable,true);  
}

QList<QPoint> RbeVisualizeWidget_LineItem::getMapToScenePoints()
{
  QList<QPoint> mP;
  for (int i = 0 ; i < m_points.size(); i++)
  {
    QPoint mapPoint = mapToScene(m_points[i]).toPoint();
    mP.push_back(mapPoint);
  }
  Q_ASSERT(mP.size() >= 2);
  
  return mP;
}

void RbeVisualizeWidget_LineItem::paint(QPainter *painter,
                                        const QStyleOptionGraphicsItem *option,
                                        QWidget *widget)
{ 
  painter->setRenderHint(QPainter::Antialiasing);    
  painter->setPen(QPen(QColor(Qt::red), 3));       
  painter->setBrush(QBrush(Qt::green));   
  QGraphicsLineItem::paint(painter,option,widget);
  
  // Draw the control points      
  painter->setPen(QPen(QColor(Qt::blue), 3)); 
  painter->setBrush(QBrush(Qt::blue));     
  for (int i=0; i<m_points.size(); ++i) 
  {
    QPoint pos = m_points[i];    
    painter->drawEllipse(QRectF(pos.x()-m_pointSize/2,pos.y()-m_pointSize/2,
                                m_pointSize, m_pointSize));
  }
  
//  if(isSelected())
//  {    
//    QPoint p = mapToScene(this->getPoints()[0]).toPoint();
//    painter->drawText(100,100,QString::number(p.x()));
//  }
}    


void RbeVisualizeWidget_LineItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{ 
  QGraphicsLineItem::mousePressEvent(mouseEvent);
  
  m_activePoint = -1;
  qreal distance = -1;
  for (int i=0; i< m_points.size(); ++i) {
    distance = QLineF(mapFromScene(mouseEvent->scenePos()), m_points.at(i)).length();
    if (distance <=  m_pointSize*2) {      
      m_activePoint = i;
      
      setFlag(QGraphicsItem::ItemIsMovable, false);
    }
  }  
  
  
  //send signal back to scene
  if(this->scene() != 0)
  { 
    mScene = static_cast<RbeVisualizeWidget_GraphicsScene *>(scene());
    mScene->sendMousePressSignalFromGraphicsItem(this,mId);
  } 
}

void RbeVisualizeWidget_LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{ 
  QGraphicsLineItem::mouseMoveEvent(mouseEvent);     
  
  if(m_activePoint != -1)
  {
    m_points[m_activePoint] = mapFromScene(mouseEvent->scenePos()).toPoint();
    setLine(QLine(m_points[0],m_points[1]));
  }
  
  //send  signal back to scene
  if(this->scene() != 0)
  { 
    mScene = static_cast<RbeVisualizeWidget_GraphicsScene *>(scene());
    mScene->sendMouseMoveSignalFromGraphicsItem(this,mId);
  } 
}

void RbeVisualizeWidget_LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QGraphicsLineItem::mouseReleaseEvent(mouseEvent);
  
  if(m_activePoint != -1)
  {
    m_points[m_activePoint] = mapFromScene(mouseEvent->scenePos()).toPoint();
    m_activePoint = -1;
  }
  
  setFlag(QGraphicsItem::ItemIsMovable, true);  
  
  //send signal back to scene
  if(this->scene() != 0)
  { 
    mScene = static_cast<RbeVisualizeWidget_GraphicsScene *>(scene());
    mScene->sendMouseReleaseSignalFromGraphicsItem(this,mId);
  } 
}

void RbeVisualizeWidget_LineItem::keyPressEvent(QKeyEvent *event)
{
  
  if(event->key() == Qt::Key_Up)
  {
    
  }
  if(event->key() == Qt::Key_Down)
  {
    
  }
  if(event->key() == Qt::Key_Left)
  {
    
  }
  if(event->key() == Qt::Key_Right)
  {
    
  }
  
}

void RbeVisualizeWidget_LineItem::focusInEvent ( QFocusEvent * event )
{
  this->setSelected(true);
}

void RbeVisualizeWidget_LineItem::focusOutEvent ( QFocusEvent * event )
{
  this->setSelected(false);
}

