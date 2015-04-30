#include "ContextArea.hpp"

using namespace Rbe;

ContextArea::ContextArea()
{    
  mType = Context::AREA;
}

ContextArea::ContextArea(int id, Context::ContextType type, std::string name, std::string desc):Context(id,type,name,desc)
{  
}

void ContextArea::setQImage(std::string filePath)
{
  QString path = QString::fromStdString(filePath);
  mImage = new QImage(path);  
}

void ContextArea::setColor(int r, int g, int b, int a)
{
  mColor =  QColor(r,g,b,a);
}

ContextArea::~ContextArea()
{
  delete mImage;
}

