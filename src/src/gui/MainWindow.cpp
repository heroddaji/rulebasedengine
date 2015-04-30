/**
  * This project need 2 folders to run the executable file.
  * firs folder is "data" same level with the executable file.
  * second folder is data/.temp
  * third folder is data/.temp/virtualFencing temp
  */


#include "MainWindow.hpp"

#include <string>
#include <exception>

#include "src/gui/RbeItemTree.hpp"
#include "src/gui/RbePropertyTree.hpp"
#include "src/gui/RbeVisualizeWidget.hpp"
#include "src/gui/RbeVisualizeWidget_GraphicsScene.hpp"
#include "src/gui/RbeVisualizeWidget_PolygonItem.hpp"
#include "src/gui/RbeVisualizeWidget_LineItem.hpp"
#include "src/gui/RbeVisualizeWidget_GraphicsView.hpp"
#include "src/gui/RbeVisualizeWidget_RuleTree.hpp"
#include "src/gui/RbeXmlHandler.hpp"
#include "src/gui/RbeGeneralContainer.hpp"
#include "src/gui/RbeVirtualFence.hpp"

#include "extra/QTFFmpegWrapper/QVideoDecoder.h"

#include "src/gui/help.cpp"

//global
QString filepath = "./data/";

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)   
{  
  videoFilePath = "";
  
  checkAndGenerateNecessaryFile();
  createActions();
  createMenus();
  createToolbars();
  createSubViewsAndLayoutAndConnection();    
}

