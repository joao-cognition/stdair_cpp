// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_ONDDATETYPES_HPP
#define STDAIR_BOM_ONDDATETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// Stdair
#include <stdair/bom/key_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_demand_types.hpp>

namespace stdair {

  // Forward declarations.
  class OnDDate;

  /** Define the O&D date list. */
  using OnDDateList_T = std::list<OnDDate*>;

  /** Define the OnD date map. */
  using OnDDateMap_T = std::map<const MapKey_T, OnDDate*>;

  /**
   * Define the yield mean and standard deviation for a certain cabin/class path.
   * This map is mandatory when using the default BOM tree.
   * This map can be empty if yields are charged otherwise (input file, ...)
   */
  using StringDemandStructPair_T = std::pair<std::string, YieldDemandPair_T>;
  using StringDemandStructMap_T = std::map<std::string, YieldDemandPair_T>;

  /**
   * Define the string matching a (cabin,class) path.
   * (i.e, the string is "Y:M;" for a one leg O&D with the cabin Y and the class M;
   * the string is "Y:M;Y:Y;" for a two legs O&D with the cabin Y and the class M
   * for the first leg, and the cabin Y and the class Y for the second leg).
   */
  using StringCabinClassPairListMap_T =
    std::map<std::string, CabinClassPairList_T>;
  using StringCabinClassPair_T = std::pair<std::string, CabinClassPairList_T>;

  /**
   * Define the WTP mean and standard deviation for a certain cabin code.
   * This information is needed to forecast O&D demand per cabin. 
   */
  using CabinForecastMap_T = std::map<CabinCode_T, WTPDemandPair_T>;
  using CabinForecastPair_T = std::pair<CabinCode_T, WTPDemandPair_T>;

}
#endif // STDAIR_BOM_ONDDATETYPES_HPP
