#include "Engine.hpp"

#include "Action.hpp"
#include "EventFilter.hpp"
#include "Rule.hpp"
#include "Object.hpp"
#include "ObjectFrame.hpp"
#include "ContextArea.hpp"
#include "ContextTripwire.hpp"
#include "Event.hpp"
#include "EventContainer.hpp"
#include "Misc.hpp"

#include "vinotion/VirtualFencing/TrackedObjectVirtualFencing.hpp"

using namespace Rbe;

Engine::Engine()
{ 
 maskPath = "";
}


void Engine::loadXmlPointType(Point &p, xmlNodePtr node)
{   
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlPointType --> node is null");
    }
    
    p.x = atoi((const char*)xmlGetProp(node,(const xmlChar*)"x"));
    p.y = atoi((const char*)xmlGetProp(node,(const xmlChar*)"y"));        
  }
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  }
}

void Engine::loadXmlLineType(Line &line, xmlNodePtr node)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlLineType --> node is null");
    }
    
    xmlNodePtr nodeLine = node->children;
    std::vector<Point > tempPoints;        
    
    while(nodeLine != NULL)
    { 
      if(nodeLine->type == XML_ELEMENT_NODE)
      {               
        if(xmlStrcmp(nodeLine->name,(const xmlChar*)"point")==0)
        {
          Point p;
          loadXmlPointType(p,nodeLine);
          
          tempPoints.push_back(p);
        }
      }                      
      nodeLine = nodeLine->next;
    }
    
    if(tempPoints.size() == 2)
    {
      line.point1 = tempPoints[0];
      line.point2 = tempPoints[1];
    }
    else
    {
      throw std::runtime_error("Engine::loadXmlLineType --> number of points in line is not equal 2");            
    }
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  }
}

void Engine::loadXmlRuleType(xmlNodePtr node)
{
  /**
    * Load Rule from xml data
    **/
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlRuleType --> node is null");
    }
    xmlNodePtr nodeRule = node->children; 
          
    while(nodeRule != NULL)
    {         
      if(nodeRule->type == XML_ELEMENT_NODE && xmlStrcmp(nodeRule->name,(const xmlChar*)"rule")==0)
      {    
        int id = atoi((const char*)xmlGetProp(nodeRule,(const xmlChar*)"id"));        
        std::string name = (char*)xmlGetProp(nodeRule,(const xmlChar*)"name");
        std::string desc = (char*)xmlGetProp(nodeRule,(const xmlChar*)"desc");
            
        Rule *aRule = new Rule(id,name,desc);            
        
        //setup eventContainer
        this->loadXmlEventContainerType(nodeRule,aRule);    
        
        mRules.push_back(aRule);         
      }
      
      nodeRule = nodeRule->next;
    }
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << " :: " <<  typeid(e).name() <<std::endl;
    exit(-1);
  }
  
}

void Engine::loadXmlEventContainerType(xmlNodePtr node, Rule *rule,bool isSubContainer)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlEventContainerType --> node is null");
    }
    
    xmlNodePtr nodeEventContainer = node->children;    
    
    while(nodeEventContainer != NULL)
    { 
      if(nodeEventContainer->type == XML_ELEMENT_NODE && xmlStrcmp(nodeEventContainer->name,(const xmlChar*)"event_container")==0)
      { 
        //load combination type:AND,OR,SEQUENCE        
        std::string type = (char*)xmlGetProp(nodeEventContainer,(const xmlChar*)"type");
        EventContainer *eContainer = new EventContainer(type);        
        
        if(type == "SEQUENCE")
        {
          double second = atof((char*)xmlGetProp(nodeEventContainer,(const xmlChar*)"second"));
          eContainer->mSecond = second;
        }
        
        if(!isSubContainer)
          rule->setEventContainer(eContainer);
        else
        {
          EventContainer *top = rule->getEventContainer();
          this->loadXmlEventContainerType(nodeEventContainer,rule,true);
          top->addContainer(eContainer);
        }
          
        //load info inside the container        
        this->loadXmlEventContainerType(nodeEventContainer,rule,true);
        this->loadXmlEventType(nodeEventContainer,eContainer);
        this->loadXmlActionType(nodeEventContainer,eContainer,NULL);
      }                      
      nodeEventContainer = nodeEventContainer->next;
    }           
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  }
}