void MainWindow::checkAndGenerateNecessaryFile()
{
  QDir dir;
  QString path = dir.absolutePath();
  
  if(!dir.exists(path + "/data"))
    dir.mkdir(path + "/data");
  
  if(!dir.exists(path + "/data/.temp"))
    dir.mkdir(path + "/data/.temp");
  
  if(!dir.exists(path + "/data/.temp/VirtualFence"))
    dir.mkdir(path + "/data/.temp/VirtualFence");
  
  QFile file;
  path = path + "/data/.temp/VirtualFence/";
  if (!file.exists(path + "config.ini"))
  {
    QString fileName = path + "config.ini";
    QFile file2(fileName);
    if (!file2.open(QFile::WriteOnly | QFile::Text))
    {
      QMessageBox::warning(this, tr("Create ini file"),
                           tr("Cannot create ini file %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
      return;
    }
    
    //write the ini file content here
    QTextStream out(&file2);         
    helper::writeDefautlVirtualFecingIni(out);
    file2.close();     
  }
}

void MainWindow::createSubViewsAndLayoutAndConnection()
{
  // UI classes
  rbeVisualizeWidget = new RbeVisualizeWidget();    
  rbePropertyTree = new RbePropertyTree();     
  rbeItemTree = new RbeItemTree();            
  
  //container for all general items
  generalContainer = new RbeGeneralContainer();    
  generalContainer->setLinkToItemTree(rbeItemTree);
  generalContainer->setLinkToPropertyTree(rbePropertyTree);
  generalContainer->setLinkToVisualizeWidget(rbeVisualizeWidget);
  generalContainer->setLinkToVisualizeWidget_Scene(rbeVisualizeWidget->getDrawScene());
  generalContainer->setLinkToVisualizeWidget_RuleTree(rbeVisualizeWidget->getRuleTree());
  generalContainer->setLinkToMainWindow(this);
  
  rbeVisualizeWidget->setLinkToContainer(generalContainer);
  
  if(!generalContainer->setUpSignalSlot())
  {
    Q_ASSERT(false);
  }
  
  // save/load class
  rbeXmlHandler = new RbeXmlHandler();  
  rbeXmlHandler->setGeneralContainer(generalContainer);
  
  //Layout
  layout = new QHBoxLayout;  
  layout->addWidget(rbeItemTree);
  layout->addWidget(rbeVisualizeWidget->getDrawView());  
  layout->addWidget(rbeVisualizeWidget->getRuleTree());
  layout->addWidget(rbePropertyTree);
  
  centralWidget = new QWidget();
  centralWidget->setLayout(layout);
  
  setCentralWidget(centralWidget);
  setWindowTitle(tr("Rule-based Engine Editor"));
}

void MainWindow::createActions()
{   
  
  newAction = new QAction("&New",this);
  newAction->setIcon(QIcon(":/images/new.png"));      
  newAction->setShortcut(tr("Ctrl+N"));
  newAction->setStatusTip("Create a new edit session");
  connect(newAction,SIGNAL(triggered()),
          this,SLOT(newFile()));
  
  openFileAction = new QAction("&Open file",this);
  openFileAction->setIcon(QIcon(":/images/open.png"));    
  openFileAction->setShortcut(tr("Ctrl+O"));
  openFileAction->setStatusTip("Open/load data from file");
  connect(openFileAction,SIGNAL(triggered()),
          this,SLOT(openFile()));
  
  
  
  openVideoAction= new QAction("Open video image",this);
  openVideoAction->setIcon(QIcon(":/images/video-x-generic.svg"));    
  //openVideoAction->setShortcut(tr("Ctrl+O+O"));
  //openVideoAction->setStatusTip("Open/load contexts data from file");
  connect(openVideoAction,SIGNAL(triggered()),
          this,SLOT(openVideoFile()));
  
  
  saveProjectAction= new QAction("&Save project",this);
  saveProjectAction->setIcon(QIcon(":/images/save.png"));     
  saveProjectAction->setShortcut(tr("Ctrl+P"));
  saveProjectAction->setStatusTip("Save data to project file");
  connect(saveProjectAction,SIGNAL(triggered()),
          this,SLOT(saveProjectFile()));
  
  saveContextsAction = new QAction("Save contexts",this);
  saveContextsAction->setIcon(QIcon(":/images/insert-object.svg"));     
  saveContextsAction->setShortcut(tr("Ctrl+S+C"));
  saveContextsAction->setStatusTip("Save contexts data to file");  
  connect(saveContextsAction,SIGNAL(triggered()),
          this,SLOT(saveContextsToFile())); 
  
  saveRulesAction = new QAction("Save rules",this);
  saveRulesAction->setIcon(QIcon(":/images/insert-object.svg"));     
  saveRulesAction->setShortcut(tr("Ctrl+S+C"));
  saveRulesAction->setStatusTip("Save rule data to file");  
  connect(saveRulesAction,SIGNAL(triggered()),
          this,SLOT(saveRulesToFile()));   
  
  
  exitAction = new QAction("&Exit",this);
  exitAction->setIcon(QIcon(":/images/application-exit.svg"));     
  exitAction->setShortcut(tr("Ctrl+Q"));
  exitAction->setStatusTip("Close application");
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));   
  
  aboutAction = new QAction("&About",this);
  aboutAction->setIcon(QIcon(":/images/find.png"));     
  aboutAction->setShortcut(tr("Ctrl+A"));
  aboutAction->setStatusTip("About");
  connect(aboutAction,SIGNAL(triggered()),
          this,SLOT(about()));
  
  newRuleAction = new QAction("&New rule",this);
  newRuleAction->setIcon(QIcon(":/images/rule.svg"));
  connect(newRuleAction,SIGNAL(triggered()),
          this,SLOT(newRule()));
  
  runVirtualFencingAction = new QAction("&Video",this);
  runVirtualFencingAction->setIcon(QIcon(":/images/arrow.png"));      
  connect(runVirtualFencingAction,SIGNAL(triggered()),
          this,SLOT(runVirtualFencing()));
  
  addLineAction = new QAction("Add &Line",this);
  addLineAction->setIcon(QIcon(":/images/linepointer.png"));     
  addLineAction->setShortcut(tr("Ctrl+L"));
  addLineAction->setStatusTip("Add line");
  connect(addLineAction,SIGNAL(triggered()),
          this,SLOT(about()));                  
  
  
}

