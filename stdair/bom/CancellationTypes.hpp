// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_CANCELLATIONTYPES_HPP
#define STDAIR_BOM_CANCELLATIONTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <memory>

namespace stdair {

  // Forward declarations
  struct CancellationStruct;
  
  // ///////////// Type definitions //////////////
  /** Define the smart pointer to a cancellation . */
  using CancellationPtr_T = std::shared_ptr<CancellationStruct>;
  
}
#endif // STDAIR_BOM_CANCELLATIONTYPES_HPP

