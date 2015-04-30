#include "Rule.hpp"

#include "Context.hpp"
#include "Object.hpp"
#include "EventContainer.hpp"
#include "Event.hpp"

using namespace Rbe;

Rule::Rule()
{
  mEventContainer = NULL;
}

Rule::Rule(int id, std::string name, std::string desc)
{
  mEventContainer = NULL;
  mId = id;
  mName = name;    
  mDesc = desc;
}

void Rule::setEventContainer(EventContainer *eC)
{
  mEventContainer = eC;
  mEventContainer->linkToMama = NULL;
}

EventContainer* Rule::getEventContainer()
{
  return mEventContainer;
} 

void Rule::process(std::vector<Context *> contexts, std::vector<Object*> objects)
{   
  mEventContainer->process(contexts,objects);  
}

void Rule::cleanEventResultQueue()
{
  mEventContainer->cleanResultQueue();
}

Rule::~Rule()
{
  if(mEventContainer == NULL) delete mEventContainer;
}


