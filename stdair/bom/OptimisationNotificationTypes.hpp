// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_OPTIMISATIONNOTIFICATIONTYPES_HPP
#define STDAIR_BOM_OPTIMISATIONNOTIFICATIONTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <memory>

namespace stdair {

  // Forward declarations
  struct OptimisationNotificationStruct;
  
  // ///////////// Type definitions //////////////
  /** Define the smart pointer to a optimisation notification. */
  using OptimisationNotificationPtr_T =
    std::shared_ptr<OptimisationNotificationStruct>;
  
}
#endif // STDAIR_BOM_OPTIMISATIONNOTIFICATIONTYPES_HPP