void Engine::loadXmlEventType(xmlNodePtr node, EventContainer *eC)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Rbe::Engine::loadXmlEventType -- node is null");
    }
    
    xmlNodePtr nodeEvent = node->children;    
    while(nodeEvent != NULL)
    { 
      if(nodeEvent->type == XML_ELEMENT_NODE && xmlStrcmp(nodeEvent->name,(const xmlChar*)"event")==0)
      { 
        std::string type = (char*)xmlGetProp(nodeEvent,(const xmlChar*)"type");
        Event *anEvent = new Event(type);        
        eC->addEvent(anEvent);
        
        //add action and filter for each event
        this->loadXmlEventFilterType(nodeEvent,anEvent);
        this->loadXmlActionType(nodeEvent,NULL,anEvent);
        
      }                      
      nodeEvent = nodeEvent->next;
    }    
  }    
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  }    
}

void Engine::loadXmlEventFilterType(xmlNodePtr node, Event *event)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlEventFilterType --> node is null");
    }
    
    xmlNodePtr nodeFilter = node->children;
    while(nodeFilter != NULL)
    { 
      if(nodeFilter->type == XML_ELEMENT_NODE && xmlStrcmp(nodeFilter->name,(const xmlChar*)"event_filter")==0)
      { 
        std::string type = (char*)xmlGetProp(nodeFilter,(const xmlChar*)"type");
        int value = atoi((const char*)xmlGetProp(nodeFilter,(const xmlChar*)"value"));
        
        EventFilter *filter = new EventFilter();
        filter->setFilterType(type);
        filter->setFilterValue(value);
        
        event->addFilter(filter);
      }                      
      nodeFilter = nodeFilter->next;
    } 
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  } 
}

void Engine::loadXmlActionType(xmlNodePtr node, EventContainer *eContainer, Event *event)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlActionType --> node is null");
    }
    
    xmlNodePtr nodeAction = node->children;
    while(nodeAction != NULL)
    { 
      if(nodeAction->type == XML_ELEMENT_NODE && xmlStrcmp(nodeAction->name,(const xmlChar*)"action")==0)
      { 
        std::string type = (char*)xmlGetProp(nodeAction,(const xmlChar*)"type");
        std::string value = (char*)xmlGetProp(nodeAction,(const xmlChar*)"value");
        Action *aAction = new Action(type);        
        aAction->setMessage(QString::fromStdString(value));
        
        if(eContainer != NULL)
        {
          eContainer->addAction(aAction);
        }
        
        if(event != NULL)
        {
          event->addAction(aAction);
        }
      }                      
      nodeAction = nodeAction->next;
    } 
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  } 
}

