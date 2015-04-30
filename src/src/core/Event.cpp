#include "Event.hpp"

#include "Action.hpp"
#include "Context.hpp"
#include "ContextArea.hpp"
#include "ContextTripwire.hpp"
#include "Object.hpp"
#include "ObjectFrame.hpp"
#include "Misc.hpp"
#include "EventContainer.hpp"
#include "EventFilter.hpp"

using namespace Rbe;


Event::Event()
{    
  mType = DISABLE;
}

Event::Event(EventType type )
{
  mType = type;
}

Event::Event(std::string type)
{  
  if(type == "ENTER_AREA") mType = ENTER_AREA;
  else if(type == "LEAVE_AREA") mType = LEAVE_AREA;  
  else if(type == "CROSSING_TRIPWIRE") mType = CROSSING_TRIPWIRE;
  else if(type == "CROSSING_TRIPWIRE_LEFT2RIGHT") mType = CROSSING_TRIPWIRE_LEFT2RIGHT;
  else if(type == "CROSSING_TRIPWIRE_RIGHT2LEFT") mType = CROSSING_TRIPWIRE_RIGHT2LEFT;  
  else if(type == "DISABLE") mType =  DISABLE;  
  else assert(false);
}

std::string Event::getTypeString()
{
  if(mType == ENTER_AREA) return "ENTER_AREA";
  if(mType == LEAVE_AREA) return "LEAVE_AREA";
  if(mType == CROSSING_TRIPWIRE) return "CROSSING_TRIPWIRE";
  if(mType == CROSSING_TRIPWIRE_LEFT2RIGHT) return "CROSSING_TRIPWIRE_LEFT2RIGHT";
  if(mType == CROSSING_TRIPWIRE_RIGHT2LEFT) return "CROSSING_TRIPWIRE_RIGHT2LEFT";
  return "";
}

void Event::setLinkToContainer(EventContainer *link)
{
  mLinkToContainer = link;
}

void Event::addAction(Action *anAction)
{
  mActions.push_back(anAction);
}

void Event::addFilter(EventFilter *filter)
{
  mFilters.push_back(filter);
}

void Event::process(std::vector<Context *> contexts, std::vector<Object *> objects)
{
  if(mType == Event::ENTER_AREA)
    this->detectAreaEvent_Enter(contexts,objects);
  
  if(mType == Event::LEAVE_AREA)
    this->detectAreaEvent_Leave(contexts,objects);
  
  if(mType == Event::CROSSING_TRIPWIRE)
    this->detectTripwireEvent_Crossing(contexts,objects);
  
  if(mType == Event::CROSSING_TRIPWIRE_LEFT2RIGHT)
    this->detectTripwireEvent_Crossing(contexts,objects,1);
  
  if(mType == Event::CROSSING_TRIPWIRE_RIGHT2LEFT)
    this->detectTripwireEvent_Crossing(contexts,objects,2);
}

bool Event::detectAreaEvent_Enter(std::vector<Context *> contexts, std::vector<Object *> objects)
{
  QueueStruct queueData;
  queueData.dataType = EVENT_QUEUE;
  queueData.eventSource = this;      
  
  std::vector<bool> resultFilters;
  
  for(int a = 0 ; a < objects.size(); a++)
  { 
    Object *object = objects[a];
    
    for(int b = 0 ; b < contexts.size(); b++)
    {    
      ContextArea *area = static_cast<ContextArea *>(contexts[b]);
      
      //each object go though each context  
      if(contexts[b]->getType() != Context::AREA)
      {
        continue;
      }
      
      //this image should be global!!
      QImage *image = area->getQImage();              
      
      ObjectFrame *lastObjectFrame = object->getCurrentObjectFrame();
      
      QRgb pixColor;
      pixColor = image->pixel(lastObjectFrame->getXCenter(),lastObjectFrame->getYCenter());            
      
      QRgb objectColor = area->getRgbColorValue();
      
      // Object enter area when it is moving (so the trajectory length should be more than a minimum length)               
      int minimumLenth = 3;
      
      if(pixColor == objectColor && object->getTrajectory().size() > minimumLenth)
      {                 
        queueData.objectsID.push_back(object->getId());        
        resultFilters.push_back(true);
      }
      else
      {                 
        resultFilters.push_back(false);
      }           
    }
  }  
  
  //check the result filter
  queueData.result = false;  
  for(int i = 0 ; i < resultFilters.size(); i++)
  {
    if(resultFilters[i] == true)
    {
      queueData.result = true;
      this->doAction();
      break;
    }
  }  
  
  if(this->mType == Event::ENTER_AREA)          
    mLinkToContainer->pushResultToQueue(queueData);  
  
  return queueData.result;
}

