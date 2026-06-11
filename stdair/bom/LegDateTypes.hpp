// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_LEGDATETYPES_HPP
#define STDAIR_BOM_LEGDATETYPES_HPP

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
  class LegDate;
  
  /** Define the leg-date list. */
  using LegDateList_T = std::list<LegDate*>;

  /** Define the leg-date map. */
  using LegDateMap_T = std::map<const MapKey_T, LegDate*>;
}
#endif // STDAIR_BOM_LEGDATETYPES_HPP

