#include "Context.hpp"

using namespace Rbe;

Context::Context()
{  
}

Context::Context(int id, ContextType type, std::string name,std::string desc)
{
  mId = id;
  mType= type;
  mName = name;
  mDesc = desc;
}

Context::~Context()
{
}

