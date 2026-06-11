#ifndef STDAIR_STDAIR_INVENTORY_TYPES_HPP
#define STDAIR_STDAIR_INVENTORY_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <map>
#include <list>
// BOOST
#include <boost/multi_array.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_maths_types.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace stdair {

  // //////// Type definitions /////////
  /** Define the type for network ID. */
  using NetworkID_T = std::string;

  /** Define a list of airline code. */
  using AirlineCodeList_T = std::vector<AirlineCode_T>;

  /** Define the list of list of class codes as a string. */
  using ClassList_StringList_T = std::vector<ClassList_String_T>;

  /** Define a list of class code. */
  using ClassCodeList_T = std::vector<ClassCode_T>;

  /** Define the sub-class code (e.g., 0, 1, 2, etc.). The subclass is
      a sub-structure for the booking class, allowing to have specific
      rules for some criteria like POS. */
  using SubclassCode_T = unsigned short;

  /** Define the flight path code (code made by a suite of flight numbers). */
  using FlightPathCode_T = std::string;

  /** Map between the cabin codes and the booking class codes within
      each cabin. */
  using CabinBookingClassMap_T = std::map<CabinCode_T, ClassList_String_T>;

  /** Curve key for FRAT5 or FF Disutility. */
  using CurveKey_T = std::string;
  
  /** Define the cabin capacity (resource, e.g., 200 seats).
      <br>The capacity is expressed as a double to cope with overbooking. */
  using CabinCapacity_T = double;

  /** Define a number of flight dates. */
  using NbOfFlightDates_T = double;

  /** Define the commited space of a cabin. */
  using CommittedSpace_T = double;

  /** Define the unsold protection (UPR). */
  using UPR_T = double;

  /** Define the value of the booking limit. */
  using BookingLimit_T = double;

  /** Define the value of the authorization level. */
  using AuthorizationLevel_T = double;

  /** Define the value of the adjustment for cabin capacity. */
  using CapacityAdjustment_T = double;

  /** Define the number of seat which could not be used for the booking. */
  using BlockSpace_T = double;

  /** Define an availability status (AVS). */
  using AvailabilityStatus_T = bool;

  /** Define a list of availabilities. */
  using BucketAvailabilities_T = std::vector<Availability_T>;

  /** Define a number of yields. */
  using NbOfYields_T = double;

  /** Define a number of InventoryControlRules. */
  using NbOfInventoryControlRules_T = double;

  /** Define availibility of booking limit. */
  using CensorshipFlag_T = bool;

  /** Define the type of day-to-departure. */
  using DTD_T = short;
  
  /** Define the type of data collection point. */
  using DCP_T = short;
  
  /** Define the type of data collection point list. */
  using DCPList_T = std::list<DCP_T>;

  /** Define the DTD (days to departure) frat5 coef map. */
  using DTDFratMap_T = std::map<DTD_T, RealNumber_T>;

  /** Define the DTD (days to departure) probability map. */
  using DTDProbMap_T = std::map<FloatDuration_T, float>;
  
  /** Define the list of censorship flags (une list per booking
      class, one censorship flag per DCP). */
  using CensorshipFlagList_T = std::vector<CensorshipFlag_T>;

  /** Define the bookingRatio (for instance OnD bookings
      over whole class bookings). */
  using BookingRatio_T = double;

  /** Define the yield of a virtual class. */
  using Yield_T = double;

  /** Define the yield level (yield as an integer). */
  using YieldLevel_T = unsigned int;

  /** Define the <YieldLevel, demand> demand map. */
  using YieldLevelDemandMap_T = std::map<YieldLevel_T, MeanStdDevPair_T>;

  /** Define the <Yield, demand> pair type.*/
  using YieldDemandPair_T = std::pair<Yield_T, MeanStdDevPair_T>;

  /** Define the Bid-Price. */
  using BidPrice_T = double;

  /** Define a Bid-Price Vector. */
  using BidPriceVector_T = std::vector<BidPrice_T>;

  /** Define the current index of a Bid-Price Vector (for a given LegCabin). */
  using SeatIndex_T = unsigned int;
  
  /** Mode of inventory control. */
  using ControlMode_T = std::string;

  /** Define the rate of overbooking */
  using OverbookingRate_T = double;

  /** Define the Booking Limit.
      <br>It is a double, as it allows for overbooking. */
  using BookingLimit_T = double;

  /** Define the Protection Level.
      <br>It is a double, as it allows for overbooking. */
  using ProtectionLevel_T = double;

  /** Define the list of EMSR values for the EMSR algorithm. */
  using EmsrValueList_T = std::vector<double>;

  /** Define the vector of booking limits.
      <br> It is a vector of double. */
  using BookingLimitVector_T = std::vector<double>;

  /** Define the vector of protection levels.
      <br> It is a vector of double. */
  using ProtectionLevelVector_T = std::vector<double>;

  /** Define a snapshot block. */
  using SnapshotBlock_T = boost::multi_array<double, 2>;

  /** Define a range for array view. */
  using SnapshotBlockRange_T = SnapshotBlock_T::index_range;

  /** Define a view for a given DTD. */
  using SegmentCabinDTDSnapshotView_T = SnapshotBlock_T::array_view<1>::type;

  /** Define a view for a given range of DTD. */
  using SegmentCabinDTDRangeSnapshotView_T =
    SnapshotBlock_T::array_view<2>::type;
  
  /** Define a const view for a given DTD. */
  using ConstSegmentCabinDTDSnapshotView_T =
    SnapshotBlock_T::const_array_view<1>::type;

  /** Define a const view for a given range of DTD. */
  using ConstSegmentCabinDTDRangeSnapshotView_T =
    SnapshotBlock_T::const_array_view<2>::type;
  
  /** Define the segment ID within a snapshot data table. */
  using SegmentDataID_T = unsigned short;
  
  /** Define the leg ID within a snapshot data table. */
  using LegDataID_T = unsigned short;

  /** Define the index type of a class within a
      snapshot block of a leg/segment. */
  using ClassIndex_T = unsigned short;

}
#endif // STDAIR_STDAIR_INVENTORY_TYPES_HPP
