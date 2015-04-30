/** \file
  * The ContextArea class file. Define an area.
  * 
  * $Id$
  */


#ifndef CONTEXTAREA_HPP
#define CONTEXTAREA_HPP

#include <iostream>
#include "Context.hpp"
#include <QImage>
#include <QColor>
#include <QString>
#include <QRgb>

#include <ViNotion/Image.hpp>
#include <ViNotion/ImageFile.hpp>

namespace Rbe
{
  
  class Object; 
  
  /**
    * ContextArea class, derived class from base class Context
    * This class define an area by using mask image and color.
    */   
  class ContextArea: public Context
  {
  public:
    
    /// Constructor.
    ContextArea();
    
    /**
      * Constructor.
      *
      * \param[in] id id of the area.
      * \param[in] type type of the area.
      * \param[in] name name of the area.
      * \param[in] desc description of the area.
      */
    ContextArea(int id, Context::ContextType type, std::string name, std::string desc);
    
    /// Destructor.
    ~ContextArea();
    
    
    /**
      * Get the path of the mask image.
      *
      * \return mMaskFilePath
      */
    inline std::string getMaskFilePath(){return mMaskFilePath;}
    
    /**
      * Set mask image path
      *
      * \param[in] filePath the path of the image file.
      */
    inline void setMaskFilePath(std::string filePath){mMaskFilePath = filePath;}        
    
    /**
      * set mask image.
      * \param[in] filePath the path of image file.
      */
    void setQImage(std::string filePath);    
    
    /**
      * get mask image.      
      */
    QImage* getQImage(){return mImage;}
    
    /**
      * set color of the area.
      * \param[in] r red value.
      * \param[in] g green value.
      * \param[in] b blue value.
      * \param[in] a alpha value.
      */
    void setColor(int r, int g, int b, int a);
    
    /**
      * get area color.
      */
    QColor getColor(){return mColor;}
    
    /**
      * get the QRbg qt type from the area color.
      */
    QRgb getRgbColorValue(){return mColor.rgba();}
    
  private:
    
    std::string mMaskFilePath; ///< path the mask image.        
    QImage *mImage;  ///< Mask image (use QImage class).        
    QColor mColor;  ///< Color of the area (use for event detection)
  };
}
#endif // CONTEXTAREA_HPP
