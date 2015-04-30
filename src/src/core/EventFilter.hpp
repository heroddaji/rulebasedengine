#ifndef EVENTFILTER_HPP
#define EVENTFILTER_HPP

#include <string>

namespace Rbe
{

class EventFilter
{
public: 
  
  enum FilterType
  {
    CONTEXT_ID = 456,
    OBJECT_ID
  };
  
  EventFilter();
  ~EventFilter();
  
  
  void setFilterType(std::string type);
  FilterType getFilterType(){return mType;}
  
  void setFilterValue(int value){mValue = value;}  
  int getFilterValue(){return mValue;}
  
private:
  FilterType mType;
  int mValue;
};
}
#endif // EVENTFILTER_HPP
