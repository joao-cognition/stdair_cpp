// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_BOMIDTYPES_HPP
#define STDAIR_BOM_BOMIDTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>
#include <stdair/bom/BomID.hpp>

namespace stdair {

  // Forward declarations.
  class BookingClass;

  /** Define the booking class ID. */
  using BookingClassID_T = struct BomID<BookingClass>;

  /** Define the list of booking class ID's. */
  using BookingClassIDList_T = std::list<BookingClassID_T>;
}
#endif // STDAIR_BOM_BOMIDTYPES_HPP

