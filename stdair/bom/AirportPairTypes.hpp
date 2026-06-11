// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_AIRPORTPAIRTYPES_HPP
#define STDAIR_BOM_AIRPORTPAIRTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// STDAIR
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations.
  class AirportPair;
  
  /** Define the airport-pair list. */
  using AirportPairList_T = std::list<AirportPair*>;

  /** Define the airport-pair map. */
  using AirportPairMap_T = std::map<const MapKey_T, AirportPair*>;

  /** Define the list of pair<MapKey_T, AirportPair>. */
  using AirportPairWithKey_T = std::pair<MapKey_T, AirportPair*>;
  using AirportPairDetailedList_T = std::list<AirportPairWithKey_T>;
}
#endif // STDAIR_BOM_AIRPORTPAIRTYPES_HPP

