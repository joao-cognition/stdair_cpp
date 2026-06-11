// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_INVENTORYTYPES_HPP
#define STDAIR_BOM_INVENTORYTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// Stdair
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations.
  class Inventory;

  /** Define the Inventory list. */
  using InventoryList_T = std::list<Inventory*>;

  /** Define the Inventory map. */
  using InventoryMap_T = std::map<const MapKey_T, Inventory*>;
  
}
#endif // STDAIR_BOM_INVENTORYTYPES_HPP
