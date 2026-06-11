// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_BREAKPOINTTYPES_HPP
#define STDAIR_BOM_BREAKPOINTTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
// STL
#include <memory>

namespace stdair {

  // Forward declarations
  struct BreakPointStruct;
  
  // ///////////// Type definitions //////////////
  /** Define the smart pointer to a Break Point event . */
  using BreakPointPtr_T = std::shared_ptr<BreakPointStruct>;

  /** Define the list of Break Points. */
  using BreakPointList_T = std::list<BreakPointStruct>;
  
}
#endif // STDAIR_BOM_BREAKPOINTTYPES_HPP

