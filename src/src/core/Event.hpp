#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>
#include <string>
#include <iostream>
#include <assert.h>
#include <QRgb>
#include <QColor>

namespace Rbe
{

class Action;
class Context;
class Object;
class EventContainer;
class EventFilter;

class Event
{
public:
  
  /**
    * Enum types of Event.
    */
  enum EventType
  {
    ENTER_AREA = 50, 
    LEAVE_AREA,
    APPEAR_AREA,
    DISAPPEAR_AREA, 
    CROSSING_TRIPWIRE,
    CROSSING_TRIPWIRE_LEFT2RIGHT,
    CROSSING_TRIPWIRE_RIGHT2LEFT,
    HIGH_SPEED, 
    DISABLE
  };
  
  /**
    * Constructor.
    */
  Event();
  
  /**
    * Constructor with default value.
    */
  Event(EventType type = DISABLE);
  
  /**
    * Constructor with string to setup its event type.
    *
    * \param[int] type a string matches an enum type.
    */
  Event(std::string type);
  
  /**
    * Destructor.
    */
  ~Event();
  
  EventType getType(){return mType;}
  std::string getTypeString();
  void setLinkToContainer(EventContainer *link);      
  
  void addAction(Action *anAction);      
  void addFilter(EventFilter *filter);
  
  void doAction();
  
  void process(std::vector<Context *> contexts, std::vector<Object *> objects);  
  
  bool detectAreaEvent_Enter(std::vector<Context *> contexts, std::vector<Object *> objects);  
  void detectAreaEvent_Leave(std::vector<Context *> contexts, std::vector<Object *> objects);  
  void detectTripwireEvent_Crossing(std::vector<Context *> contexts, std::vector<Object *> objects, int direction = 0);  
  
  // bool detectAreaEvent_Appear(std::vector<Context *> contexts, std::vector<Object*> objects);
  // bool detectAreaEvent_Disappear(std::vector<Context *> contexts, std::vector<Object*> objects);        
  
  void setupFilter();
  
private:     
  
  EventContainer *mLinkToContainer;
  EventType mType;
  std::vector<Action* > mActions;
  std::vector<EventFilter* > mFilters;  
};

}

#endif // EVENT_HPP
