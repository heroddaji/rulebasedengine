#include "RbeVisualizeWidget_PolygonItem.hpp"

#include "RbeGeneralContainer.hpp"
#include "RbeGeneralItem.hpp"
#include "RbeVisualizeWidget_GraphicsScene.hpp"

RbeVisualizeWidget_PolygonItem::RbeVisualizeWidget_PolygonItem(QPoint pos,
                                                               QGraphicsItem *parent,
                                                               QGraphicsScene *scene)
                                                               :QGraphicsPolygonItem(parent,scene)
{      
  QList<QPoint> posList;
  posList.push_back(pos);
  this->initPolygon(posList);  
}

RbeVisualizeWidget_PolygonItem::RbeVisualizeWidget_PolygonItem(QList<QPoint> posList,
                                                               QGraphicsItem *parent,
                                                               QGraphicsScene *scene)
  :QGraphicsPolygonItem(parent,scene)
{  
  this->initPolygon(posList);
}

void RbeVisualizeWidget_PolygonItem::initPolygon(QList<QPoint> list)
{
  m_pointSize = 4;    
  radius = 40;
  qreal PI = 3.14;  
  numOfVertex = 6;   
  
  this->setColor(Qt::green);
  
  if(list.size() == 1)
  {        
    radiusPoint = list[0];  
    for (int i = 0; i < numOfVertex; i++) {    
      qreal angle = (double)(2 * PI * i / numOfVertex);
      qreal x = radius * qCos(angle);
      qreal y =  radius * qSin(angle);        
      QPoint p(x,y);
      p = p + radiusPoint;
      m_points.push_back(p);
    }
  }
  else
  {
    numOfVertex = list.size();
    
    for (int i = 0; i <  list.size(); i++) {         
      m_points.push_back(list[i]);
    }
  }
  
  QPolygonF pol;
  for(int i = 0 ; i < m_points.size(); i++)
  {
    pol << m_points[i];
  }
  
  setPolygon(pol);
  
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  setFlag(QGraphicsItem::ItemIsFocusable,true);
}

void RbeVisualizeWidget_PolygonItem::generatePolygonPath()
{  
  qreal PI = 3.14;    
  
  m_points.clear();
  
  for (int i = 0; i < numOfVertex; i++) {    
    qreal angle = (double)(2 * PI * i / numOfVertex);
    qreal x = radius * qCos(angle);
    qreal y =  radius * qSin(angle);        
    QPoint p(x,y);
    p = p + radiusPoint;
    m_points.push_back(p);
  }
  
  QPolygonF pol;
  for(int i = 0 ; i < m_points.size(); i++)
  {
    pol << m_points[i];
  }
  
  setPolygon(pol);
}

void RbeVisualizeWidget_PolygonItem::generateRandomColor()
{  
  qsrand((uint)QTime::currentTime().msec()); 
  int r = qrand() % 128 + 127;
  int g = qrand() % 128 + 127;
  int b = qrand() % 128 + 127;
  int a = 255;
    
  this->setColor(r,g,b,a);  
}

void RbeVisualizeWidget_PolygonItem::setColor(int r, int g, int b, int a)
{
  mBrushColor = QColor(r,g,b,a);
  this->setPen(QPen(QColor(Qt::red), 3));       
  this->setBrush(QBrush(mBrushColor));       
}

void RbeVisualizeWidget_PolygonItem::setColor(QColor color)
{
  mBrushColor = color;
  this->setPen(QPen(QColor(Qt::red), 3));       
  this->setBrush(QBrush(mBrushColor));       
}

QList<QPoint> RbeVisualizeWidget_PolygonItem::getMapToScenePoints()
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

void RbeVisualizeWidget_PolygonItem::addPoint(QPointF scenePoint)
{
  scenePoint = mapFromScene(scenePoint);    
  
  m_activePoint = -1;
  qreal min = 999999;
  qreal distance = 999999;
  for (int i=0; i< m_points.size(); ++i) {
    distance = QLineF(scenePoint, m_points.at(i)).length();
    
    //use min to detect the point index
    if (min >=  distance) 
    {     
      min = distance;
      m_activePoint = i;      
    }
    
//    if (distance <=  m_pointSize*3) 
//    {    
//      m_activePoint = i;      
//    }
  }    
  if(m_activePoint != -1)
  {
    m_points.insert(m_activePoint,scenePoint.toPoint());
    
    QPolygonF pol;
    for(int i = 0 ; i < m_points.size(); i++)
    {
      pol << m_points[i];
    }
    
    setPolygon(pol);  
  }
}

void RbeVisualizeWidget_PolygonItem::removePoint(QPointF scenePoint)
{
  scenePoint = mapFromScene(scenePoint);    
  
  m_activePoint = -1;  
  qreal distance = -1;
  for (int i=0; i< m_points.size(); ++i) {
    distance = QLineF(scenePoint, m_points.at(i)).length();
    if (distance <=  m_pointSize*2) 
    {    
     m_activePoint = i;      
    }
  }    
  
  if(m_activePoint != -1)
  {
    m_points.removeAt(m_activePoint);
    
    QPolygonF pol;
    for(int i = 0 ; i < m_points.size(); i++)
    {
      pol << m_points[i];
    }
    
    setPolygon(pol);    
  }  
}

