// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_FAREFAMILYTYPES_HPP
#define STDAIR_BOM_FAREFAMILYTYPES_HPP

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
  class FareFamily;

  /** Define the fare family list. */
  using FareFamilyList_T = std::list<FareFamily*>;

  /** Define the fare family map. */
  using FareFamilyMap_T = std::map<const MapKey_T, FareFamily*>;
}
#endif // STDAIR_BOM_FAREFAMILYTYPES_HPP
