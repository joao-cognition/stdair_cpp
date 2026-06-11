#ifndef STDAIR_STDAIR_DEMAND_TYPES_HPP
#define STDAIR_STDAIR_DEMAND_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <list>
#include <map>
// Boost (Extended STL)
#include <boost/tuple/tuple.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_inventory_types.hpp>


namespace stdair {

  // //////// Type definitions /////////
  /** Define the availability option allowing the ticket change. */
  using ChangeFees_T = bool;

  /** Define the refundable availability of a tickets. */
  using NonRefundable_T = bool;

  /** Define the saturday stay of a tickets. */
  using SaturdayStay_T = bool;

  /** Define the average ratio (between 0 and 100 percent) of demand with
      a saturday stay status equal to TRUE. */
  using SaturdayStayRatio_T = double;

  /** Define the average ratio of demand with change fee
      availability. */
  using ChangeFeesRatio_T = double;

  /** Define the average ratio of demand with non-refundable
      availability. */
  using NonRefundableRatio_T = double;

  /** Define the disutility of restriction. */
  using Disutility_T = double;

  /** Define the passenger characteristics, leisure or business
      for instance (1-letter-code, e.g., L or B). */
  using PassengerType_T = std::string;

  /** Define the identifier of a distribution pattern (e.g., 1). */
  using DistributionPatternId_T = std::string;

  /** Define the identifier of a cancellation rate curve (e.g., C1). */
  using CancellationRateCurveId_T = std::string;

  /** Define the identifier of an airline preference set list (e.g., AP1). */
  using AirlinePreferenceId_T = std::string;

  /** Define a cancellation & and no-show rate pair. */
  using CancellationNoShowRatePair_T = std::pair<Percentage_T, Percentage_T>;

  /** Define the identifier of a demand characteristics pattern
      (e.g. Ch12); for a customer choice model   */
  using CharacteristicsPatternId_T = std::string;

  /** Define characteristics component index (e.g. W for WTP) */
  using CharacteristicsIndex_T = std::string;

  /** Define a Willingness-To-Pay (WTP) (e.g., 1000.0 Euros). */
  using WTP_T = double;

  /** Define the name of a WTP-component of characteristics pattern. */
  using CharacteristicsWTP_tuple_T = boost::tuples::tuple<double, WTP_T>;
  
  /** Define the <WTP, demand> pair type.*/
  using WTPDemandPair_T = std::pair<WTP_T, MeanStdDevPair_T>;

  /** Define a number of cancellations (travellers). */
  using NbOfCancellations_T = NbOfRequests_T;

  /** Define a number of no-shows. */
  using NbOfNoShows_T = NbOfRequests_T;

  /** Define a indicator of demand to class matching. */
  using MatchingIndicator_T = double;

  /** Type definition for the hashed key of the DemandStreamKey object. */
  using DemandStreamKeyStr_T = std::string;

  /** Type of booking channel (D=direct, I=indirect, N=oNline, F=oFfline). */
  using ChannelLabel_T = std::string;

  /** Type of frequent flyer (P=Platinum, G=Gold, S=Silver, M=Member, N=None).*/
  using FrequentFlyer_T = std::string;

  /** Define the Request status for booking (1-letter-code, e.g.,
      B: booked, C: cancelled, R: Rejected). */
  using RequestStatus_T = std::string;

  /** Define a map between a BookingID and a TravelSolutionID. */
  using BookingTSIDMap_T = std::map<Identity_T, Identity_T>;

  /** Define a pair (cabin code, class code) e.g., (economy, K). */
  using CabinClassPair_T = std::pair<CabinCode_T, ClassCode_T>;

  /** Define a list of pair (cabin code, class code). */
  using CabinClassPairList_T = std::list<CabinClassPair_T>;

  /** Define the forecast booking requests proportion. */
  using ProportionFactor_T = double;

  /** Define the list of forecast booking requests proportions. */
  using ProportionFactorList_T = std::list<ProportionFactor_T>;

  /** Define the O&D string key (e.g. "SQ;11,2010-Feb-08;SIN,BKK"). */
  using OnDString_T = std::string;

  /** Define the list of O&D string key. */
  using OnDStringList_T = std::list<OnDString_T>;

}
#endif // STDAIR_STDAIR_DEMAND_TYPES_HPP