void MainWindow::createMenus()
{  
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openFileAction);
  fileMenu->addAction(openVideoAction);
  fileMenu->addAction(saveProjectAction);  
  fileMenu->addAction(exitAction);
  
  ruleMenu = menuBar()->addMenu(tr("&Edit"));  
  ruleMenu->addAction(newRuleAction);
  
  aboutMenu = menuBar()->addMenu(tr("&Help"));
  aboutMenu->addAction(aboutAction);
}

void MainWindow::createToolbars()
{
  
  //////////////////
  QToolBar *fileToolBar;
  fileToolBar = addToolBar(tr("fileToolbar"));
  fileToolBar->addAction(newAction);
  fileToolBar->addAction(openFileAction);
  //  fileToolBar->addAction(saveAction);
  fileToolBar->addAction(saveProjectAction);
  
  //Button for contexts related
  contextItemsButtonGroup = new QButtonGroup(this);  
  QToolButton *selectItemToolButton = new QToolButton(this);  
  selectItemToolButton ->setCheckable(true);
  selectItemToolButton ->setChecked(true);
  selectItemToolButton ->setIcon(QIcon(":/images/pointer.png"));  
  
  QToolButton *insertLineToolButton = new QToolButton(this);
  insertLineToolButton->setCheckable(true);  
  insertLineToolButton->setIcon(QIcon(":/images/linepointer.png"));  
  
  QToolButton *insertPolygonToolButton = new QToolButton(this);
  insertPolygonToolButton->setCheckable(true);  
  insertPolygonToolButton->setIcon(QIcon(":/images/polygon.png"));  
  
  contextItemsButtonGroup->addButton(selectItemToolButton,int(RbeVisualizeWidget_GraphicsScene::ITEM_SELECTION_MODE));
  contextItemsButtonGroup->addButton(insertLineToolButton,int(RbeVisualizeWidget_GraphicsScene::INSERT_LINE_MODE));  
  contextItemsButtonGroup->addButton(insertPolygonToolButton,int(RbeVisualizeWidget_GraphicsScene::INSERT_POLYGON_MODE));
  
  QToolBar *contextItemsButtonGroupToolBar;    
  contextItemsButtonGroupToolBar = addToolBar(tr("contextItemsButtonGroup"));
  contextItemsButtonGroupToolBar->addWidget(selectItemToolButton);
  contextItemsButtonGroupToolBar->addWidget(insertLineToolButton);
  contextItemsButtonGroupToolBar->addWidget(insertPolygonToolButton);    
  
  connect(contextItemsButtonGroup, SIGNAL(buttonClicked(int)),
          this, SLOT(contextItemsButtonGroupClicked()));
  
  
  ////////////////////RULE TOOLBAR  
  QToolBar *ruleToolBar;
  ruleToolBar = addToolBar(tr("ruleToolbar"));
  ruleToolBar->addAction(newRuleAction);
  
  ////////////////////
  QToolBar *runToolBar;
  runToolBar = addToolBar(tr("runToolbar"));  
  //runToolBar->addAction(runAnimateAction);
  runToolBar->addAction(openVideoAction);
  runToolBar->addAction(runVirtualFencingAction);  
  //////////////////////
  QToolBar *exitToolBar;
  exitToolBar = addToolBar((tr("exitToolBar")));
  exitToolBar->addAction(exitAction);
  
}

void MainWindow::restoreContextItemsButtonGroupToSelectionMode()
{
  contextItemsButtonGroup->button(int(RbeVisualizeWidget_GraphicsScene::ITEM_SELECTION_MODE))->setChecked(true);
}

void MainWindow::contextItemsButtonGroupClicked()
{
  rbeVisualizeWidget->getDrawScene()->setMode(RbeVisualizeWidget_GraphicsScene::Mode(contextItemsButtonGroup->checkedId()));      
}


