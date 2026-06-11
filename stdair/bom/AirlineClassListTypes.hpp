// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_AIRLINECLASSLISTTYPES_HPP
#define STDAIR_BOM_AIRLINECLASSLISTTYPES_HPP

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
  class AirlineClassList;
  
  /** Define the segment-features list. */
  using AirlineClassListList_T = std::list<AirlineClassList*>;

  /** Define the segment-features map. */
  using AirlineClassListMap_T = std::map<const MapKey_T, AirlineClassList*>;

  /** Define the list of pair<MapKey_T, AirlineCodeList>. */
  using AirlineClassListWithKey_T = std::pair<MapKey_T, AirlineClassList*>;
  using AirlineClassListDetailedList_T = std::list<AirlineClassListWithKey_T>;
}
#endif // STDAIR_BOM_AIRLINECLASSLISTTYPES_HPP