void Event::detectAreaEvent_Leave(std::vector<Context *> contexts, std::vector<Object *> objects)
{ 
  //detect enter_area event    
  bool value = false;  
  QueueStruct queueData;
  queueData.dataType = EVENT_QUEUE;
  queueData.eventSource = this;      
  std::vector<bool> resultFilters;
  static int area_id = -1;
  static int frameCount = 0;
  int limit =10;
  
  for(int a = 0 ; a < objects.size(); a++)
  { 
    Object *object = objects[a];
    
    for(int b = 0 ; b < contexts.size(); b++)
    {    
      ContextArea *area = static_cast<ContextArea *>(contexts[b]);
      
      //each object go though each context  
      if(contexts[b]->getType() != Context::AREA)
      {
        continue;
      }
      
      //this image should be global!!
      QImage *image = area->getQImage();              
      
      ObjectFrame *lastObjectFrame = object->getCurrentObjectFrame();
      
      QRgb pixColor;
      pixColor = image->pixel(lastObjectFrame->getXCenter(),lastObjectFrame->getYCenter());            
      
      QRgb objectColor = area->getRgbColorValue();
      
      // Object enter area when it is moving (so the trajectory length should be more than a minimum length)               
      int minimumLenth = 3;
      
      if(pixColor == objectColor && object->getTrajectory().size() > minimumLenth)
      { 
        area_id = area->getID();
      }
      
      if(area_id != -1)
      {  
        if(area->getID() == area_id)
        {
          if(pixColor != objectColor && object->getTrajectory().size() > minimumLenth)
          { 
            if(frameCount < limit)
            {
              queueData.objectsID.push_back(object->getId());        
              resultFilters.push_back(true);
              ++frameCount;
            }             
          }
          else
          {                 
            resultFilters.push_back(false);
            frameCount = 0;
            //area_id = -1;
          }
        }
      }
    }  
  }
  
  //check the result filter
  queueData.result = false;    
  for(int i = 0 ; i < resultFilters.size(); i++)
  {
    if(resultFilters[i] == true)
    {
      queueData.result = true;
      this->doAction();
      break;
    }
  }
  
  mLinkToContainer->pushResultToQueue(queueData);    
}


