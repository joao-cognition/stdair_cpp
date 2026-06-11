#ifndef STDAIR_STDAIR_RM_TYPES_HPP
#define STDAIR_STDAIR_RM_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <map>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_inventory_types.hpp>

namespace stdair {
  // Forward declarations.
  class BookingClass;

  // //////// Type definitions /////////
  /** Mode of the forecaster. */
  using ForecasterMode_T = std::string;

  /** Limit of similar flight-dates used in the forecaster. */
  using HistoricalDataLimit_T = short;

  /** Mode of the forecaster. */
  using OptimizerMode_T = std::string;

  /** Define the demand for a policy. */
  using PolicyDemand_T = NbOfBookings_T;

  /** Define the vector of generated demand (for MC integration use).
      <br> It is a vector of double. */
  using GeneratedDemandVector_T = std::vector<double>;

  /** Define the holder of the generated demand vectors. */
  using GeneratedDemandVectorHolder_T = std::vector<GeneratedDemandVector_T>;

  /** Define the sellup probability.*/
  using SellupProbability_T = double;
  
    /** Define the vector of historical unconstrained demand. */
  using UncDemVector_T = std::vector<NbOfRequests_T>;
  
    /** Define the vector of historical bookings. */
  using BookingVector_T = std::vector<NbOfBookings_T>;

  /** Define the FRAT5 coefficient. */
  using FRAT5_T = double;

  /** Define the FRAT5 curve. */
  using FRAT5Curve_T = std::map<const DTD_T, FRAT5_T>;

  /** Define the fare family disutility curve. */
  using FFDisutilityCurve_T = std::map<const DTD_T, double>;

  /** Define the sell-up factor curve. */
  using SellUpCurve_T = std::map<const DTD_T, double>;

  /** Define the dispatching factor curve. */
  using DispatchingCurve_T = std::map<const DTD_T, double>;

  /** Define the map between class and sell-up factor curve. */
  using BookingClassSellUpCurveMap_T = std::map<BookingClass*, SellUpCurve_T>;

  /** Define the map between class and dispatching factor curve. */
  using BookingClassDispatchingCurveMap_T =
    std::map<BookingClass*, DispatchingCurve_T>;

  /** Define the map between the yield of a class and the demand forecast
      of this class within a policy. */
  using YieldDemandMap_T = std::map<const Yield_T, double>;

  /** Define the revenue of a policy */
  using Revenue_T = double;

  /** Define the number of samples for the generated demand of booking class */
  using NbOfSamples_T = unsigned int;

}
#endif // STDAIR_STDAIR_RM_TYPES_HPP
