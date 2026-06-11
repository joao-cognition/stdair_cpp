#ifndef STDAIR_STDAIR_BASIC_TYPES_HPP
#define STDAIR_STDAIR_BASIC_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <list>

namespace stdair {

  // //////////////////////////////////////////////////////////////////////
  // Basic types
  /** Location code (3-letter-code, e.g., LON). */
  using LocationCode_T = std::string;
  
  /** Define a distance (kilometers). */
  using Distance_T = unsigned long int;

  /** Define the Airport Code type (3-letter-code, e.g., LHR). */
  using AirportCode_T = LocationCode_T;

  /** City code */
  using CityCode_T = LocationCode_T;

  /** Define the key description. */
  using KeyDescription_T = std::string;
  
  /** Define the Airline Code type (2-letter-code, e.g., BA). */
  using AirlineCode_T = std::string;

  /** Define the type for flight numbers. */
  using FlightNumber_T = unsigned short;

  /** Define the type for data table numbers. */
  using TableID_T = unsigned short;

  /** Define the cabin code (class of service, e.g., first, business,
      economy). */
  using CabinCode_T = std::string;

  /** Define the code of the fare family (e.g., 1, 2, 3, etc.). */
  using FamilyCode_T = std::string;

  /** Define the code of the policy (e.g., 1, 2, 3, etc.). */
  using PolicyCode_T = std::string;

  /** Define the code of the nesting structure (e.g., "default"). */
  using NestingStructureCode_T = std::string;

  /** Define the code of the nesting node (e.g., 1, 2, 3, etc). */
  using NestingNodeCode_T = std::string;

  /** Define the booking class code (product segment class, e.g., H,
      B, K, etc.). */
  using ClassCode_T = std::string;

  /** Define a identity number. */
  using Identity_T = unsigned long;

  /** Type of trip type (RO=outbound of round-trip, RI=inbound of round-trip, 
      OW=one way). */
  using TripType_T = std::string;

  /** Monetary value */
  using MonetaryValue_T = double;
  
  /** Real number */
  using RealNumber_T = double;
  
  /** Define a percentage value (between 0 and 100%). */
  using Percentage_T = double;

  /** Define a price value (e.g., 1000.0 Euros). */
  using PriceValue_T = double;

  /** Define a yield value (e.g., 1000.0 Euros). */
  using YieldValue_T = double;

  /** Define a price currency (e.g., EUR for Euros). */
  using PriceCurrency_T = std::string;

  /** Define an amount of revenue. */
  using Revenue_T = double;

  /** Define the name of a multiplier. */
  using Multiplier_T = double;
  
  /** Define the number of seats (it can be non integer, because the
      overbooking can be applied at booking class or PNR level). */
  using NbOfSeats_T = double;

  /** Count */
  using Count_T = unsigned int;
  
  /** Number of passengers (in a group) for a booking. */
  using PartySize_T = short;

  /** Define a number of requests. */
  using NbOfRequests_T = double;

  /** Define a number of bookings. */
  using NbOfBookings_T = NbOfRequests_T;

  /** Define a number of cancellations. */
  using NbOfCancellations_T = NbOfRequests_T;

  /** Define a number of travel solutions
      (in a travel solution block). */
  using NbOfTravelSolutions_T = unsigned short;

  /** Define the list of class codes as a string. */
  using ClassList_String_T = std::string;

  /** Define a number of segment-dates (in a path). */
  using NbOfSegments_T = unsigned short;

  /** Define a number of airlines (in a path). */
  using NbOfAirlines_T = unsigned short;

  /** Define an availability. */
  using Availability_T = double;

  /** Define the price of a travel solution. */
  using Fare_T = double;

  /** Define the censorship flag. */
  using Flag_T = bool;

  /** Define the unsigned index type. */
  using UnsignedIndex_T = unsigned int;

  /** Define the number of booking classes. */
  using NbOfClasses_T = unsigned int;

  /** Define the number of fare families. */
  using NbOfFareFamilies_T = unsigned int;

  // ///////////// Technical ////////////////
  /** File or directory name.
      <br>It may contain paths, relative or absolute (e.g., /foo/bar
      or C:\foo\bar). */
  using Filename_T = std::string;
  
  /** Define the file address type (e.g. "a_directory/a_filename").
      <br>NOTE: That type should be deprecated. */
  using FileAddress_T = std::string;

  /** Progress status (usually, a percentage expressed as a floating
      point number). */
  using ProgressPercentage_T = float;

}
#endif // STDAIR_STDAIR_BASIC_TYPES_HPP
