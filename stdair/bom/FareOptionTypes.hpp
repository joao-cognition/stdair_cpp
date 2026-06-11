// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_FAREOPTIONTYPES_HPP
#define STDAIR_BOM_FAREOPTIONTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
#include <map>
// STDAIR
#include <stdair/stdair_types.hpp>
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations.
  struct FareOptionStruct;
  
  /** Define the booking class list. */
  using FareOptionList_T = std::list<FareOptionStruct>;

}
#endif // STDAIR_BOM_FAREOPTIONTYPES_HPP