void Event::detectTripwireEvent_Crossing(std::vector<Context *> contexts, std::vector<Object *> objects, int direction)
{
  //direction
  // 0: no direction
  // 1: left2right
  // 2: right2left
  
  bool value = false;
  QueueStruct queueData;
  queueData.dataType = EVENT_QUEUE;
  queueData.eventSource = this;    
  
  std::vector<bool> resultFilters;
  
  for(int a = 0 ; a < objects.size(); a++)
  {
    Object *object = objects[a];
    
    for(int b = 0 ; b < contexts.size(); b++)
    {
      Context *aC = contexts[b];
      if(aC->getType() != Context::TRIPWIRE)
      {
        continue;
      }
      
      ContextTripwire *tripwire =  static_cast<ContextTripwire *>(contexts[b]);            
      
      
      //only detect if the number of trajectory of object is bigger than a value        
      unsigned numberOfFrame = 5;
      if(object->getTrajectory().size() >= numberOfFrame)
      {        
        ObjectFrame *aOF = object->getCurrentObjectFrame();      
        
        value = true;
        //compute tripwire line equation
        Line *aLine = tripwire->getLine();
        Point tripwirePoint1 = aLine->point1;
        Point tripwirePoint2 = aLine->point2;
        assert((tripwirePoint2.x - tripwirePoint1.x) != 0);
        float tripwireSlope = ((float)(tripwirePoint2.y - tripwirePoint1.y))/ ((float)(tripwirePoint2.x - tripwirePoint1.x));
        float tripwireYIntersect = (float)(tripwirePoint1.y - tripwireSlope*tripwirePoint1.x);
        
        //get last point in the vector, not the first point
        Point objectPoint1 = object->getTrajectory()[object->getTrajectory().size()-1];            
        Point objectPoint2;
        
        int trajectoryLength = 5;
        if(object->getTrajectory().size() > trajectoryLength)
        {
          objectPoint2 = object->getTrajectory()[object->getTrajectory().size()-trajectoryLength];
        }
        else
          objectPoint2 = object->getTrajectory()[0];
        
        float objectSlope;
        if((objectPoint2.x - objectPoint1.x) == 0)
        {
          objectSlope = 0.00001;
        }
        else
        {
          objectSlope = ((float)(objectPoint2.y - objectPoint1.y)) / ((float)(objectPoint2.x - objectPoint1.x));
        }            
        
        float objectYIntersect = (float)(objectPoint1.y - objectSlope*objectPoint1.x);
        
        //calculate the cross point
        float x_cross = (objectYIntersect - tripwireYIntersect)/(tripwireSlope - objectSlope);
        float y_cross = tripwireSlope*x_cross + tripwireYIntersect;
        
        //validate the cross point if it inside objectline or tripwireline
        // is the x-cross coordinate in the object line ?
        if (objectPoint2.x > objectPoint1.x)
        {
          if (x_cross > objectPoint2.x || x_cross < objectPoint1.x)
          {
            value = false;
          }
        }
        else
        {
          if (x_cross > objectPoint1.x || x_cross < objectPoint2.x)
          {
            value = false;
          }            
        }
        
        // is the y-cross coordinate in the object line
        if (objectPoint2.y > objectPoint1.y)
        {
          if (y_cross > objectPoint2.y || y_cross < objectPoint1.y)
          {
            value = false;
          }
        }
        else
        {
          if (y_cross > objectPoint1.y || y_cross < objectPoint2.y)
          {
            value = false;
          }
        }
        
        // is the x-cross coordinate in the trip wire
        if (tripwirePoint1.x > tripwirePoint2.x)
        {
          if (x_cross > tripwirePoint1.x || x_cross < tripwirePoint2.x)
          {
            value = false;
          }
        }
        else
        {
          if (x_cross > tripwirePoint2.x || x_cross < tripwirePoint1.x)
          {
            value = false;
          }
        } 
        
        // is the y-cross coordinate in the trip wire
        if (tripwirePoint1.y > tripwirePoint2.y)
        {
          if (y_cross > tripwirePoint1.y || y_cross < tripwirePoint2.y)
          {
            value = false;
          }
        }
        else
        {
          if (y_cross > tripwirePoint2.y || y_cross < tripwirePoint1.y)
          {
            value = false;
          }
        }                    
        
        if(value == true)
        {      
          queueData.objectsID.push_back(object->getId());          
          queueData.contextID = tripwire->getID();
          
          //now check if direction is given
          if(direction == 0)
            value = true;
          if(direction == 1) //left2right
          {            
            if(objectPoint1.x > objectPoint2.x)
              value = true;
            else
              value = false;
          }
          if(direction == 2)//right2left
          {
            if(objectPoint1.x < objectPoint2.x)
              value = true;
            else
              value = false;
                  
          }
          
          resultFilters.push_back(value);
        }      
        else      
        {          
          resultFilters.push_back(false);
          
        }      
      }  
    }
  }
  
  //check the result filter  
  queueData.result = false;
  for(int i = 0 ; i < resultFilters.size(); i++)
  {
    if(resultFilters[i] == true)
    {
      queueData.result = true;
      this->doAction();
      break;
    }
  }
  
  mLinkToContainer->pushResultToQueue(queueData);         
}

void Event::doAction()
{
  for(int i = 0 ; i < mActions.size(); i++)
  {
    Action *action = mActions[i];
    action->performAction();
  }
}

Event::~Event()
{
  //todo: release all resources
  mActions.erase(mActions.begin(),mActions.end());
  mFilters.erase(mFilters.begin(),mFilters.end());
}
