// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_POLICYTYPES_HPP
#define STDAIR_BOM_POLICYTYPES_HPP

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
  class Policy;

  /** Define the fare family list. */
  using PolicyList_T = std::list<Policy*>;

  /** Define the fare family map. */
  using PolicyMap_T = std::map<const MapKey_T, Policy*>;
  
}
#endif // STDAIR_BOM_POLICYTYPES_HPP
