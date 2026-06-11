// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_DATEPERIODTYPES_HPP
#define STDAIR_BOM_DATEPERIODTYPES_HPP

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
  class DatePeriod;
  
  /** Define the date-period list. */
  using DatePeriodList_T = std::list<DatePeriod*>;

  /** Define the date-period  map. */
  using DatePeriodMap_T = std::map<const MapKey_T, DatePeriod*>;

  /** Define the list of pair<MapKey_T, DatePeriod>. */
  using DatePeriodWithKey_T = std::pair<MapKey_T, DatePeriod*>;
  using DatePeriodDetailedList_T = std::list<DatePeriodWithKey_T>;
}
#endif // STDAIR_BOM_DATEPERIODTYPES_HPP

