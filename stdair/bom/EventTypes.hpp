// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_EVENTTYPES_HPP
#define STDAIR_BOM_EVENTTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// STL
#include <memory>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/stdair_event_types.hpp>
#include <stdair/basic/ProgressStatus.hpp>
#include <stdair/bom/key_types.hpp>

namespace stdair {

  /// Forward declarations
  struct EventStruct;
  
  /**
   * Define an element of a event list.
   */
  using EventListElement_T = std::pair<const LongDuration_T, EventStruct>;
  
  /**
   * Define a list of events.
   */
  using EventList_T = std::map<const LongDuration_T, EventStruct>;
}
#endif // STDAIR_BOM_EVENTTYPES_HPP

