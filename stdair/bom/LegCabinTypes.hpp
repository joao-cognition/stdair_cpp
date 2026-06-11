// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_LEGCABINTYPES_HPP
#define STDAIR_BOM_LEGCABINTYPES_HPP

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
  class LegCabin;

  /** Define the leg-cabin list. */
  using LegCabinList_T = std::list<LegCabin*>;

  /** Define the leg-cabin map. */
  using LegCabinMap_T = std::map<const MapKey_T, LegCabin*>;

}
#endif // STDAIR_BOM_LEGCABINTYPES_HPP

