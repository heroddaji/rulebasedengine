#ifndef RBEVISUALIZEWIDGET_GRAPHICSSCENE_HPP
#define RBEVISUALIZEWIDGET_GRAPHICSSCENE_HPP

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QtDebug>

class RbeVisualizeWidget_LineItem;
class RbeVisualizeWidget_PolygonItem;
class RbeGeneralContainer;

class RbeVisualizeWidget_GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit RbeVisualizeWidget_GraphicsScene(QObject *parent = 0);    
  
    enum Mode
    {
      INSERT_LINE_MODE, 
      INSERT_POLYGON_MODE, 
      ITEM_SELECTION_MODE , 
      OPEN_VIDEO_MODE, 
      NOTHING_MODE
    };
    
    enum EventType
    {
      MOUSE_PRESS = 999,
      MOUSE_MOVE,
      MOUSE_RELEASE, 
      NEW_ITEM, 
      BACK_TO_SELECTION_MODE,
      DELETE_ITEM
    };
    
    //this enum must be synchronize with type of subclass graphic item to make it work    
    enum GraphicItemType
    {
      LINE_ITEM = QGraphicsItem::UserType + 1, 
      POLYGON_ITEM
    };
    
    void setupGuiFromContextItems(RbeGeneralContainer *);
    void focusOnItem(uint itemID);
    
    void setLinkToContainer(RbeGeneralContainer *);
    RbeGeneralContainer * getLinkToContainer();
    
     QHash<uint,QGraphicsItem *>* getHashTable(){return &mContextItems;}
    
    void sendDeleteItemActionFromGraphicsItem(QGraphicsItem *,uint itemID, int eType = DELETE_ITEM);
    void sendMousePressSignalFromGraphicsItem(QGraphicsItem *,uint itemID, int eType = MOUSE_PRESS);
    void sendMouseMoveSignalFromGraphicsItem(QGraphicsItem *,uint itemID, int eType = MOUSE_MOVE);
    void sendMouseReleaseSignalFromGraphicsItem(QGraphicsItem *,uint itemID, int eType = MOUSE_RELEASE);
    void sendAddNewItemSignal(QGraphicsItem *,uint itemID = 99, int eType = NEW_ITEM);
    void sendBackToSelectionModeSignal(QGraphicsItem *,uint itemID = 99, int eType = BACK_TO_SELECTION_MODE);
    
    void clearAllItems();
    
protected:
    void contextMenuEvent ( QGraphicsSceneContextMenuEvent * contextMenuEvent );
    
private:
    RbeGeneralContainer *mLinkToContainer;
    
    RbeVisualizeWidget_LineItem *line;
    QGraphicsLineItem *tempLine;    
    RbeVisualizeWidget_PolygonItem *polygon;    
    Mode itemMode;
    
    int mDeleteItemId;
    QPointF mScenePos;
    QHash<uint,QGraphicsItem *> mContextItems;    
       
    
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void drawBackground ( QPainter * painter, const QRectF & rect );
   
public slots:
    void setMode(int mode);
    void keepSceneRect();    
    
private slots:
      void deleteItem();
      void addPoint();
      void removePoint();
      void increaseSize();
      void decreaseSize();
    
};

#endif // RBEVISUALIZEWIDGET_GRAPHICSSCENE_HPP
