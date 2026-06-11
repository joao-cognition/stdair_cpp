// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_FLIGHTDATETYPES_HPP
#define STDAIR_BOM_FLIGHTDATETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations
  class FlightDate;

  // //////////////////////// Type definitions /////////////////////////
  /** Define the flight-date list. */
  using FlightDateList_T = std::list<FlightDate*>;

  /** Define the flight-date map. */
  using FlightDateMap_T = std::map<const MapKey_T, FlightDate*>;

}
#endif // STDAIR_BOM_FLIGHTDATETYPES_HPP

