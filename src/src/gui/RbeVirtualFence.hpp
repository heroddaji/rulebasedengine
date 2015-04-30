#ifndef RBEVIRTUALFENCE_HPP
#define RBEVIRTUALFENCE_HPP

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include "MainWindow.hpp"

namespace Rbe{
    class Engine;
    class Rule;
    class Event;
    class EventContainer;
    class Object;
}
class RbeVirtualFence : public QDialog
{
    Q_OBJECT
  friend class MainWindow;
public:
    explicit RbeVirtualFence(QWidget *parent = 0);
    
  int run();
  
  MainWindow *mainW;
  Rbe::Engine *engine;
  QString videoFilePath;
  QString tempContextPath;
  QString tempRulePath;
  
  void loadSub(int &oID,int &cID, Rbe::Event *event, Rbe::EventContainer *c, Rbe::Object *);
signals:

public slots:

};

#endif // RBEVIRTUALFENCE_HPP
