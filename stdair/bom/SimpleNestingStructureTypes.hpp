// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_SIMPLENESTINGSTRUCTURETYPES_HPP
#define STDAIR_BOM_SIMPLENESTINGSTRUCTURETYPES_HPP

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
  class SimpleNestingStructure;

  /** Define the fare family list. */
  using SimpleNestingStructureList_T = std::list<SimpleNestingStructure*>;

  /** Define the fare family map. */
  using SimpleNestingStructureMap_T =
    std::map<const MapKey_T, SimpleNestingStructure*>;
  
}
#endif // STDAIR_BOM_SIMPLENESTINGSTRUCTURETYPES_HPP
