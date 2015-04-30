/** \file
 * Settings class prototype.
 *
 * $Id: Settings.hpp 1980 2011-01-17 15:15:47Z kris $
 */

#ifndef VI_SETTINGS_HPP
#define VI_SETTINGS_HPP

#include <string>
#include <map>
#include <SimpleIni.h>

#include <ViNotion/Compat.hpp>

#include "Parameter.hpp"

namespace Vi
{
  /**
   * Settings class. Basically this is a combination of an std::map with the
   *  Vi::Parameter class, plus extra functionality to deal with .ini files,
   *  taken from SimpleIni.
   *
   * \todo Implement the saving of data. Not trivial, as new settings may be
   *        added to the Setttings object, other than were in a possibly loaded
   *        .ini file. Also comments may get lost, maybe put a comment field in
   *        the Vi::Parameter object itself.
   * \todo Distinguish sections of parameters, such as SimpleIni does.
   * \todo Support multiple values for a parameter?
   * \todo Extend with future XML functionality.
   */
  class EXPORT_TYPE Settings
  {
  public:

    /**
     * Constructor. Optionally the constructor can be instructed to load a .ini
     *  file to initialize the Settings object with, this file will be opened
     *  for reading, and closed immediately thereafter (i.e. no filedescriptor
     *  will be left open during the lifetime of the Settings object).
     *
     * \param[in] fIniFile Optional .ini file from which to initialize.
     */
    Settings(const std::string &fIniFile = std::string(""));

    //-------------------------------------------------------------------------
    /// \{ \name Accessing parameters

    /**
     * Get the parameter object given the parameter name.
     *
     * \param[in]   fName The name of the parameter.
     * \param[out] fParam The Parameter object (if successfully fetched),
     *    otherwise, the given parameter object is left untouched.
     *
     * \return True if the parameter was successfully fetched, false if not.
     */
    bool getParameter(const std::string &fName, Vi::Parameter &fParam) const;

    //-------------------------------------------------------------------------
    /// \{ \name Modifying parameters

    /**
     * Set a parameter to the Settings object. If the parameter with 
     *
     * \param[in]  fName The name of the parameter to set.
     * \param[in] fParam The parameter to set.
     *
     * \return True if the parameter already existed (and was overwritten),
     *          false if otherwise.
     */
    bool setParameter(const std::string &fName, const Vi::Parameter &fParam);

    /**
     * Remove a parameter from the Settings object.
     *
     * \param[in] fName The name of the parameter to be removed.
     *
     * \return True if the parameter was deleted, false if the parameter did
     *          not exist.
     */
    bool delParameter(const std::string &fName);

    //-------------------------------------------------------------------------
    /// \{ \name File storage/retreival of settings

    /**
     * Load the parameters in the Settings object from a .ini file.
     *
     * \note This function will not clear the settings object and thus will
     *        overwrite any identically named parameters that are loaded from
     *        the file.
     *
     * \param[in] fName The name of the file to be loaded.
     */
    void loadFromFile(const std::string &fName);

    //-------------------------------------------------------------------------
    /// \{ \name Miscellanea

    /**
     * Check whether the Settings class is empty.
     *
     * \return True if the Settings class is empty, false if not.
     */
    bool isEmpty() const;

    /**
     * Clear all parameters from the Settings object.
     *
     * \note This function will also close the file (if loaded).
     */
    void clear();

    /**
     * Print the current Settings content to standard output (for debugging).
     */
    void print() const;

  private:

    /**
     * Read data into Parameter map from a given .ini file.
     */
    void readFromIniFile(const std::string &fIniFile);

    /// The map containing all the parameters.
    std::map<std::string, Vi::Parameter> mParameterMap;

    /// The SimpleIni .ini file object.
    CSimpleIniA *mIniFile;
  };
}

#endif // VI_SETTINGS_HPP
