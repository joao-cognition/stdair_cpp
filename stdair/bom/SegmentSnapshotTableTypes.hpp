// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_SEGMENTSNAPSHOTTABLETYPES_HPP
#define STDAIR_BOM_SEGMENTSNAPSHOTTABLETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// BOOST
#include <boost/multi_array.hpp>
// StdAir
#include <stdair/bom/key_types.hpp>
#include <stdair/stdair_inventory_types.hpp>

namespace stdair {

  // Forward declarations
  class SegmentSnapshotTable;
  class SegmentCabin;

  // //////////////////////// Type definitions /////////////////////////
  /** Define the guillotine-block list. */
  using SegmentSnapshotTableList_T = std::list<SegmentSnapshotTable*>;

  /** Define the guillotine-block map. */
  using SegmentSnapshotTableMap_T =
    std::map<const MapKey_T, SegmentSnapshotTable*>;

  /** Define the map between the segment-cabins and the segment data ID. */
  using SegmentCabinIndexMap_T = std::map<const SegmentCabin*, SegmentDataID_T>;

  /** Define the map between the class and their index. */
  using ClassIndexMap_T = std::map<const MapKey_T, ClassIndex_T>;

}
#endif // STDAIR_BOM_SEGMENTSNAPSHOTTABLETYPES_HPP

