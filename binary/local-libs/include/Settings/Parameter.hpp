/** \file
 * Parameter data structure prototype.
 *
 * $Id: Parameter.hpp 1983 2011-01-18 09:41:42Z kris $
 */

#ifndef VI_PARAMETER_HPP
#define VI_PARAMETER_HPP

#include <string>
#include <stdint.h>

#include <ViNotion/Misc.hpp>
#include <ViNotion/Bbox.hpp>

namespace Vi
{
  /**
   * Enumerator describing the available parameter types.
   */
  enum ParameterType
  {
    PARTYPE_BOOL,   ///< Boolean.
    PARTYPE_INT8,   ///< 8-bit integer.
    PARTYPE_UINT8,  ///< 8-bit unsigned integer.
    PARTYPE_INT16,  ///< 16-bit integer.
    PARTYPE_UINT16, ///< 16-bit unsigned integer.
    PARTYPE_INT32,  ///< 32-bit integer.
    PARTYPE_UINT32, ///< 32-bit unsigned integer.
    PARTYPE_INT64,  ///< 64-bit integer.
    PARTYPE_UINT64, ///< 64-bit unsigned integer.
    PARTYPE_FLOAT,  ///< Floating point.
    PARTYPE_DOUBLE, ///< Double precision floating point.
    PARTYPE_STRING, ///< String.
    PARTYPE_FRAC,   ///< Vi::Frac<int> fractional.
    PARTYPE_POINT,  ///< Vi::Point<int>.
    PARTYPE_BBOX    ///< Vi::Bbox<int>.
  };                    

  /**
   * The parameter data structure. This data type describes a single parameter
   *  setting, e.g. a threshold of sorts, having a certain type.
   */
  class Parameter
  {
  public:

    /**
     * Constructor.
     *
     * \param[in]  fType The type of the Parameter.
     * \param[in] fValue Value of the parameter.
     */
    Parameter(Vi::ParameterType fType = Vi::PARTYPE_BOOL,
              const std::string fValue = "false");

    //-------------------------------------------------------------------------
    /// \{ \name Parameter value getters

    /**
     * Get the parameter value.
     *
     * This function carries out a cast from the string value of the parameter
     *  which holds the actual parameter. To induce type correctness, a casting
     *  exception is thrown for incorrect types and over-/underflows.
     *
     * \param[in] fValue The value casted from the internal string value.
     */
    void getValue(bool &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(int8_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(uint8_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(int16_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(uint16_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(int32_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(uint32_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(int64_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(uint64_t &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(float &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(double &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(std::string &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(Vi::Frac<int> &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(Vi::Point<int> &fValue) const;

    /// See getValue(bool &fValue).
    void getValue(Vi::Bbox<int> &fValue) const;

    //-------------------------------------------------------------------------
    /// \{ \name Parameter value setters

    /**
     * Set the parameter value.
     *
     * This function carries out a cast from the string value of the parameter
     *  which holds the actual parameter. To induce type correctness, a casting
     *  exception is thrown for incorrect types and over-/underflows.
     *
     * \param[in] fValue The value casted from the internal string value.
     */
    void setValue(bool fValue);

    /// See setValue(bool &fValue).
    void setValue(int8_t fValue);

    /// See setValue(bool &fValue).
    void setValue(uint8_t fValue);

    /// See setValue(bool &fValue).
    void setValue(int16_t fValue);

    /// See setValue(bool &fValue).
    void setValue(uint16_t fValue);

    /// See setValue(bool &fValue).
    void setValue(int32_t fValue);

    /// See setValue(bool &fValue).
    void setValue(uint32_t fValue);

    /// See setValue(bool &fValue).
    void setValue(int64_t fValue);

    /// See setValue(bool &fValue).
    void setValue(uint64_t fValue);

    /// See setValue(bool &fValue).
    void setValue(float fValue);

    /// See setValue(bool &fValue).
    void setValue(double fValue);

    /// See setValue(bool &fValue).
    void setValue(const std::string &fValue);

    /// See setValue(bool &fValue).
    void setValue(const Vi::Frac<int> &fValue);

    /// See setValue(bool &fValue).
    void setValue(const Vi::Point<int> &fValue);

    /// See setValue(bool &fValue).
    void setValue(const Vi::Bbox<int> &fValue);

    //-------------------------------------------------------------------------
    /// \{ \name Miscellanea

    /**
     * Get a string containing the parameter data.
     */
    std::string toString() const;

    Vi::ParameterType mType; ///< The type of the parameter.
    std::string mValue;      ///< The value of the parameter.

    /**
     * Convert a Parameter type to a string value.
     *
     * \param[in] fType The type enumerator.
     *
     * \return The string corresponding to the enumerator type.
     */
    static std::string typeToString(Vi::ParameterType fType);

    /**
     * Convert a string to a parameter type. May throw an error if the type is
     *  not detected.
     *
     * \param[in] fString The type string.
     *
     * \return The enumerator type corresponding to the string.
     */
    static Vi::ParameterType stringToType(const std::string &fString);

  private:

    /**
     * Check the given type, throw an error if there is no correspondence.
     *
     * \param[in] fType1 The first input type (the one of the Parameter).
     * \param[in] fType2 The second input type.
     */
    void checkParamTypeGetValue(Vi::ParameterType fType1,
                                Vi::ParameterType fType2) const;
  };
}

#endif // VI_PARAMETER_HPP
