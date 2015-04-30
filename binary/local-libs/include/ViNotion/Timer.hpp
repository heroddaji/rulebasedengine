/** \file
 * Timer class header file.
 *
 * $Id: Timer.hpp 1641 2010-09-03 13:48:44Z rick $
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <string>
#include <iostream>

#ifndef _WIN32
#include <boost/date_time/posix_time/posix_time.hpp>
#endif

#include "Compat.hpp"

namespace Vi
{
  /**
   * Timer class (1337 71m3r c14ss). For windows we use the high
   *  performance counters.  Note that the resolution of the timing is
   *  determined by the used processor hardware. For non-windows we
   *  use the boost posix_time timers.
   */
  class EXPORT_TYPE Timer
  {
  public:
    /**
     * Constructor, starts timer automatically.
     *
     * If you don't want the timer to start automatically, you can call
     *  start() to set the new start time and reset() to reset the 
     *  cumulative counter. Note that the cumulative counter is NOT reset,
     *  so by calling start() and stop() the timer acts as a stopwatch.
     */
    Timer();

    /// Operator += increments the timeElapsed with the argument's timeElapsed.
    Timer& operator+=(const Timer& aTimer);

    /// Sets the new start time for the counter.
    void start();

    /// Resets the cumulative counter, but does NOT set new start time.
    void reset();

    /// Stops the timer and increases the cumulative counter.
    unsigned int stop(const std::string &comment = "");

    /**
     * Returns the cumulative elapsed time from last stop() call.
     *
     * If stop() has not yet been called, returns the previous timeElapsed 
     *  value from previous call to stop() and thus 0 if stop() has not been
     *  called at all.
     */
    unsigned int getCum(const std::string &comment = "");

    /// Returns the cumulative elapsed time until now, but does NOT stop the timer.
    unsigned int get(const std::string &comment = "");

  private:
  
#ifdef _WIN32
    /// Frequency of the high performance counter.
    __int64 frequency;
    __int64 timeStart;
#else
    /// Time when the timer was started the previous time, updated when calling 
    /// start() and reset by reset().
    boost::posix_time::ptime timeStart;
#endif

    /// Elapsed time until now, only updated when calling stop().
    unsigned int timeElapsed;
  };
}

#endif // TIMER_HPP