void Engine::loadXmlContextType(xmlNodePtr node)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXmlContextType --> node is null");
    }
    
    xmlNodePtr nodeContext = node->children; //context node
    
    //loop through each context node and collect info
    while(nodeContext != NULL)
    {       
      if(nodeContext->type == XML_ELEMENT_NODE && xmlStrcmp(nodeContext->name,(const xmlChar*)"context")==0)
      {
        int id = atoi((const char*)xmlGetProp(nodeContext,(const xmlChar*)"id"));
        Context::ContextType type;
        
        std::string stype = (char*)xmlGetProp(nodeContext,(const xmlChar*)"type");
        if(stype == "area" || stype == "AREA")
        {
          type = Context::AREA;
        }
        if(stype == "tripwire" || stype == "TRIPWIRE")
        {
          type = Context::TRIPWIRE;
        }
        
        std::string name = (char*)xmlGetProp(nodeContext,(const xmlChar*)"name");
        std::string desc = (char*)xmlGetProp(nodeContext,(const xmlChar*)"desc");
        
        
        if(type == Context::AREA)
        {
          ContextArea *aContext = new ContextArea(id,type,name,desc);
          //set mask image
          aContext->setMaskFilePath(maskPath);          
          aContext->setQImage(maskPath);
          
          //get color
          xmlNodePtr nodeInsideContext = nodeContext->children;                            
          while(nodeInsideContext != NULL)
          {        
            if(nodeInsideContext->type == XML_ELEMENT_NODE)
            {
              //context color
              if(xmlStrcmp(nodeInsideContext->name,(const xmlChar*)"color")==0)
              { 
                int r = atoi((const char*)xmlGetProp(nodeInsideContext,(const xmlChar*)"r"));
                int g = atoi((const char*)xmlGetProp(nodeInsideContext,(const xmlChar*)"g"));
                int b = atoi((const char*)xmlGetProp(nodeInsideContext,(const xmlChar*)"b"));
                int a = atoi((const char*)xmlGetProp(nodeInsideContext,(const xmlChar*)"a"));
                
                aContext->setColor(r,g,b,a);
              }
            }                    
            nodeInsideContext = nodeInsideContext->next;
          }  
          
          //insert to vector
          mContexts.push_back(aContext);
        }
        
        if(type == Context::TRIPWIRE)
        {
          ContextTripwire *aContext = new ContextTripwire(id,type,name,desc);
          
          xmlNodePtr nodeInsideContext = nodeContext->children;                  
          
          while(nodeInsideContext != NULL)
          { 
            if(nodeInsideContext->type == XML_ELEMENT_NODE)
            {
              if(xmlStrcmp(nodeInsideContext->name,(const xmlChar*)"line")==0)
              {
                Line line;
                this->loadXmlLineType(line,nodeInsideContext);
                aContext->setLine(line);
              }
            }                      
            
            nodeInsideContext = nodeInsideContext->next;
          }            
          
          mContexts.push_back(aContext);
        }
      }   
      nodeContext = nodeContext->next;
    }
    
    
  }
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  }
}

void Engine::loadXMLMaskType(std::string &maskFileName, xmlNodePtr node)
{
  try
  {
    if(node == NULL)
    {
      throw std::runtime_error("Engine::loadXMLMaskType --> node is null");
    }
    
    xmlNodePtr nodeMask = node->children;
    while(nodeMask != NULL)
    { 
      if(nodeMask->type == XML_ELEMENT_NODE && xmlStrcmp(nodeMask->name,(const xmlChar*)"mask")==0)
      {  
        maskFileName = (char*)xmlGetProp(nodeMask,(const xmlChar*)"path");      
      }
      nodeMask = nodeMask->next;
    }
  }
  
  catch(const std::exception &e)
  {
    std::cout<< "Error: " << e.what();
    exit(-1);
  } 
}



void Engine::loadObjectDataFromVirtualFence(std::vector<TrackedObjectVirtualFencing> &mTracksVF)
{
  if(mTracksVF.size() == 0)
    mObjects.clear();
  
  if(mTracksVF.size() > 0)
  {
    if(mObjects.size() == 0)
    {
      for(int i = 0 ; i < mTracksVF.size();i++)
      {
        TrackedObjectVirtualFencing *trackV = &mTracksVF[i];
        Object *newObject = new Object();
        newObject->setId(trackV->mID);
        trackV->setCurrentObjectFrame(newObject);
        trackV->updateRbeObjectFrame(newObject);
        trackV->updateRbeTrajectory(newObject);
        
        mObjects.push_back(newObject);
      }
    }
    
    else if(mObjects.size() > 0)
    {
      //update and add new
      if(mObjects.size() <= mTracksVF.size())
      {
        for(int i = 0; i < mTracksVF.size(); i++)
        {
          TrackedObjectVirtualFencing *trackV = &mTracksVF[i];
          
          if(isObjectExist(trackV->mID))
          {
            //update                        
            Object *newObject = mObjects[i];
            trackV->setCurrentObjectFrame(newObject);
            trackV->updateRbeObjectFrame(newObject);
            trackV->updateRbeTrajectory(newObject);
            
          }
          else
          {
            //add new 
            Object *newObject = new Object();
            newObject->setId(trackV->mID);
            trackV->setCurrentObjectFrame(newObject);
            trackV->updateRbeObjectFrame(newObject);
            trackV->updateRbeTrajectory(newObject);
            
            mObjects.push_back(newObject);
          }
        }
      }
      
      //remove
      else if(mObjects.size() > mTracksVF.size())
      {
        for(int i = 0; i < mObjects.size(); i++)
        {
          Object *newObject = mObjects[i];
          int objectID = newObject->getId();
          
          bool value = false;
          for(int j = 0; j < mTracksVF.size(); j++)
          {
             TrackedObjectVirtualFencing *trackV = &mTracksVF[j];
             int trackId = trackV->mID;
             if(trackId == objectID)
             {
               value = true;               
               break;
             }             
          }
          
          //remove if false
          if(value == false)
          {
            mObjects.erase(mObjects.begin()+i);
          }
        }
      }
    }
  }
}

