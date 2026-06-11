#ifndef STDAIR_BAS_BASCOMPARSERHELPERTYPES_HPP
#define STDAIR_BAS_BASCOMPARSERHELPERTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <sstream>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/service/Logger.hpp>

namespace stdair {
  
  // ////////////////////////////////////////////////////////////////////
  //
  // Parser structure helper
  //
  // ////////////////////////////////////////////////////////////////////
  /** Date & time element parser. */
  template <int MIN = 0, int MAX = 0>
  struct date_time_element {
    unsigned int _value;

    // ////////// Constructors ///////////
    /** Default constructor. */
    date_time_element () { }
    /** Default copy constructor. */
    date_time_element (const date_time_element& t) : _value (t._value) { }
    /** Constructor. */
    date_time_element (int i) : _value (i) { }
    /** Checker. */
    void check () const {
      if (_value < MIN || _value > MAX) {
        std::ostringstream oMessage;
        oMessage << "The value: " << _value << " is out of range ("
                 << MIN << ", " << MAX << ")";
        throw stdair::ParserException (oMessage.str());
      }
    }
  };

  /** Operator* overload. */
  template <int MIN, int MAX>
  inline date_time_element<MIN,
                           MAX> operator*(const date_time_element<MIN, MAX>& o1,
                                          const date_time_element<MIN, MAX>& o2){
    return date_time_element<MIN, MAX> (o1._value * o2._value);
  }

  /** Operator+ overload. */
  template <int MIN, int MAX>
  inline date_time_element<MIN,
                           MAX> operator+(const date_time_element<MIN, MAX>& o1,
                                          const date_time_element<MIN, MAX>& o2){
    return date_time_element<MIN, MAX> (o1._value + o2._value);    
  }

  /** Type definitions for the date and time elements. */
  using hour_t = date_time_element<0, 23>;
  using minute_t = date_time_element<0, 59>;
  using second_t = date_time_element<0, 59>;
  using year_t = date_time_element<1900, 2100>;
  using month_t = date_time_element<1, 12>;
  using day_t = date_time_element<1, 31>;
  
}
#endif // STDAIR_BAS_BASCOMPARSERHELPERTYPES_HPP
