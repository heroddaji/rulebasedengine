#include "ContextTripwire.hpp"


using namespace Rbe;
ContextTripwire::ContextTripwire()
{
  mType = Context::TRIPWIRE;
}

ContextTripwire::ContextTripwire(int id, Context::ContextType type, std::string name, std::string desc):Context(id,type,name,desc)
{
  
}

void ContextTripwire::setLine(Line &line)
{
  mLine = line;
}

Line* ContextTripwire::getLine()
{
  return &mLine;
}

ContextTripwire::~ContextTripwire()
{  
}