//false: object doesn't exist
//true: object exist
bool Engine::isObjectExist(int otherVectorID)
{  
  if(mObjects.size() == 0)  
    return false;
  
  std::vector<Object *>::iterator it;
  Object *aObject;
  for(it = mObjects.begin(); it != mObjects.end(); ++it)
  {
    aObject = (*it);
    if(aObject->getId() == otherVectorID)
    {
      return true;
    }
    
    else
    {
      continue;
    }
  }
  
 return false;
}
/**
  * Read all context from xml file and store into memory
 */
void Engine::readContextFile(std::string fileName)
{       
  try
  {
    xmlDocPtr doc;
    xmlNodePtr node;
    
    doc = xmlParseFile(fileName.c_str());        
    
    if(doc != NULL)
    {
      node = doc->children; //root node
      loadXMLMaskType(maskPath,node);
      loadXmlContextType(node);      
    }  
    
    else
    {
      throw std::runtime_error("Engine::loadContextFile --> xmlDoc is null");
    }
    
    xmlFreeDoc(doc);
    
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << " :: " <<  typeid(e).name() <<std::endl;
    exit(-1);
  }
}


/**
 * Load rule xml file
 *
 * read rule file with filename, and begin to load the rules from xml data
 */
void Engine::readRuleFile(std::string fileName)
{
  try
  {
    xmlDocPtr doc;
    xmlNodePtr node;
        
    doc = xmlParseFile(fileName.c_str());
    if(doc != NULL)
    {       
      node = doc->children; //root node ""rules"      
      loadXmlRuleType(node);            
    }
    
    else
    {
      throw std::runtime_error("Engine::loadRuleFile --> xmlDoc is null");
    }
    
    xmlFreeDoc(doc); 
  }
  
  catch(const std::exception &e)
  {
    std::cout << "Error: " << e.what() << std::endl;
    exit(-1);
  }        
}


 
bool Engine::isNewObject(int aID)
{
  bool isNew = true;
  
  for(unsigned i=0; i< mObjects.size(); i++)
  {
    Object *aO = (Object*)mObjects[i];
    
    if(aO->getId() == aID)
    {
      isNew = false;
      break;
    }
  }
  
  return isNew;
}



void Engine::processRule()
{
  for(uint i = 0; i < mRules.size(); i++ )
  {
    Rule *aRule = mRules[i];
    
    aRule->process(mContexts,mObjects);
  }
}


void Engine::cleanRuleEventResultQueue()
{
  for(uint i = 0; i < mRules.size(); i++ )
  {
    Rule *aRule = mRules[i];
    
    aRule->cleanEventResultQueue();
  }
}

void Engine::clear()
{
  mObjects.erase(mObjects.begin(),mObjects.end());
  mRules.erase(mRules.begin(),mRules.end());
  mContexts.erase(mContexts.begin(),mContexts.end());
  mContexts.clear();
  mObjects.clear();
  mRules.clear();
}

Engine::~Engine()
{
  mObjects.erase(mObjects.begin(),mObjects.end());
  mRules.erase(mRules.begin(),mRules.end());
  mContexts.erase(mContexts.begin(),mContexts.end());
  
  mContexts.clear();
  mObjects.clear();
  mRules.clear();
}


