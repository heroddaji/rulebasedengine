#ifndef RBEVISUALIZEWIDGET_GRAPHICSVIEW_HPP
#define RBEVISUALIZEWIDGET_GRAPHICSVIEW_HPP

#include <QGraphicsView>
#include "RbeVisualizeWidget_GraphicsScene.hpp"

class RbeVisualizeWidget_GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit RbeVisualizeWidget_GraphicsView(QWidget *parent = 0);

signals:

public slots:
    
private:
    
protected:
 //   void drawBackground ( QPainter * painter, const QRectF & rect );
   
    
};

#endif // RBEVISUALIZEWIDGET_GRAPHICSVIEW_HPP
