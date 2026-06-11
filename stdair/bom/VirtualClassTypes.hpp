// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_VIRTUALCLASSTYPES_HPP
#define STDAIR_BOM_VIRTUALCLASSTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
#include <map>
// STDAIR
#include <stdair/stdair_basic_types.hpp>

namespace stdair {

  // Forward declarations.
  struct VirtualClassStruct;
  
  /** Define the booking class list. */
  using VirtualClassList_T = std::list<VirtualClassStruct>;
  
  /** Define the booking class map. */
  using VirtualClassMap_T = std::map<const YieldLevel_T, VirtualClassStruct>;
}
#endif // STDAIR_BOM_VIRTUALCLASSTYPES_HPP