void MainWindow::saveFile()
{
  this->saveContextsToFile();
  this->saveRulesToFile();
}

void MainWindow::saveProjectFile(bool popup)
{
  if(generalContainer->getGeneralRuleItems()->size() == 0 &&
     generalContainer->getGeneralContextItems()->size() == 0) 
    return;  
  
  QString filep = filepath + "project.rbe";
  QString fileName;
  if(popup == true)
  {
    fileName = QFileDialog::getSaveFileName(this, 
                                            tr("Save project"),
                                            tr((const char*)filep.toLatin1()),
                                            tr("*.rbe"));
  }
  else
    fileName = filep;
  
  if (fileName.isEmpty())
    return;
  
  
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Save project file"),
                         tr("Cannot save file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }
  rbeXmlHandler->writeProjectFile(&file);  
  this->saveContextsToFile();
  this->saveRulesToFile();
}




void MainWindow::saveContextsToFile()
{
  if(generalContainer->getGeneralContextItems()->size()==0)
    return;
  
  QString filep = filepath + ".temp/contexts.xml";      
  //  QString fileName = QFileDialog::getSaveFileName(this, 
  //                                                 tr("Save contexts"),
  //                                                  tr((const char*)filep.toLatin1()),
  //                                                 tr("*.xml"));
  
  QString fileName = filep;
  
  if (fileName.isEmpty())
    return;
  
  //make sure if the scene is empty, dont save the file
  if(rbeVisualizeWidget->getDrawScene()->items().size() == 0)
    return;
  
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Save contex file"),
                         tr("Cannot save file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }
  rbeXmlHandler->writeContextDataToFile(&file);
  
  this->saveContextsPixmapToFile();
}

void MainWindow::saveRulesToFile()
{
  if(generalContainer->getGeneralRuleItems()->size() == 0)
  {
    return;
  }
  
  QString filep = filepath + ".temp/rules.xml";      
  //  QString fileName = QFileDialog::getSaveFileName(this, 
  //                                                 tr("Save rule"),
  //                                                 tr((const char*)filep.toLatin1()),
  //                                                 tr("*.xml"));
  
  QString fileName = filep;
  
  if (fileName.isEmpty())
    return;
  
  
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Save rule file"),
                         tr("Cannot save file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }
  rbeXmlHandler->writeRuleDataToFile(&file);  
  
}

void MainWindow::saveContextsPixmapToFile()
{
  
  QString fileName;
  bool quick = true;
  
  QString filep = filepath + ".temp/mask.png";  
  
  if(quick == true)
  {
    fileName = filep;
  }
  
  else
  {
    fileName = QFileDialog::getSaveFileName(this, 
                                            tr("Save contexts image"),
                                            tr((const char*)filep.toLatin1()),
                                            tr("*.png"));
  }
  
  if (fileName.isEmpty())
    return;
  
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Save mask image"),
                         tr("Cannot save file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }
  
  QImage image(388,288,QImage::Format_ARGB32_Premultiplied);
  QRect sRect(0,0,388,288);
  QPainter painter(&image);
  rbeVisualizeWidget->getDrawScene()->render(&painter);  
  
  //traslate pixel
  QRgb pixColor;  
  
  //set a mask image, keep the area color, black the rest  
  for (int i = 0; i < image.width(); ++i)
  {
    for (int j = 0; j < image.height(); ++j)
    {
      pixColor = image.pixel(i, j);      
      bool value = false;
      for(int k = 0 ; k < generalContainer->getGeneralContextItems()->size(); k++)
      {
        if(rbeVisualizeWidget->getDrawScene()->items()[k]->type() == RbeVisualizeWidget_GraphicsScene::POLYGON_ITEM)
        {
          RbeVisualizeWidget_PolygonItem *item = qgraphicsitem_cast<RbeVisualizeWidget_PolygonItem*>(rbeVisualizeWidget->getDrawScene()->items()[k]);
          if(QColor::fromRgb(pixColor) == item->getColor())   
          {
            value = true;
          }
        }        
      }
      if(value == false)
      {
        image.setPixel(i, j, qRgb(0,0,0));//set to black
      }
    }
  }   
  image.save(fileName,"PNG");    
}

void MainWindow::openFile(bool popup)
{
  QString fileName;
  if(popup == true)
  {
    fileName = QFileDialog::getOpenFileName(this, 
                                                    tr("Open file"),
                                                    tr((const char*)filepath.toLatin1()),
                                                    tr("*.rbe"));
  }
  else
    fileName = filepath+"project.rbe";
  
  if (fileName.isEmpty())
    return;  
  
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(this, tr("Open file"),
                         tr("Cannot open file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }  
  
  rbeXmlHandler->read(&file);
}



void MainWindow::openVideoFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, 
                                                  tr("Open video file"),
                                                  tr((const char*)filepath.toLatin1()),
                                                  tr("*.avi"));
  
  if (fileName.isEmpty())
    return;  
  
  
  QVideoDecoder decoder;  
  decoder.openFile(fileName);
  if(decoder.isOk()==false)
  {
    QMessageBox::critical(this,"Error","Error loading the video");
    return;
  }
  
  if(!decoder.seekNextFrame())
  {
    QMessageBox::critical(this,"Error","seekNextFrame failed");
  }
  
  
  QImage img;
  // Decode a frame
  int et,en;
  if(!decoder.getFrame(img,&en,&et))
  {
    QMessageBox::critical(this,"Error","Error decoding the frame");
    return;
  }
  
  decoder.close();
  
  //now display image
  // Convert the QImage to a QPixmap for display
  QPixmap pixmap;  
  pixmap = QPixmap(img.size());
  QPainter painter;
  painter.begin(&pixmap);
  painter.drawImage(0,0,img);
  painter.end();   
  
  //this->rbeVisualizeWidget->getDrawScene()->addPixmap(pixmap);
  this->rbeVisualizeWidget->getDrawScene()->setBackgroundBrush(QBrush(QPixmap(pixmap)));
  
  videoFilePath = fileName;
}

void MainWindow::newFile()
{
  this->rbeItemTree->clear();
  this->rbePropertyTree->clear();
  this->rbeVisualizeWidget->getDrawScene()->clear();    
  this->rbeVisualizeWidget->getDrawScene()->setBackgroundBrush(QBrush(QColor(Qt::white)));
  
  //remove all items
  generalContainer->clearAllItems();
  
  //clear filepath
  videoFilePath = "";      
}

void MainWindow::newRule()
{
  generalContainer->update(RbeGeneralContainer::ADD_NEW_RULE);
}

void MainWindow::exit()
{
  this->close();
}

void MainWindow::about()
{
  QMessageBox box;
  box.setText("Rule-based engine Editor");
  box.exec();
  return;
}

void MainWindow::runAnimate()
{
  //  rbeAnimationDialog = new RbeAnimationDialog();    
  //  rbeAnimationDialog->show(); 
  //  rbeAnimationDialog->start();
}


void MainWindow::runVirtualFencing()
{
  if(generalContainer->getGeneralContextItems()->size() == 0)
  {
    QMessageBox box;
    box.setText("Please add contexts");
    box.exec();
    return;
  }
  
  if(generalContainer->getGeneralRuleItems()->size() == 0)
  {
    QMessageBox box;
    box.setText("Please add rules");
    box.exec();
    return;
  }
  
  if(videoFilePath == "")
  {
    this->openVideoFile();
  } 
  //BUGS.txt -> 2
  
  //save project to temp folder and run
  this->saveProjectFile(false);
  this->openFile(false); 
  
  generalContainer->clearRuleTree();
  
  RbeVirtualFence *dialog = new RbeVirtualFence(this);  
  dialog->mainW = this;
  
  dialog->videoFilePath = this->videoFilePath; 
  dialog->run();
}


MainWindow::~MainWindow()
{
  
}



