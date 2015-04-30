#ifndef RULE_HPP
#define RULE_HPP

#include <string>
#include <vector>
#include <iostream>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>

namespace Rbe
{   
  class EventContainer;
  class Context;
  class Object;
  class Rule
  {    
  public:
    Rule();
    Rule(int id, std::string name, std::string desc);
    ~Rule();    
        
    int getID(){return mId;}        
    void setID(int id){mId = id;}
    
    std::string getName(){return mName;}        
    void setName(std::string name){mName = name;}
    
    std::string getDesc(){return mDesc;}        
    void setDesc(std::string desc){mDesc = desc;}
            
    void setEventContainer(EventContainer *eC);
    EventContainer*  getEventContainer();
        
    void process(std::vector<Context *> contexts, std::vector<Object*> objects);    
    void cleanEventResultQueue();

  private:
    int mId;
    std::string mName;
    std::string mDesc;           
    EventContainer *mEventContainer;
  };
}

#endif // RULE_HPP
