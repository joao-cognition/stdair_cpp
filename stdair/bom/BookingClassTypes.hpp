// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_BOOKINGCLASSTYPES_HPP
#define STDAIR_BOM_BOOKINGCLASSTYPES_HPP

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
  class BookingClass;

  /** Define the booking class list. */
  using BookingClassList_T = std::list<BookingClass*>;

  /** Define the booking class map. */
  using BookingClassMap_T = std::map<const MapKey_T, BookingClass*>;
}
#endif // STDAIR_BOM_BOOKINGCLASSTYPES_HPP

