#ifndef RBEVISUALIZEWIDGET_POLYGONITEM_HPP
#define RBEVISUALIZEWIDGET_POLYGONITEM_HPP

#include <QtCore/qmath.h>
#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QTime>
#include <QGraphicsSceneContextMenuEvent>

class RbeVisualizeWidget_GraphicsScene;

class RbeVisualizeWidget_PolygonItem:  public QGraphicsPolygonItem
{
  
public:
    RbeVisualizeWidget_PolygonItem(QPoint pos,QGraphicsItem *parent=0, QGraphicsScene *scene=0);
    RbeVisualizeWidget_PolygonItem(QList<QPoint> posList,QGraphicsItem *parent=0, QGraphicsScene *scene=0);
    
    void initPolygon(QList<QPoint> list);
    
    enum { Type = UserType + 2 };    
    
    int type() const { return Type;}
    
    void setID(uint id){mId = id;}
    uint getID() const{return mId;}
    
    QList<QPoint> getPoints(){return m_points;}    
    QList<QPoint> getMapToScenePoints();
    
    void setColor(int r, int g, int b, int a);
    void setColor(QColor color);
    QColor getColor(){return mBrushColor;}
    void generateRandomColor();       
    
    void addPoint(QPointF scenePoint);
    void removePoint(QPointF scenePoint);
    void increaseSize(QPointF scenePoint);
    void decreaseSize(QPointF scenePoint);
        
private:
    RbeVisualizeWidget_GraphicsScene *mScene;
    uint mId;
    QList<QPoint>m_points;  
    int m_activePoint;
    int m_pointSize;    
    int numOfVertex;
    int radius;
    QPoint radiusPoint;
    
    QColor mBrushColor;
    
    void generatePolygonPath();    
    
protected:
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget);    
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *event);
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );
};

#endif // RBEVISUALIZEWIDGET_POLYGONITEM_HPP
