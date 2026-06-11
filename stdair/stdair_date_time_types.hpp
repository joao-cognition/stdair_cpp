#ifndef STDAIR_STDAIR_DATE_TIME_TYPES_HPP
#define STDAIR_STDAIR_DATE_TIME_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost (Extended STL)
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace stdair {

  // //////// Type definitions /////////
  /** Define the type for durations (e.g., elapsed in-flight time). */
  using Duration_T = boost::posix_time::time_duration;

  /** Define the type for date (e.g., departure date of a flight). */
  using Date_T = boost::gregorian::date;
    
  /** Time */
  using Time_T = boost::posix_time::time_duration;

  /** Define an accurate time (date+time). */
  using DateTime_T = boost::posix_time::ptime;

  /** Define the Period (e.g., period during which flights depart). */
  using DatePeriod_T = boost::gregorian::date_period;

  /** Define the Day-Of-the-Week as a string. */
  using DOW_String_T = std::string;

  /** Define the Date Offset (e.g., -1 ). */
  using DateOffset_T = boost::gregorian::date_duration;

  /** Define a duration in number of days. */
  using DayDuration_T = int;

  /** Define the Saturday stay status of a travel. */
  using SaturdayStay_T = bool;
  
  /** Time duration in (integer) number of seconds */
  using IntDuration_T = long int;

  /** Time duration in (unsigned long long integer) number of milliseconds */
  using LongDuration_T = long long int;
  
  /** Duration in (float) number of time units */
  using FloatDuration_T = float;

}
#endif // STDAIR_STDAIR_DATE_TIME_TYPES_HPP
