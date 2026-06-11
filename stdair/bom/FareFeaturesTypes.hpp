// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_FAREFEATURESTYPES_HPP
#define STDAIR_BOM_FAREFEATURESTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// STDAIR
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations.
  class FareFeatures;
  
  /** Define the date-period list. */
  using FareFeaturesList_T = std::list<FareFeatures*>;

  /** Define the date-period  map. */
  using FareFeaturesMap_T = std::map<const MapKey_T, FareFeatures*>;

  /** Define the list of pair<MapKey_T, FareFeatures>. */
  using FareFeaturesWithKey_T = std::pair<MapKey_T, FareFeatures*>;
  using FareFeaturesDetailedList_T = std::list<FareFeaturesWithKey_T>;
}
#endif // STDAIR_BOM_FAREFEATURESTYPES_HPP

