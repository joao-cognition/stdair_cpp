// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_TIMEPERIODTYPES_HPP
#define STDAIR_BOM_TIMEPERIODTYPES_HPP

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
  class TimePeriod;
  
  /** Define the time-period list. */
  using TimePeriodList_T = std::list<TimePeriod*>;

  /** Define the time-period  map. */
  using TimePeriodMap_T = std::map<const MapKey_T, TimePeriod*>;

  /** Define the list of pair<MapKey_T, TimePeriod>. */
  using TimePeriodWithKey_T = std::pair<MapKey_T, TimePeriod*>;
  using TimePeriodDetailedList_T = std::list<TimePeriodWithKey_T>;
}
#endif // STDAIR_BOM_TIMEPERIODTYPES_HPP

