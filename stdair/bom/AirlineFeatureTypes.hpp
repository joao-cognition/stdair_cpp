// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_AIRLINEFEATURETYPES_HPP
#define STDAIR_BOM_AIRLINEFEATURETYPES_HPP

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
  class AirlineFeature;

  /** Define the airline feature list. */
  using AirlineFeatureList_T = std::list<AirlineFeature*>;

  /** Define the airline feature map. */
  using AirlineFeatureMap_T = std::map<const MapKey_T, AirlineFeature*>;

}
#endif // STDAIR_BOM_AIRLINEFEATURETYPES_HPP

