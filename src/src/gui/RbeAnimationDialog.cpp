#include "RbeAnimationDialog.hpp"
#include "ui_RbeAnimationDialog.h"
#include "src/core/Engine.hpp"
#include "src/core/ContextArea.hpp"
#include "src/core/ContextTripwire.hpp"
#include "src/core/Rule.hpp"
#include "src/core/EventContainer.hpp"
#include "src/core/Event.hpp"
#include "src/core/Object.hpp"
#include "src/core/ObjectFrame.hpp"
#include "src/core/Misc.hpp"

RbeAnimationDialog::RbeAnimationDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::RbeAnimationDialog)
{
  
  rbeEngine = new Rbe::Engine();
  rbeEngine->load("../trunk/data/contexts.xml","../trunk/data/rules.xml","../trunk/data/objects_output.xml");  
  
  ui->setupUi(this);
  gScene = new QGraphicsScene();
  ui->setupUi(this);
  ui->graphicsView->setScene(gScene);
  
  timerForDraw = new QTimer();
  timerForDraw->setInterval(30);
  connect(ui->spinBox,SIGNAL(valueChanged(int)),
          this,SLOT(on_spinBox_valueChanged(int)));
  
  connect(timerForDraw, SIGNAL(timeout()), this, SLOT(draw()));
  
  
  index = 0;      
  this->drawMask();  
  
}

void RbeAnimationDialog::start()
{
  timerForDraw->start();
}

void RbeAnimationDialog::draw()
{
  gScene->clear();
  this->drawObjects();
  
  //draw frame
  gScene->addText(QString::number(index));    
  
  index++;
  if(index == rbeEngine->frameCounter)
  { 
    timerForDraw->stop();
  }
}

void RbeAnimationDialog::drawMask()
{
  
  //draw the mask file
  QString path = QString::fromStdString(rbeEngine->getMaskPath());                        
  QPixmap pixmap(path);
  
  QImage image = pixmap.toImage();
  QRgb pixColor;
  
  //tranform color
  for (int i = 0; i < pixmap.width(); ++i)
  {
    for (int j = 0; j < pixmap.height(); ++j)
    {
      pixColor = image.pixel(i, j);                    
      bool value = false;
      
      for(unsigned k = 0 ; k < rbeEngine->getContexts().size(); k++)
      {
        if(rbeEngine->getContexts()[k]->getType() == Rbe::Context::AREA)
        {
          Rbe::ContextArea *area = static_cast<Rbe::ContextArea *>(rbeEngine->getContexts()[k]);
          if(QColor::fromRgb(pixColor) == area->getColor())   
          {
            value = true;
          }
        }        
      }
      
      if(value == false)
      {
        image.setPixel(i, j, qRgb(255,255,155));
      }          
    }
  }
  pixmap = pixmap.fromImage(image);
  gScene->addPixmap(pixmap);
  gScene->setBackgroundBrush(QBrush(pixmap));
  
}

void RbeAnimationDialog::drawTripwires()
{
  //draw tripwire
  for(unsigned i = 0; i < rbeEngine->getContexts().size(); i++ )
  {
    
    if(rbeEngine->getContexts()[i]->getType() == Rbe::Context::TRIPWIRE)
    {
      Rbe::ContextTripwire *ctx = static_cast<Rbe::ContextTripwire *>(rbeEngine->getContexts()[i]);
      
      
      Rbe::Line *aLine = ctx->getLine();
      Rbe::Point p1 = aLine->point1;
      Rbe::Point p2 = aLine->point2;            
      
      qreal x1 = p1.x;
      qreal y1 = p1.y;
      qreal x2 = p2.x;
      qreal y2 = p2.y;
      
      gScene->addLine(x1,y1,x2,y2);
      
    }
  }
}

void RbeAnimationDialog::drawObjects()
{
  this->drawTripwires();
  
  //  //draw each object frames and its trajectory
  //  for(unsigned i=0; i < rbeEngine->getObjects().size(); i++)
  //  {
  //    Rbe::Object *aObj = rbeEngine->getObjects()[i];       
  
  //    //detect event
  //    rbeEngine->detectEvent(rbeEngine->getRules(),rbeEngine->getContexts(),rbeEngine->getObjects());
  
  //    //then draw
  //    Rbe::ObjectFrame *AOFrame = aObj->currentObjectFrameFromTheIterator();
  
  //    if (aObj->checkForValidObjectFrame(index))
  //    {
  //      //draw object bbox
  //      gScene->addRect(AOFrame->getX(),AOFrame->getY(),AOFrame->getWidth(),AOFrame->getHeight());            
  
  //      //draw it trajectory
  //      unsigned numberOfFrame = 5;
  //      if(AOFrame->getTrajectories().size() > numberOfFrame)
  //      {
  
  //        for(unsigned j=0; j < AOFrame->getTrajectories().size() - 1 ;j++)
  //        {
  //          Rbe::Point *p1 = AOFrame->getTrajectories()[j];
  //          Rbe::Point *p2 = AOFrame->getTrajectories()[j+1];
  //          gScene->addLine(p1->x,p1->y,p2->x,p2->y);
  //        }
  //      }          
  
  
  //    }        
  
  //    //advance to next object frame in each objects
  //    rbeEngine->nextObjectFrame(aObj,index);
  //  }
}



RbeAnimationDialog::~RbeAnimationDialog()
{
  delete ui;
  delete rbeEngine;
}


void RbeAnimationDialog::on_buttonBox_accepted()
{
  timerForDraw->stop();
  rbeEngine->clear();
}

void RbeAnimationDialog::on_buttonBox_rejected()
{
  timerForDraw->stop();
  rbeEngine->clear();
}

void RbeAnimationDialog::on_spinBox_valueChanged(int n)
{
  timerForDraw->setInterval(n);
}

