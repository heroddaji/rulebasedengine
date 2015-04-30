/** \file
* The Object class
* $Id: Object.hpp 30 2010-12-01 15:27:51Z dai $
*/

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <map>
#include <string>

namespace Rbe
{    
  typedef std::map<int,int> EventQueue;
  class ObjectFrame;
  struct Point;
  class Object
  {    
  public:
    
    enum PostionState{ INSIDE_AREA = 410, OUTSIDE_AREA, ON_TRIPWIRE };
    
    Object();     
    ~Object();
    
    int getId(){return mId;}
    void setId(int id){mId = id;}        
    
    ObjectFrame *getCurrentObjectFrame();
    void setCurrentObjectFrame(ObjectFrame* f);
    void addObjectFrame(ObjectFrame *f);   
    std::vector<ObjectFrame* > getObjectFrames();
    
    void addTrajectory(Point &p);    
    std::vector<Point> getTrajectory();
       
    void addEventQueue(EventQueue &queue);
    EventQueue mAnEventQueue;
    std::vector<EventQueue>mEventQueues;
    
    void clearObjectFrames();
    void clearTrajectory();
    void clearEventQueue();
    
    std::string eventTypeToString(int type);
    int mCloneResultOfEventContainer;
  private:
    int mId; 
    ObjectFrame *mCurrentObjectFrame;
    std::vector<ObjectFrame *> mObjectFrames;
    std::vector<Point> mTrajectory;    
        
  };
}

#endif // OBJECT_HPP
