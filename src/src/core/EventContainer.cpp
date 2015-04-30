#include "EventContainer.hpp"

#include "Event.hpp"
#include "Action.hpp"
#include "ContextArea.hpp"
#include "ContextTripwire.hpp"
#include "Object.hpp"
#include "Misc.hpp"

using namespace Rbe;


EventContainer::EventContainer()
{
  mType = NO_CONTAINER_TYPE;        
  order = 0;
}

EventContainer::EventContainer(ContainerType type)
{
  mType = type;  
  order = 0;
}

EventContainer::EventContainer(std::string type)
{
  //AND = 70, OR, SEQUENCE, NO_CONTAINER_TYPE};
  if(type == "AND") mType = AND;
  else if(type == "OR") mType = OR;
  else if(type == "SEQUENCE") mType = SEQUENCE;
  else if(type == "ONE_EVENT") mType = ONE_EVENT;
  else if(type == "NO_CONTAINER_TYPE") mType = NO_CONTAINER_TYPE;
  else assert(false);
  order = 0;
}

void EventContainer::addContainer(EventContainer *container)
{
  mContainers.push_back(container);
  container->linkToMama = this;
}

void EventContainer::addEvent(Event *event)
{
  mEvents.push_back(event);
  event->setLinkToContainer(this);
}

void EventContainer::addAction(Action *anAction)
{
  mActions.push_back(anAction);
}

void EventContainer::pushResultToQueue(QueueStruct &value)
{ 
  mResultQueue.push_back(value);
  
  if(linkToMama == NULL)
    int a = 0;
  
  QueueStruct queueData;
  if(linkToMama != NULL)
  {    
    queueData.dataType = CONTAINER_QUEUE;
    queueData.eventSource = NULL; 
    
  }
  
  if(processResultQueue() == true)
  {
    if(linkToMama != NULL)
    { 
      queueData.result = true;
    }
    
    this->doAction();    
  }
  else
  {
    if(linkToMama != NULL)
      queueData.result = false;
  }
  
  if(linkToMama != NULL)
  {
    linkToMama->pushResultToQueue(queueData);
  }
}

bool EventContainer::processResultQueue()
{
  bool value = false;
  if(mResultQueue.size() == mContainers.size() + mEvents.size())
  {
    if(mType == EventContainer::ONE_EVENT)
    {
      value = mResultQueue[0].result;      
    }
    
    if(mType == EventContainer::AND)
    { 
      value = mResultQueue[0].result;
      for(int i = 0; i < mResultQueue.size(); i++)
        value = value && mResultQueue[i].result;
    }
    
    if(mType == EventContainer::OR)
    { 
      value = mResultQueue[0].result;
      
      //OR calculate
      int count=0;
      for(int i = 0; i < mResultQueue.size(); i++)
      {
        value = value || mResultQueue[i].result;
        
        //make sure all false OR not result in true ( 0 || 0 = 1 )  
        if(mResultQueue[i].result == false)
          count++;
      }
      
      if(count == mResultQueue.size())
        value = false;      
    }
    
    if(mType == EventContainer::SEQUENCE)
    {               
      if(mResultQueue[order].result == true)
      {
        clocks.push_back(clock());
        
        if(order < mResultQueue.size())
          ++order;
        else if (order == mResultQueue.size())
          order = 0;
      }
      
      if(clocks.size() == mContainers.size() + mEvents.size())
      {
        if( (clocks[clocks.size() - 1] - clocks[0]) <= (mSecond * CLOCKS_PER_SEC))
        {
          value = true;         
        }
        clocks.erase(clocks.begin(),clocks.end());
        order = 0;
      }
      
    }
  }
  
  else
    value = false;
  
  return value;
}

void EventContainer::cleanResultQueue()
{
  //clean
  for(int i = 0 ; i < mContainers.size(); i++)
  {
    mContainers[i]->cleanResultQueue();
  }
     
  mResultQueue.erase(mResultQueue.begin(),mResultQueue.end());
  
}

void EventContainer::process(std::vector<Context *> contexts, std::vector<Object *> objects)
{
  this->processSubContainers(contexts,objects);
  this->processEvents(contexts,objects); 
}

void EventContainer::processSubContainers(std::vector<Context *> contexts, std::vector<Object *>objects)
{
  for(int i = 0 ; i < mContainers.size(); i++)
  {       
    EventContainer *container = mContainers[i];
    container->processSubContainers(contexts,objects);    
    container->processEvents(contexts,objects); 
  }  
}

void EventContainer::processEvents(std::vector<Context *> contexts, std::vector<Object *> objects)
{ 
  for(int i = 0 ; i < mEvents.size(); i++)
  {       
    Event *event = mEvents[i];
    event->process(contexts,objects);
  }  
}


void EventContainer::doAction()
{  
  for(int i = 0 ; i < mActions.size(); i++)
  {
    Action *action = mActions[i];
    action->performAction();
  }  
}

EventContainer::~EventContainer()
{
  mContainers.erase(mContainers.begin(),mContainers.end());
  mEvents.erase(mEvents.begin(),mEvents.end());
  mActions.erase(mActions.begin(),mActions.end());
}
