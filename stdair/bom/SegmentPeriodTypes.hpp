// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_SEGMENTPERIODTYPES_HPP
#define STDAIR_BOM_SEGMENTPERIODTYPES_HPP

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
  class SegmentPeriod;
  
  /** Define the segment-period list. */
  using SegmentPeriodList_T = std::list<SegmentPeriod*>;

  /** Define the segment-period map. */
  using SegmentPeriodMap_T = std::map<const MapKey_T, SegmentPeriod*>;

  /** Define the list of pair<MapKey_T, SegmentPeriod>. */
  using SegmentPeriodWithKey_T = std::pair<MapKey_T, SegmentPeriod*>;
  using SegmentPeriodDetailedList_T = std::list<SegmentPeriodWithKey_T>;
}
#endif // STDAIR_BOM_SEGMENTPERIODTYPES_HPP

