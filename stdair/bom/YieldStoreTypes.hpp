// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_YIELDSTORETYPES_HPP
#define STDAIR_BOM_YIELDSTORETYPES_HPP

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
  class YieldStore;

  /** Define the Inventory list. */
  using YieldStoreList_T = std::list<YieldStore*>;
  
  /** Define the Inventory map. */
  using YieldStoreMap_T = std::map<const MapKey_T, YieldStore*>;
  
}
#endif // STDAIR_BOM_YIELDSTORETYPES_HPP