void RbeVisualizeWidget_PolygonItem::increaseSize(QPointF scenePoint)
{
  QPoint p = mapFromScene(scenePoint).toPoint();
  int size = 5;
  for(int i = 0 ; i < m_points.size(); i++)
  {
    if(m_points[i].x() <= p.x() && m_points[i].y() <= p.y())
    {
      //-x,-y      
      m_points[i].setX(m_points[i].x() - size );
      m_points[i].setY(m_points[i].y() - size );
    }
    
    if(m_points[i].x() > p.x() && m_points[i].y() <= p.y())
    {
      //+x,-y
      m_points[i].setX(m_points[i].x() + size );
      m_points[i].setY(m_points[i].y() - size );
    }
    
    if(m_points[i].x() > p.x() && m_points[i].y() > p.y())
    {
      //+x,+y
      m_points[i].setX(m_points[i].x() + size );
      m_points[i].setY(m_points[i].y() + size );
    }
    
    if(m_points[i].x() <= p.x() && m_points[i].y() > p.y())
    {
      //-x,+y
      m_points[i].setX(m_points[i].x() - size );
      m_points[i].setY(m_points[i].y() + size );
    }
  }
  
  QPolygonF pol;
  for(int i = 0 ; i < m_points.size(); i++)
  {
    pol << m_points[i];
  }
  
  setPolygon(pol);    
}

void RbeVisualizeWidget_PolygonItem::decreaseSize(QPointF scenePoint)
{
  QPoint p = mapFromScene(scenePoint).toPoint();
  int size = 5;
  for(int i = 0 ; i < m_points.size(); i++)
  {
    if(m_points[i].x() <= p.x() && m_points[i].y() <= p.y())
    {
      //-x,-y      
      m_points[i].setX(m_points[i].x() + size );
      m_points[i].setY(m_points[i].y() + size );
    }
    
    if(m_points[i].x() > p.x() && m_points[i].y() <= p.y())
    {
      //+x,-y
      m_points[i].setX(m_points[i].x() - size );
      m_points[i].setY(m_points[i].y() + size );
    }
    
    if(m_points[i].x() > p.x() && m_points[i].y() > p.y())
    {
      //+x,+y
      m_points[i].setX(m_points[i].x() - size );
      m_points[i].setY(m_points[i].y() - size );
    }
    
    if(m_points[i].x() <= p.x() && m_points[i].y() > p.y())
    {
      //-x,+y
      m_points[i].setX(m_points[i].x() + size );
      m_points[i].setY(m_points[i].y() - size );
    }
  } 
 
  QPolygonF pol;
  for(int i = 0 ; i < m_points.size(); i++)
  {
    pol << m_points[i];
  }
  
  setPolygon(pol);    
}

void RbeVisualizeWidget_PolygonItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setRenderHint(QPainter::Antialiasing);  
 
  QGraphicsPolygonItem::paint(painter,option,widget);
  
  painter->setPen(QPen(QColor(Qt::blue), 3)); 
  painter->setBrush(QBrush(Qt::blue));              
  for (int i=0; i<m_points.size(); ++i) 
  {
    QPoint pos = m_points[i];    
    painter->drawEllipse(QRectF(pos.x()-m_pointSize/2,pos.y()-m_pointSize/2,
                                m_pointSize, m_pointSize));
  }  
}

void RbeVisualizeWidget_PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{   
  
  radiusPoint = mapFromScene(mouseEvent->scenePos()).toPoint();
  
  m_activePoint = -1;
  qreal distance = -1;
  for (int i=0; i<m_points.size(); ++i) {
    distance = QLineF(mapFromScene(mouseEvent->scenePos()), m_points.at(i)).length();
    if (distance <=  m_pointSize*2) {      
      m_activePoint = i;
      
      setFlag(QGraphicsItem::ItemIsMovable, false);
    }
  }  
  
  //send press signal back to scene
  if(this->scene() != 0)
  { 
    mScene = static_cast<RbeVisualizeWidget_GraphicsScene *>(scene());
    mScene->sendMousePressSignalFromGraphicsItem(this,mId);
  }      
}

void RbeVisualizeWidget_PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QGraphicsPolygonItem::mouseMoveEvent(mouseEvent);  
  
  radiusPoint = mapFromScene(mouseEvent->scenePos()).toPoint();
  
  if(m_activePoint != -1)
  {
    m_points[m_activePoint] = mapFromScene(mouseEvent->scenePos()).toPoint();     
    setPolygon(QPolygon(m_points.toVector()));
  }
 
  if(this->scene() != 0)
  { 
    mScene = static_cast<RbeVisualizeWidget_GraphicsScene *>(scene());
    mScene->sendMouseMoveSignalFromGraphicsItem(this,mId);
  } 
}

void RbeVisualizeWidget_PolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  QGraphicsPolygonItem::mouseReleaseEvent(mouseEvent);  
  
  radiusPoint = mapFromScene(mouseEvent->scenePos()).toPoint();
  
  if(m_activePoint != -1)
  {
    m_points[m_activePoint] = mapFromScene(mouseEvent->scenePos()).toPoint();
    m_activePoint = -1;
  }
  
  setFlag(QGraphicsItem::ItemIsMovable, true);  
}

void RbeVisualizeWidget_PolygonItem::keyPressEvent(QKeyEvent *event)
{
  
  if(event->key() == Qt::Key_Up)
  {
    numOfVertex++;
    this->generatePolygonPath();
  }
  if(event->key() == Qt::Key_Down)
  {
    numOfVertex >3 ? --numOfVertex :numOfVertex ;
    this->generatePolygonPath();
  }
  if(event->key() == Qt::Key_Left)
  {
    radius += 5;
    this->generatePolygonPath();
  }
  if(event->key() == Qt::Key_Right)
  {
    radius -= 5;
    this->generatePolygonPath();
  }
  
}

void RbeVisualizeWidget_PolygonItem::focusInEvent ( QFocusEvent * event )
{
  this->setSelected(true);
}

void RbeVisualizeWidget_PolygonItem::focusOutEvent ( QFocusEvent * event )
{
  this->setSelected(false);
}

