// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_RMEVENTTYPES_HPP
#define STDAIR_BOM_RMEVENTTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
// STL
#include <memory>

namespace stdair {

  // Forward declarations
  struct RMEventStruct;
  
  // ///////////// Type definitions //////////////
  /** Define the smart pointer to a RM event . */
  using RMEventPtr_T = std::shared_ptr<RMEventStruct>;

  /** Define the list of RM events. */
  using RMEventList_T = std::list<RMEventStruct>;
  
}
#endif // STDAIR_BOM_RMEVENTTYPES_HPP

