// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_SEGMENTCABINTYPES_HPP
#define STDAIR_BOM_SEGMENTCABINTYPES_HPP

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
  class SegmentCabin;
  
  /** Define the segment-cabin list. */
  using SegmentCabinList_T = std::list<SegmentCabin*>;

  /** Define the segment-cabin map. */
  using SegmentCabinMap_T = std::map<const MapKey_T, SegmentCabin*>;

}
#endif // STDAIR_BOM_SEGMENTCABINTYPES_HPP

