#ifndef RBEVISUALIZEWIDGET_LINEITEM_HPP
#define RBEVISUALIZEWIDGET_LINEITEM_HPP

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsScene>

class RbeVisualizeWidget_GraphicsScene;

class RbeVisualizeWidget_LineItem : public QGraphicsLineItem
{  
public:      
  RbeVisualizeWidget_LineItem(QPoint p1, QPoint p2, QGraphicsItem *parent=0, QGraphicsScene *scene=0);
  
  enum { Type = UserType + 1 };
  
  int type() const { return Type;} 
  
  void setID(uint id){mId = id;}
  uint getID() const {return mId;}
  
  QList<QPoint> getPoints(){return m_points;}    
  QList<QPoint> getMapToScenePoints();
  
private:    
  uint mId;
  QList<QPoint> m_points;  
  QPoint m_p1;
  QPoint m_p2;
  int m_activePoint;
  int m_pointSize;
  
  RbeVisualizeWidget_GraphicsScene *mScene;
    
protected:
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);    
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *event);
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );
        
};

#endif // RBEVISUALIZEWIDGET_LINEITEM_HPP
