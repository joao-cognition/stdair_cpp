// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_BOOKINGREQUESTTYPES_HPP
#define STDAIR_BOM_BOOKINGREQUESTTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// STL
#include <memory>

namespace stdair {

  // Forward declarations
  struct BookingRequestStruct;
  
  // ///////////// Type definitions //////////////
  /** Define the smart pointer to a booking request. */
  using BookingRequestPtr_T = std::shared_ptr<BookingRequestStruct>;
  
  /** Define the hash key for the demand generator. */
  using DemandGeneratorKey_T = std::string;

  
}
#endif // STDAIR_BOM_BOOKINGREQUESTTYPES_HPP

