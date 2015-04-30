/** \file
  * The ContextTripwire class file. Define a tripwire.
  * 
  * $Id$
  */

#ifndef CONTEXTTRIPWIRE_HPP
#define CONTEXTTRIPWIRE_HPP

#include <string>
#include <vector>
#include "Context.hpp"
#include "Misc.hpp"

namespace Rbe
{ 
  /**
    * Tripwire class, derive from Context.
    * define a tripwire by a line.
    */
  class ContextTripwire: public Context
  {
  public:
    
    /**
      * Constructor.
      */
    ContextTripwire();
    
    /**
      * Constructor.
      *
      * \param[in] id id of the tripwire.
      * \param[in] type type of the tripwire.
      * \param[in] name name of the tripwire.
      * \param[in] desc description of the tripwire.
      */
    ContextTripwire(int id, Context::ContextType type, std::string name, std::string desc);
    
    /**
      * Destructor.
      */
    ~ContextTripwire();  
    
    /**
      * set tripwire position.
      *
      * \param[in] line a line to define tripwire pos.
      */
    void setLine(Line &line);
    
    /**
      * get tripwire position by its line.
      *
      * \return line.
      */
    Line* getLine();
    
  private:           
    Line mLine;  ///< tripwire line.
  };
}
#endif // CONTEXTTRIPWIRE_HPP
