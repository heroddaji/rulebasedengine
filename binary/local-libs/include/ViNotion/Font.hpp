/** \file
 * Font class header file.
 *
 * $Id: Font.hpp 1690 2010-09-13 09:36:08Z kris $
 */

#ifndef FONT_HPP
#define FONT_HPP

#include <vector>
#include <string>

#include "Compat.hpp"
#include "Image.hpp"
#include "Misc.hpp"
#include "Draw.hpp"

// Forward declarations.
typedef struct FT_FaceRec_ *FT_Face;
typedef struct FT_BitmapGlyphRec_ *FT_BitmapGlyph;
typedef struct FT_LibraryRec_ *FT_Library;

namespace Vi
{
  /**
   * Font rendering class. This class loads a font into textures and
   *  displays them. It is loosely based on NeHe lesson 43
   *  (http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=43). It
   *  uses FreeType (http://www.freetype.org/) for font rendering and
   *  this class thus also supports all file formats FreeType supports.
   */
  class EXPORT_TYPE Font
  {
  public:
    /**
     * Initialize the font class instantiation using a font file.
     *
     * \param[in] fFilename File name of the font file.
     * \param[in] fFontSize Font size in pixels.
     */
    void init(const std::string &fFilename, unsigned int fFontSize = 32);

    /**
     * Initialize the font class instantiation using in memory data.
     *
     * \param[in] fData     Pointer to the beginning of the font data.
     * \param[in] fDataSize Size of the font data.
     * \param[in] fFontSize Font size in pixels.
     */
    void init(const void *fData = NULL, size_t fDataSize = 0, unsigned int fFontSize = 32);

    /**
     * Draw text on image. Newlines (\n) are also handled.
     *
     * \param[in,out] fImage Image to draw.
     * \param[in]     fStr   String to print.
     * \param[in]     fPos   Start position of string.
     * \param[in]     fCol   Text color.
     */
    void drawText(Vi::Image<> &fImage,
                  const std::string &fStr,
                  Vi::Point<int> fPos,
                  Vi::YCCColor fCol) const;

    /**
     * Destructor.
     */
    ~Font();
    
  private:
    /**
     * Initialize the font class instantiation. Fonts can be loaded
     *  from a file or from memory. If fData equals NULL, the font is
     *  read from the file indicated by fFilename. The function only
     *  loads the characters for the ASCII character encoding.
     *
     * \param[in] fFilename File name of the font file.
     * \param[in] fData     Pointer to the beginning of the font data.
     * \param[in] fDataSize Size of the font data.
     * \param[in] fFontSize Font size in pixels.
     */
    void init(const std::string &fFilename, const void *fData, size_t fDataSize, unsigned int fFontSize);

    /**
     * Render a bitmap for a character.
     *
     * \param[in] fFace Font face to get character from.
     * \param[in] fCh   ASCII value of the character.
     */
    void renderChar(FT_Face fFace, unsigned char fCh);
    
    std::vector<FT_BitmapGlyph> mGlyphs; ///< Vector containing glyphs of the font in ASCII order from 0 to 127.
    unsigned int mFontSize;              ///< Size of the font in pixels.
    FT_Library mLibrary;                 ///< The handle to a FreeType library instance. 
  };
}
#endif // FONT_HPP
