/** \file
  * some struct types.
  * 
  * $Id$
  */

#ifndef MISC_RBE_HPP
#define MISC_RBE_HPP

#include <vector>
#include <time.h>

namespace Rbe
{ 
  /**
    * 2d point struct
    */
  struct Point
  {
    int x;
    int y;  
  };
  
  /**
    * Line struct. Each line has 2 points. 
    */
  struct Line
  {
    Point point1;
    Point point2;
  };
  
  
  enum QueuDataType
  {
    CONTAINER_QUEUE = 5463,
    EVENT_QUEUE
  };
  
  class Event;
  class Object;
  
  struct QueueStruct
  {
    bool result;    
    int dataType;
    int contextID;
    clock_t clock;
    Event *eventSource;
    std::vector<int> objectsID;
  };
}

#endif

