// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_POSCHANNELTYPES_HPP
#define STDAIR_BOM_POSCHANNELTYPES_HPP

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
  class PosChannel;
  
  /** Define the fare-point_of_sale list. */
  using PosChannelList_T = std::list<PosChannel*>;

  /** Define the fare-point_of_sale map. */
  using PosChannelMap_T = std::map<const MapKey_T, PosChannel*>;

  /** Define the list of pair<MapKey_T, PosChannel>. */
  using PosChannelWithKey_T = std::pair<MapKey_T, PosChannel*>;
  using PosChannelDetailedList_T = std::list<PosChannelWithKey_T>;
}
#endif // STDAIR_BOM_POSCHANNELTYPES_HPP
