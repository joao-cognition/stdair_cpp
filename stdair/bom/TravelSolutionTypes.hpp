// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_TRAVELSOLUTIONTYPES_HPP
#define STDAIR_BOM_TRAVELSOLUTIONTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
#include <map>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/key_types.hpp>
#include <stdair/stdair_inventory_types.hpp> // bid price related types.
#include <stdair/bom/BomIDTypes.hpp>

namespace stdair {

  // Forward declarations.
  struct TravelSolutionStruct;
  
  /** Define the booking class list. */
  using TravelSolutionList_T = std::list<TravelSolutionStruct>;

  /** Define the segment path key. */
  using SegmentPath_T = KeyList_T;

  /** Define the list of segment paths. */
  using SegmentPathList_T = std::list<SegmentPath_T>;

  /** Define booking class - object ID map. */
  using ClassObjectIDMap_T = std::map<const ClassCode_T, BookingClassID_T>;

  /** Define list of boking class - object ID maps.*/
  using ClassObjectIDMapHolder_T = std::list<ClassObjectIDMap_T>;
  
  /** Define booking class - yield map. */
  using ClassYieldMap_T = std::map<const ClassCode_T, YieldValue_T>;

  /** Define list of booking class - yield maps. */
  using ClassYieldMapHolder_T = std::list<ClassYieldMap_T>;

  /** Define list of bid price vectors. */
  using BidPriceVectorHolder_T = std::list<BidPriceVector_T>;

  /** Define booking class - bid price reference map. */
  using ClassBpvMap_T = std::map<const ClassCode_T, const BidPriceVector_T*>;

  /** Define list of booking class - bid price reference maps. */
  using ClassBpvMapHolder_T = std::list<ClassBpvMap_T>;
}
#endif // STDAIR_BOM_TRAVELSOLUTIONTYPES_HPP

