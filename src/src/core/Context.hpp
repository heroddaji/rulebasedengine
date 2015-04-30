/** \file
  * The Context base class
  * 
  * $Id$
  */

#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <ViNotion/Image.hpp>
#include <ViNotion/ImageFile.hpp>


namespace Rbe
{
  /**
    * Context describes a virtual place, object that will be check for events.
    * This is a context base class.
    */  
  class Context{                
    
  public:
    
    /**
      * Enumerator for different context types.
      */
    enum ContextType
    { 
      AREA, 
      TRIPWIRE, 
      DISABLE
    };
    
    /**
      * Constructor
      */
    Context();
    
    /**
      * Constructor.
      *
      * \param[in] id id of the context.
      * \param[in] type type of the context.
      * \param[in] name name of the context.
      * \param[in] desc description of the context.
      */
    Context(int id, ContextType type, std::string name, std::string desc);
    
    /// Virtual destructor.
    virtual ~Context();
    
    /// get the context id.
    inline int getID() const { return mId; }        
    
    /// get the context type.
    inline ContextType getType() const {return mType;}
    
    /// get the context name.
    inline std::string getName() const {return mName;}
    
    /// get the context description.
    inline std::string getDesc() const {return mDesc;}
    
    /** 
      * Set context id.
      *
      * \param[in] id id of the context.
      */
    inline void setID(int id){mId = id;}
    
    /** 
      * set context type.
      *
      * \param[in] type type of context, for example Area or Tripwire.
      */
    inline void setType(ContextType type){mType = type;}
    
    /** 
      * Set context name.    
      *
      * \param[in] name name of the context.
      */
    inline void setName(std::string name){mName = name;}
    
    /** 
      * Set context description.
      *
      * \param[in] desc description of the context.
      */
    inline void setDesc(std::string desc){mDesc = desc;}    
    
    
  protected:
    
    int mId;           ///< context id.    
    ContextType mType; ///< context type.    
    std::string mName; ///< context name.
    std::string mDesc; ///< context description.
    
  };  
}

#endif
