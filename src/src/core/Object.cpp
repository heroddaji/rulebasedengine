#include "Object.hpp"

#include "ObjectFrame.hpp"
#include "Misc.hpp"
#include "Event.hpp"
#include "EventContainer.hpp"

using namespace Rbe;

Object::Object()
{
  mCurrentObjectFrame = new ObjectFrame();
  mCloneResultOfEventContainer = -1;
}

void Object::addObjectFrame(ObjectFrame *f)
{
  mObjectFrames.push_back(f);
}


void Object::addTrajectory(Point &p)
{
  mTrajectory.push_back(p);
}

std::vector<ObjectFrame* > Object::getObjectFrames()
{
  return mObjectFrames;
}

std::vector<Point> Object::getTrajectory()
{
  return mTrajectory;
}

ObjectFrame* Object::getCurrentObjectFrame()
{
  return mCurrentObjectFrame;
}

void Object::setCurrentObjectFrame(ObjectFrame* f)
{ 
  mCurrentObjectFrame = f;
}

void Object::addEventQueue(EventQueue &queue)
{
  //set a lenght for the queue, now is 10
  int length = 5;
  mEventQueues.push_back(queue);
  if(mEventQueues.size() > length)
    mEventQueues.erase(mEventQueues.begin());
}

std::string Object::eventTypeToString(int type)
{
  if(type == Event::ENTER_AREA)
    return "ENTER_AREA";
  if(type == Event::LEAVE_AREA)
    return "LEAVE_AREA";
  if(type == Event::CROSSING_TRIPWIRE)
    return "CROSSING_TRIPWIRE";
  if(type == Event::APPEAR_AREA)
    return "APPEAR_AREA";
  if(type == Event::DISAPPEAR_AREA)
    return "DISAPPEAR_AREA";
  if(type == EventContainer::AND)
    return "AND";
  if(type == EventContainer::OR)
    return "OR";
  if(type == EventContainer::SEQUENCE)
    return "SEQUENCE";
  if(type == EventContainer::ONE_EVENT)
    return "ONE_EVENT";
  return "";
}

void Object::clearEventQueue()
{
  
  if(!mAnEventQueue.empty())  
    mAnEventQueue.erase(mAnEventQueue.begin(),mAnEventQueue.end());      
  
  mAnEventQueue.clear();
  
  if(!mEventQueues.empty())  
    mEventQueues.erase(mEventQueues.begin(),mEventQueues.end());      
  
  mEventQueues.clear();
  
}

void Object::clearObjectFrames()
{
  if(!mObjectFrames.empty())
  {
    for(int i=0; i< mObjectFrames.size(); i++)
      delete mObjectFrames[i];
    
    mObjectFrames.clear();
  } 
}

void Object::clearTrajectory()
{
  if(!mTrajectory.empty())  
    mTrajectory.erase(mTrajectory.begin(),mTrajectory.end());      
  
  mTrajectory.clear();
}

Object::~Object()
{
  delete mCurrentObjectFrame;
  this->clearObjectFrames();
  this->clearTrajectory();
}
