#ifndef EVENTCONTAINER_HPP
#define EVENTCONTAINER_HPP

#include <vector>
#include <string>
#include <assert.h>
#include <time.h>
#include <stdio.h>


namespace Rbe
{
struct QueueStruct;
class Event;
class Action;
class Context;
class Object;

class EventContainer
{
public:
  enum ContainerType{AND = 70, OR, SEQUENCE, ONE_EVENT, NO_CONTAINER_TYPE};
  
  EventContainer();
  EventContainer(ContainerType type = NO_CONTAINER_TYPE);
  EventContainer(std::string type);
  ~EventContainer();
  
  ContainerType getType(){return mType;}
  
  void addContainer(EventContainer *container);
  void addEvent(Event *event);
  void addAction(Action *anAction);      
  
  void process(std::vector<Context *> contexts, std::vector<Object *>objects);      
  void processSubContainers(std::vector<Context *> contexts, std::vector<Object *>objects);      
  void processEvents(std::vector<Context *> contexts, std::vector<Object *>objects);        
  
  std::vector<QueueStruct>mResultQueue;
  void pushResultToQueue(QueueStruct &qT);
  bool processResultQueue();
  void cleanResultQueue();
    
  double mSecond;
  clock_t mEndClock;
    
  EventContainer *linkToMama;
  std::vector<EventContainer *> mContainers;
private:
  
  int order;
  std::vector<clock_t> clocks;
  ContainerType mType;    
  
  std::vector<Event *> mEvents;
  std::vector<Action *> mActions;  
  void doAction();
};

}
#endif // EVENTCONTAINER_HPP
