// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_CONFIGHOLDERTYPES_HPP
#define STDAIR_BOM_CONFIGHOLDERTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
// STL
#include <memory>

namespace stdair {

  // Forward declarations
  struct ConfigHolderStruct;
  
  // ///////////// Type definitions //////////////
  /** Define the smart pointer to a Config Holder structure. */
  using ConfigHolderPtr_T = std::shared_ptr<ConfigHolderStruct>;
  
}
#endif // STDAIR_BOM_CONFIGHOLDERTYPES_HPP

