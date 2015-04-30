/** \file
* The Engine class. The master of all kind.
*
* $Id$
*/

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <exception>

#include <libxml/xmlreader.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <QString>

class TrackedObjectVirtualFencing;

namespace Rbe
{    
class Context;
class Rule;
class Object;    
class EventContainer;
class EventFilter;
class Event;  
struct Point;
struct Line;
class Action;
class ObjectFrame;

/**
    * Engine class, responsile for loading data from xml file and process rules
    * from events and actions
    */
class Engine
{    
  friend class Object;
  
public:  
/**
  * Constructor.
  */
  Engine();
  
/**
  * Destructor.
  */
  ~Engine();
 
/**
  * Check if an object is in the mContexts vector.
  */  
  bool isObjectExist(int id);
  
/**
  * take object data from VirtualFencing app, and tranlate to object class data
  */
  void loadObjectDataFromVirtualFence(std::vector<TrackedObjectVirtualFencing> &mTracksVFs);
  
  
  std::vector<Context *> getContexts(){return mContexts;}
  std::vector<Object *> getObjects(){return mObjects;}               
  std::vector<Rule *> getRules(){return mRules;}               
  
  void readContextFile(std::string fileName);        
  void readRuleFile(std::string fileName);        
  
  //detection           
  void processRule();    
  void cleanRuleEventResultQueue();
  void clear();
  
  std::string getMaskPath(){return maskPath;}
private:
  
  std::vector<Rule *> mRules;
  std::vector<Context *> mContexts;
  std::vector<Object *> mObjects;       
  
  std::string maskPath;
  
  bool isNewObject(int id);
  
  void loadXmlContextType(xmlNodePtr node);        
  void loadXmlPointType(Point &point, xmlNodePtr node);
  void loadXmlLineType(Line &line, xmlNodePtr node);    
  void loadXMLMaskType(std::string &mask, xmlNodePtr node);    
  
  void readObjectFile(std::string fileName);          
  void loadXmlTrajectoryType(ObjectFrame *objectFrame,xmlNodePtr node);    
  
  void loadXmlRuleType(xmlNodePtr node);   
  void loadXmlEventContainerType(xmlNodePtr node, Rule *rule, bool isSubContainer = false);        
  void loadXmlEventType(xmlNodePtr node, EventContainer *eC);            
  void loadXmlEventFilterType(xmlNodePtr node, Event *e);            
  void loadXmlActionType(xmlNodePtr node, EventContainer *eC = NULL, Event *e = NULL);       
};

}
#endif // ENGINE_HPP


