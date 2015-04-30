/** \file
  * The Action class file
  * 
  * $Id$
  */


#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <iostream>
#include <assert.h>

#include <QSound>
#include <QString>
#include <QApplication>
#include <QDebug>

namespace Rbe
{

/**
  * Action class, define the behavior for different Action type.    
  */
class Action
{    
public:  
  
  /**
    * Enumerator for different action types.
    */
  enum ActionType
  {
    PRINT = 10, ///< Print action.
    ALARM,      ///< Alram action.
    DISABLE     ///< Default type, do nothing.
  };
  
  /**
    * Constructor with default type value.
    */  
  Action(ActionType type = DISABLE);       
  
  /**
    * Constructor
    *
    * \param[in] type a string that match ActionType enum
    */
  Action(std::string type);       
  
  /**
    * Destructor
    */
  ~Action();
  
  /**
    * set the message to be printed
    */
  void setMessage(QString message);
      
  /**
    * run the action
    */
  void performAction(); 
  
  
private:    
  ActionType mType;     ///< instance of ActionType enum
  QSound *mAlarmSound;  
  QString mMessage;
};

}

#endif // ACTION_HPP
