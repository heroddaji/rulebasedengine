#ifndef OBJECTFRAME_HPP
#define OBJECTFRAME_HPP


#include <assert.h>

namespace Rbe
{   
  class ObjectFrame
  {
  public:
    ObjectFrame();
    ObjectFrame(ObjectFrame *);    
    ObjectFrame(int x,int y, int w, int h);
    ~ObjectFrame();
    
    int getX(){return mX;}
    int getY(){return mY;}
    int getWidth(){return mWidth;}
    int getHeight(){return mHeight;}    
    
    void setX(int x){mX = x;}
    void setY(int y){mY = y;}
    void setWidth(int w){mWidth = w;}
    void setHeight(int h){mHeight = h;}
    
     int getXCenter(){return (mX + mWidth/2);}
     int getYCenter(){return (mY + mHeight/2);}
    
  private:        
    int mX;
    int mY;
    int mWidth;
    int mHeight;            
    
  };
}



#endif // OBJECTFRAME_HPP
