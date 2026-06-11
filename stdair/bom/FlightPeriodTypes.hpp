// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_FLIGHTPERIODTYPES_HPP
#define STDAIR_BOM_FLIGHTPERIODTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations.
  class FlightPeriod;

  /** Define the flight-period list. */
  using FlightPeriodList_T = std::list<FlightPeriod*>;

  /** Define the flight-period map. */
  using FlightPeriodMap_T = std::map<const MapKey_T, FlightPeriod*>;
}
#endif // STDAIR_BOM_FLIGHTPERIODTYPES_HPP

