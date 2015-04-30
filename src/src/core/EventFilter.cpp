#include "EventFilter.hpp"

using namespace Rbe;

EventFilter::EventFilter()
{
  
}

void EventFilter::setFilterType(std::string type)
{
  if(type == "OBJECT_ID")
    mType = OBJECT_ID;
  if(type == "CONTEXT_ID")
    mType = CONTEXT_ID;
  
}

EventFilter::~EventFilter()
{
  ;
}
