#include "ObjectFrame.hpp"
#include "Misc.hpp"

using namespace Rbe;
int leakCounter=0;

ObjectFrame::ObjectFrame()
{  
  leakCounter++;
}

ObjectFrame::ObjectFrame(int oX,int oY, int oW, int oH)
{  
  leakCounter++;
  this->mX = oX;
  this->mY = oY;
  this->mWidth= oW;
  this->mHeight= oH;  
}

ObjectFrame::ObjectFrame(ObjectFrame *outFrame)
{
  mX = outFrame->getX();
  mY = outFrame->getY();
  mWidth = outFrame->getWidth();
  mHeight = outFrame->getHeight();
}

ObjectFrame::~ObjectFrame()
{
  leakCounter--;
}


