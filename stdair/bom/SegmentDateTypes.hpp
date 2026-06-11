// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_SEGMENTDATETYPES_HPP
#define STDAIR_BOM_SEGMENTDATETYPES_HPP

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
  class SegmentDate;
  
  /** Define the segment-date list. */
  using SegmentDateList_T = std::list<SegmentDate*>;

  /** Define the segment-date map. */
  using SegmentDateMap_T = std::map<const MapKey_T, SegmentDate*>;
}

#endif // STDAIR_BOM_SEGMENTDATETYPES_HPP

