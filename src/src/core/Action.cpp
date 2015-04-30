#include "Action.hpp"

using namespace Rbe;

Action::Action(ActionType type)
{
  mType = type;
  mAlarmSound = new QSound("./data/.temp/sound/alarm.wav");
  mAlarmSound->play();
}

Action::Action(std::string type)
{
  if(type == "PRINT") mType = PRINT;
  else if(type == "ALARM") mType = ALARM;
  else if(type == "DISABLE") mType = DISABLE;
  else assert(false);
  
  QString path = QApplication::applicationDirPath() + "/data/.temp/sound/alarm.wav";
  ///home/dai/Dropbox/Graduation/rulebasedengine/build/data/.temp/sound/alarm.wav
  mAlarmSound = new QSound(path );
  bool playable = mAlarmSound->isAvailable();
  //mAlarmSound->play();
}


void Action::performAction()
{
  static int a = 0;
  
  if(mType == PRINT) 
  {
    std::cout << mMessage.toStdString() << "  " <<a <<"\n";
    
    //qDebug() << mMessage << "" << a;;
  }  
  
  if(mType == ALARM) 
  {
   // mAlarmSound->play(); not functional
    std::cout << mMessage.toStdString() <<  "  "  << a <<"\n";
    
   // qDebug() << mMessage << a;;
  }  
  a++;
}

void Action::setMessage(QString message)
{
  mMessage = message;
}

Action::~Action()
{
  delete mAlarmSound;
}
