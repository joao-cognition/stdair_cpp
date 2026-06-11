// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_YIELDFEATURESTYPES_HPP
#define STDAIR_BOM_YIELDFEATURESTYPES_HPP

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
  class YieldFeatures;
  
  /** Define the date-period list. */
  using YieldFeaturesList_T = std::list<YieldFeatures*>;

  /** Define the date-period  map. */
  using YieldFeaturesMap_T = std::map<const MapKey_T, YieldFeatures*>;

  /** Define the list of pair<MapKey_T, YieldFeatures>. */
  using YieldFeaturesWithKey_T = std::pair<MapKey_T, YieldFeatures*>;
  using YieldFeaturesDetailedList_T = std::list<YieldFeaturesWithKey_T>;
}
#endif // STDAIR_BOM_YIELDFEATURESTYPES_HPP

