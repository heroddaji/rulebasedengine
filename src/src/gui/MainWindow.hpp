#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtGui>
#include <QGraphicsItem>
#include <QString>
#include <QThread>

class RbeAnimationDialog;
class RbeItemTree;    
class RbePropertyTree;
class RbeVisualizeWidget;
class RbeXmlHandler;
class RbeGeneralContainer;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();   
        
    void restoreContextItemsButtonGroupToSelectionMode();
      RbeGeneralContainer *generalContainer;
private:            
    QHBoxLayout *layout;
    QWidget *centralWidget;
    
    RbeItemTree *rbeItemTree;  
    RbePropertyTree  *rbePropertyTree;
    RbeAnimationDialog *rbeAnimationDialog;    
    RbeVisualizeWidget *rbeVisualizeWidget;    
    RbeXmlHandler *rbeXmlHandler;    
  
    
    QMenu *fileMenu;    
    QMenu *contextMenu;    
    QMenu *ruleMenu; 
    QMenu *aboutMenu;
    
    QAction *newAction;    
    QAction *openFileAction;     
    QAction *openVideoAction; 
    QAction *saveAction;    
    QAction *saveContextsAction;    
    QAction *saveContextsPixmapAction;    
    QAction *saveRulesAction;
    QAction *saveProjectAction;
    QAction *exitAction;    
    QAction *aboutAction;    
    QAction *newRuleAction;
    QAction *runAnimateAction;    
    QAction *runVirtualFencingAction;
    QAction *addLineAction;
    
    QButtonGroup *contextItemsButtonGroup;
    
    QString videoFilePath;
    
    void checkAndGenerateNecessaryFile();
    void createActions();
    void createMenus();
    void createToolbars();  
    void createSubViewsAndLayoutAndConnection();    
    
signals:
    
private slots:  
   
    void saveFile();
    void saveProjectFile(bool popup=true);   
    void saveContextsToFile();
    void saveRulesToFile();
    void saveContextsPixmapToFile();
    void openFile(bool popup=true);        
    void openVideoFile();
    void newFile();
    void newRule();
    void exit();
    void about();
    
    void contextItemsButtonGroupClicked();
    
    void runAnimate();
    void runVirtualFencing();
    
    
    
};

#endif // MAINWINDOW_HPP
