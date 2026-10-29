/*!
 * \page BomStructsTestSuite_cpp Unit tests for the stdair/bom value structures
 * \code
 */
// STL
#include <exception>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
// Boost
#include <boost/make_shared.hpp>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Event.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/bom/AirlineFeatureKey.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/AirportPairKey.hpp>
#include <stdair/bom/BomHolderKey.hpp>
#include <stdair/bom/BomKeyManager.hpp>
#include <stdair/bom/BomRootKey.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/BookingClassKey.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/bom/ClassAvailabilityStruct.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/DatePeriodKey.hpp>
#include <stdair/bom/DoWStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/FareFamilyKey.hpp>
#include <stdair/bom/FareFeatures.hpp>
#include <stdair/bom/FareFeaturesKey.hpp>
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/bom/FlightDateKey.hpp>
#include <stdair/bom/FlightPeriod.hpp>
#include <stdair/bom/FlightPeriodKey.hpp>
#include <stdair/bom/InventoryKey.hpp>
#include <stdair/bom/LegDateKey.hpp>
#include <stdair/bom/OnDDateKey.hpp>
#include <stdair/bom/OptimisationNotificationStruct.hpp>
#include <stdair/bom/ParsedKey.hpp>
#include <stdair/bom/PeriodStruct.hpp>
#include <stdair/bom/PosChannel.hpp>
#include <stdair/bom/PosChannelKey.hpp>
#include <stdair/bom/RMEventStruct.hpp>
#include <stdair/bom/SegmentDateKey.hpp>
#include <stdair/bom/SegmentPeriod.hpp>
#include <stdair/bom/SegmentPeriodKey.hpp>
#include <stdair/bom/SnapshotStruct.hpp>
#include <stdair/bom/TimePeriod.hpp>
#include <stdair/bom/TimePeriodKey.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/VirtualClassStruct.hpp>
#include <stdair/bom/YieldFeatures.hpp>
#include <stdair/bom/YieldFeaturesKey.hpp>
#include <stdair/bom/YieldStoreKey.hpp>
#include <stdair/factory/FacBom.hpp>
#include <stdair/STDAIR_Service.hpp>

namespace stdair {
  // Defined in BookingRequestStruct.cpp, not declared in any header
  void intDisplay (std::ostream&, const int&);
}

namespace {

  const std::string BA9_KEY ("BA, 9, 2011-06-10, LHR, SYD, 21:45");
  const std::string QF12_KEY ("QF, 12, 2011-06-10, LHR, SYD, 20:45");

  bool contains (const std::string& iStr, const std::string& iSub) {
    return iStr.find (iSub) != std::string::npos;
  }

  template <typename T>
  std::string streamed (const T& iObj) {
    std::ostringstream oStr;
    iObj.toStream (oStr);
    std::istringstream iStr ("");
    const_cast<T&> (iObj).fromStream (iStr);
    return oStr.str();
  }

  stdair::BookingRequestStruct buildRoundTripRequest() {
    const stdair::Date_T lDepDate (2011, boost::gregorian::Jun, 10);
    const stdair::DateTime_T lReqDateTime (stdair::Date_T (2011,
                                                           boost::gregorian::May,
                                                           15),
                                           stdair::Duration_T (10, 5, 0));
    return stdair::BookingRequestStruct ("GEN-1", "LHR", "SYD", "LHR",
                                         lDepDate, lReqDateTime, "Y", 2.0,
                                         "IN", stdair::TRIP_TYPE_ROUND_TRIP,
                                         7, "G", stdair::Duration_T (8, 0, 0),
                                         1500.0, 20.0, true, 15.0, true, 25.0);
  }

  /** Owns a STDAIR_Service (logger + sample BOM builders). */
  struct ServiceFixture {
    ServiceFixture()
      : _logFile ("BomStructsTestSuite.log"),
        _logParams (stdair::LOG::DEBUG, _logFile),
        _service (_logParams) {
    }
    std::ofstream _logFile;
    const stdair::BasLogParams _logParams;
    stdair::STDAIR_Service _service;
  };

  stdair::BookingClass& createBookingClass (const stdair::ClassCode_T& iCode,
                                            const double iMean,
                                            const double iStdDev) {
    const stdair::BookingClassKey lKey (iCode);
    stdair::BookingClass& lBC =
      stdair::FacBom<stdair::BookingClass>::instance().create (lKey);
    lBC.setMean (iMean);
    lBC.setStdDev (iStdDev);
    lBC.generateDemandSamples (4, 7);
    return lBC;
  }
}

BOOST_AUTO_TEST_SUITE (bom_structs_test_suite)

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (booking_request_struct_test, ServiceFixture) {
  const stdair::BookingRequestStruct lReq =
    _service.buildSampleBookingRequest();
  BOOST_CHECK_EQUAL (lReq.getDemandGeneratorKey(), "");
  BOOST_CHECK_EQUAL (lReq.getOrigin(), "LHR");
  BOOST_CHECK_EQUAL (lReq.getDestination(), "SYD");
  BOOST_CHECK_EQUAL (lReq.getPOS(), "LHR");
  BOOST_CHECK_EQUAL (lReq.getPreferedDepartureDate(),
                     stdair::Date_T (2011, boost::gregorian::Jun, 10));
  BOOST_CHECK_EQUAL (lReq.getPreferredDepartureTime(),
                     stdair::Duration_T (8, 0, 0));
  BOOST_CHECK_EQUAL (lReq.getRequestDateTime().date(),
                     stdair::Date_T (2011, boost::gregorian::May, 15));
  BOOST_CHECK_EQUAL (lReq.getPartySize(), 3.0);
  BOOST_CHECK_EQUAL (lReq.getTripType(), stdair::TRIP_TYPE_INBOUND);
  BOOST_CHECK_EQUAL (lReq.getChangeFees(), false);
  BOOST_CHECK_EQUAL (lReq.getChangeFeeDisutility(), 30.0);
  BOOST_CHECK_EQUAL (lReq.getNonRefundable(), false);
  BOOST_CHECK_EQUAL (lReq.getNonRefundableDisutility(), 50.0);
  BOOST_CHECK (!lReq.getPreferredCabin().empty());
  BOOST_CHECK (!lReq.getBookingChannel().empty());
  BOOST_CHECK (!lReq.getFrequentFlyerType().empty());
  BOOST_CHECK (lReq.getWTP() > 0.0);
  BOOST_CHECK (lReq.getValueOfTime() > 0.0);
  BOOST_CHECK (lReq.getStayDuration() >= 0);

  const std::string lDescription = lReq.describe();
  BOOST_CHECK (contains (lDescription, "LHR-SYD"));
  BOOST_CHECK_EQUAL (streamed (lReq), lDescription);

  // Inbound trip: the stay duration is displayed
  const std::string lDisplay = lReq.display();
  BOOST_CHECK (contains (lDisplay, "2011-05-15, 10:00:00, LHR"));
  BOOST_CHECK (contains (lDisplay, "00:00-23:59"));

  // Copy constructor
  const stdair::BookingRequestStruct lCopy (lReq);
  BOOST_CHECK_EQUAL (lCopy.describe(), lDescription);
  BOOST_CHECK_EQUAL (lCopy.display(), lDisplay);

  // Constructor with a demand generator key, round-trip
  const stdair::BookingRequestStruct lRT = buildRoundTripRequest();
  BOOST_CHECK_EQUAL (lRT.getDemandGeneratorKey(), "GEN-1");
  BOOST_CHECK_EQUAL (lRT.getStayDuration(), 7);
  BOOST_CHECK_EQUAL (lRT.getChangeFees(), true);
  BOOST_CHECK (contains (lRT.display(), ", RT, 7, G, 1500"));

  // CRS sample, one-way: the stay duration is displayed as 0
  const stdair::BookingRequestStruct lCRS =
    _service.buildSampleBookingRequest (true);
  BOOST_CHECK (!lCRS.describe().empty());
  BOOST_CHECK (!lCRS.display().empty());

  // One-way trip: the stay duration is displayed as 0
  const stdair::BookingRequestStruct
    lOW ("LHR", "SYD", "LHR", stdair::Date_T (2011, 6, 10),
         lReq.getRequestDateTime(), "Y", 1.0, "DN", stdair::TRIP_TYPE_ONE_WAY,
         5, "M", stdair::Duration_T (9, 0, 0), 800.0, 10.0, false, 0.0,
         false, 0.0);
  BOOST_CHECK (contains (lOW.display(), ", OW, 0, M, 800"));

  // Two-digit helper
  std::ostringstream lInt1, lInt2, lInt3;
  stdair::intDisplay (lInt1, 5);
  stdair::intDisplay (lInt2, 42);
  stdair::intDisplay (lInt3, 1234);
  BOOST_CHECK_EQUAL (lInt1.str(), "05");
  BOOST_CHECK_EQUAL (lInt2.str(), "42");
  BOOST_CHECK_EQUAL (lInt3.str(), "34");
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (fare_option_and_class_availability_test) {
  // Default fare option
  stdair::FareOptionStruct lDefault;
  BOOST_CHECK (lDefault.getClassPath().empty());
  lDefault.setFare (450.0);
  lDefault.setAvailability (9.0);
  lDefault.setChangeFees (true);
  lDefault.setNonRefundable (false);
  lDefault.setSaturdayStay (true);
  lDefault.addClassList ("Y");
  lDefault.addClassList ("M");
  BOOST_CHECK_EQUAL (lDefault.getFare(), 450.0);
  BOOST_CHECK_EQUAL (lDefault.getAvailability(), 9.0);
  BOOST_CHECK_EQUAL (lDefault.getChangeFees(), true);
  BOOST_CHECK_EQUAL (lDefault.getNonRefundable(), false);
  BOOST_CHECK_EQUAL (lDefault.getSaturdayStay(), true);
  BOOST_CHECK_EQUAL (lDefault.getClassPath().size(), 2U);
  BOOST_CHECK_EQUAL (lDefault.describe(),
                     "Class path: Y-M; 450 EUR; conditions: 1 0 1");
  BOOST_CHECK_EQUAL (lDefault.display(), "Y-M, 450, 1 0 1");
  BOOST_CHECK_EQUAL (streamed (lDefault), lDefault.describe());

  const stdair::FareOptionStruct lCopy (lDefault);
  BOOST_CHECK_EQUAL (lCopy.describe(), lDefault.describe());
  lDefault.emptyClassList();
  BOOST_CHECK (lDefault.getClassPath().empty());

  // Full constructor
  const stdair::FareOptionStruct lFO ("Q", 900.0, false, true, false);
  BOOST_CHECK_EQUAL (lFO.getClassPath().size(), 1U);
  BOOST_CHECK_EQUAL (lFO.getClassPath().front(), "Q");
  BOOST_CHECK_EQUAL (lFO.display(), "Q, 900, 0 1 0");

  // Class availability
  const stdair::ClassAvailabilityStruct lEmpty;
  BOOST_CHECK (lEmpty.getClassAvailabilityMap().empty());
  BOOST_CHECK_EQUAL (lEmpty.describe(), "");

  stdair::ClassAvailabilityMap_T lMap;
  lMap.insert (stdair::ClassAvailabilityMap_T::value_type ("Q", 4.0));
  lMap.insert (stdair::ClassAvailabilityMap_T::value_type ("Y", 7.0));
  const stdair::ClassAvailabilityStruct lAvl (lMap);
  BOOST_CHECK_EQUAL (lAvl.getClassAvailabilityMap().size(), 2U);
  BOOST_CHECK_EQUAL (lAvl.describe(), "Q:4 Y:7");
  BOOST_CHECK_EQUAL (lAvl.display(), "Q:4 Y:7");
  BOOST_CHECK_EQUAL (streamed (lAvl), "Q:4 Y:7");
  const stdair::ClassAvailabilityStruct lAvlCopy (lAvl);
  BOOST_CHECK_EQUAL (lAvlCopy.describe(), "Q:4 Y:7");
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (travel_solution_struct_test, ServiceFixture) {
  // Empty travel solution
  const stdair::TravelSolutionStruct lEmpty;
  BOOST_CHECK (lEmpty.getSegmentPath().empty());
  BOOST_CHECK_EQUAL (lEmpty.describe(), "Segment path:  ### Fare options: ");
  BOOST_CHECK_EQUAL (lEmpty.describeSegmentPath(), "Segment path: ");
  BOOST_CHECK_EQUAL (lEmpty.display(), " ---  --- ");

  // Hand-built, two-segment travel solution
  stdair::TravelSolutionStruct lTS;
  lTS.addSegment (BA9_KEY);
  lTS.addSegment (QF12_KEY);
  stdair::ClassAvailabilityMap_T lAvlMap;
  lAvlMap.insert (stdair::ClassAvailabilityMap_T::value_type ("Y", 3.0));
  lTS.addClassAvailabilityMap (lAvlMap);
  lTS.addClassAvailabilityMap (lAvlMap);

  stdair::BookingClass& lY = createBookingClass ("Y", 10.0, 2.0);
  stdair::ClassObjectIDMap_T lIDMap;
  lIDMap.insert (stdair::ClassObjectIDMap_T::value_type
                 ("Y", stdair::BookingClassID_T (lY)));
  lTS.addClassObjectIDMap (lIDMap);

  stdair::ClassYieldMap_T lYieldMap;
  lYieldMap.insert (stdair::ClassYieldMap_T::value_type ("Y", 800.0));
  lTS.addClassYieldMap (lYieldMap);

  stdair::BidPriceVector_T lBpv;
  lBpv.push_back (100.0);
  lBpv.push_back (50.0);
  lTS.addBidPriceVector (lBpv);

  stdair::ClassBpvMap_T lBpvMap;
  lBpvMap.insert (stdair::ClassBpvMap_T::value_type ("Y", &lBpv));
  lTS.addClassBpvMap (lBpvMap);

  const stdair::FareOptionStruct lFO1 ("Y", 1000.0, true, false, false);
  const stdair::FareOptionStruct lFO2 ("Y", 800.0, false, true, true);
  lTS.addFareOption (lFO1);
  lTS.getFareOptionListRef().push_back (lFO2);

  BOOST_CHECK_EQUAL (lTS.getSegmentPath().size(), 2U);
  BOOST_CHECK_EQUAL (lTS.getClassAvailabilityMapHolder().size(), 2U);
  BOOST_CHECK_EQUAL (lTS.getClassObjectIDMapHolder().size(), 1U);
  BOOST_CHECK_EQUAL (lTS.getClassYieldMapHolder().size(), 1U);
  BOOST_CHECK_EQUAL (lTS.getBidPriceVectorHolder().size(), 1U);
  BOOST_CHECK_EQUAL (lTS.getClassBpvMapHolder().size(), 1U);
  BOOST_CHECK_EQUAL (lTS.getFareOptionList().size(), 2U);

  const std::string lDescNoChoice = lTS.describe();
  BOOST_CHECK (contains (lDescNoChoice, "Fare options: "));
  BOOST_CHECK (contains (lDescNoChoice, " , "));
  BOOST_CHECK (contains (lTS.describeSegmentPath(), " - "));
  BOOST_CHECK_EQUAL (streamed (lTS), lDescNoChoice);

  const std::string lDisplay = lTS.display();
  BOOST_CHECK (contains (lDisplay, "[0] "));
  BOOST_CHECK (contains (lDisplay, " ; [1] "));
  BOOST_CHECK (contains (lDisplay, "Y:3"));

  lTS.setChosenFareOption (lTS.getFareOptionList().front());
  BOOST_CHECK_EQUAL (lTS.getChosenFareOption().getFare(), 1000.0);
  BOOST_CHECK (contains (lTS.describe(), "Chosen fare option: "));

  const stdair::TravelSolutionStruct lCopy (lTS);
  BOOST_CHECK_EQUAL (lCopy.describe(), lTS.describe());
  BOOST_CHECK_EQUAL (lCopy.display(), lDisplay);

  // Sample travel solutions from the command layer
  stdair::TravelSolutionList_T lSamples;
  _service.buildSampleTravelSolutions (lSamples);
  BOOST_CHECK (!lSamples.empty());
  for (stdair::TravelSolutionList_T::const_iterator itTS = lSamples.begin();
       itTS != lSamples.end(); ++itTS) {
    BOOST_CHECK (!itTS->describe().empty());
    BOOST_CHECK (!itTS->display().empty());
  }
  stdair::TravelSolutionList_T lPricing;
  _service.buildSampleTravelSolutionForPricing (lPricing);
  BOOST_CHECK (!lPricing.empty());
  BOOST_CHECK (contains (lPricing.front().describeSegmentPath(), "BA"));
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (cancellation_struct_test) {
  const stdair::DateTime_T lDateTime (stdair::Date_T (2011, 5, 20),
                                      stdair::Duration_T (9, 30, 0));
  stdair::SegmentPath_T lPath;
  lPath.push_back (BA9_KEY);
  lPath.push_back (QF12_KEY);

  // Class codes given as a string
  const stdair::CancellationStruct lCx (lPath, "YM", 2, lDateTime);
  BOOST_CHECK_EQUAL (lCx.getSegmentPath().size(), 2U);
  BOOST_CHECK_EQUAL (lCx.getClassList(), "YM");
  BOOST_CHECK (lCx.getClassIDList().empty());
  BOOST_CHECK_EQUAL (lCx.getPartySize(), 2);
  BOOST_CHECK_EQUAL (lCx.getCancellationDateTime(), lDateTime);
  BOOST_CHECK (contains (lCx.describe(), BA9_KEY + "-" + QF12_KEY + ";YM;2;"));
  BOOST_CHECK (contains (lCx.display(), "[0] " + BA9_KEY + " ; [1] "));
  BOOST_CHECK_EQUAL (streamed (lCx), lCx.describe());

  // Class codes given as booking class object IDs
  stdair::BookingClass& lY = createBookingClass ("Y", 5.0, 1.0);
  stdair::BookingClass& lM = createBookingClass ("M", 5.0, 1.0);
  stdair::BookingClassIDList_T lIDList;
  lIDList.push_back (stdair::BookingClassID_T (lY));
  lIDList.push_back (stdair::BookingClassID_T (lM));
  const stdair::CancellationStruct lCxID (lPath, lIDList, 1, lDateTime);
  BOOST_CHECK (lCxID.getClassList().empty());
  BOOST_CHECK_EQUAL (lCxID.getClassIDList().size(), 2U);
  BOOST_CHECK (contains (lCxID.describe(), ";Y-M;1;"));
  BOOST_CHECK (contains (lCxID.display(), ";Y-M;1;"));
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (simple_event_structs_test) {
  const stdair::DateTime_T lDateTime (stdair::Date_T (2011, 1, 15),
                                      stdair::Duration_T (12, 0, 0));
  // Optimisation notification
  const stdair::OptimisationNotificationStruct
    lON ("LHR", "SYD", "LHR", stdair::Date_T (2011, 6, 10), lDateTime, "Y",
         2.0, "DN", "RT", 7, "G", stdair::Duration_T (8, 0, 0), 900.0, 15.0);
  BOOST_CHECK_EQUAL (lON.getOrigin(), "LHR");
  BOOST_CHECK_EQUAL (lON.getDestination(), "SYD");
  BOOST_CHECK_EQUAL (lON.getPOS(), "LHR");
  BOOST_CHECK_EQUAL (lON.getPreferedDepartureDate(),
                     stdair::Date_T (2011, 6, 10));
  BOOST_CHECK_EQUAL (lON.getNotificationDateTime(), lDateTime);
  BOOST_CHECK_EQUAL (lON.getPreferredCabin(), "Y");
  BOOST_CHECK_EQUAL (lON.getPartySize(), 2.0);
  BOOST_CHECK_EQUAL (lON.getOptimisationChannel(), "DN");
  BOOST_CHECK_EQUAL (lON.getTripType(), "RT");
  BOOST_CHECK_EQUAL (lON.getStayDuration(), 7);
  BOOST_CHECK_EQUAL (lON.getFrequentFlyerType(), "G");
  BOOST_CHECK_EQUAL (lON.getPreferredDepartureTime(),
                     stdair::Duration_T (8, 0, 0));
  BOOST_CHECK_EQUAL (lON.getWTP(), 900.0);
  BOOST_CHECK_EQUAL (lON.getValueOfTime(), 15.0);
  BOOST_CHECK (contains (lON.describe(), "for (LHR) LHR-SYD"));
  const stdair::OptimisationNotificationStruct lONCopy (lON);
  BOOST_CHECK_EQUAL (streamed (lONCopy), lON.describe());

  // RM event
  const stdair::RMEventStruct lRM ("BA", "BA9 2011-06-10", lDateTime);
  BOOST_CHECK_EQUAL (lRM.getAirlineCode(), "BA");
  BOOST_CHECK_EQUAL (lRM.getFlightDateDescription(), "BA9 2011-06-10");
  BOOST_CHECK_EQUAL (lRM.getRMEventTime(), lDateTime);
  BOOST_CHECK_EQUAL (lRM.describe(),
                     "BA, BA9 2011-06-10, 2011-Jan-15 12:00:00");
  const stdair::RMEventStruct lRMCopy (lRM);
  BOOST_CHECK_EQUAL (streamed (lRMCopy), lRM.describe());

  // Snapshot
  const stdair::SnapshotStruct lSnap ("AF", lDateTime);
  BOOST_CHECK_EQUAL (lSnap.getAirlineCode(), "AF");
  BOOST_CHECK_EQUAL (lSnap.getSnapshotTime(), lDateTime);
  BOOST_CHECK_EQUAL (lSnap.describe(), "AF, 2011-Jan-15 12:00:00");
  const stdair::SnapshotStruct lSnapCopy (lSnap);
  BOOST_CHECK_EQUAL (streamed (lSnapCopy), lSnap.describe());

  // Break point, from a date-time and from a date
  const stdair::BreakPointStruct lBP (lDateTime);
  BOOST_CHECK_EQUAL (lBP.getBreakPointTime(), lDateTime);
  BOOST_CHECK_EQUAL (lBP.describe(), "2011-Jan-15 12:00:00");
  const stdair::BreakPointStruct lBPDate (stdair::Date_T (2011, 1, 16));
  BOOST_CHECK_EQUAL (lBPDate.describe(), "2011-Jan-16 00:00:00");
  const stdair::BreakPointStruct lBPCopy (lBP);
  BOOST_CHECK_EQUAL (streamed (lBPCopy), lBP.describe());

  // Airline
  stdair::AirlineStruct lAirline;
  lAirline.setAirlineCode ("BA");
  lAirline.setAirlineName ("British Airways");
  BOOST_CHECK_EQUAL (lAirline.getAirlineCode(), "BA");
  BOOST_CHECK_EQUAL (lAirline.getAirlineName(), "British Airways");
  const stdair::AirlineStruct lAF ("AF", "Air France");
  BOOST_CHECK_EQUAL (lAF.describe(), "AF Air France");
  const stdair::AirlineStruct lAirlineCopy (lAirline);
  BOOST_CHECK_EQUAL (streamed (lAirlineCopy), "BA British Airways");
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (event_struct_test, ServiceFixture) {
  const stdair::DateTime_T lDateTime (stdair::Date_T (2011, 1, 15),
                                      stdair::Duration_T (12, 0, 0));
  const stdair::LongDuration_T lExpectedStamp =
    (lDateTime - stdair::DEFAULT_EVENT_OLDEST_DATETIME).total_milliseconds();

  // Default event
  const stdair::EventStruct lDefault;
  BOOST_CHECK_EQUAL (lDefault.getEventType(), stdair::EventType::BKG_REQ);
  BOOST_CHECK_EQUAL (lDefault.getEventTimeStamp(), 0);

  // Booking request
  const stdair::BookingRequestPtr_T lReqPtr =
    boost::make_shared<stdair::BookingRequestStruct>
    (_service.buildSampleBookingRequest());
  stdair::EventStruct lBkg (stdair::EventType::BKG_REQ, lReqPtr);
  BOOST_CHECK_EQUAL (lBkg.getEventTime(), lReqPtr->getRequestDateTime());
  BOOST_CHECK_EQUAL (lBkg.getBookingRequest().getOrigin(), "LHR");
  BOOST_CHECK (contains (lBkg.describe(), "BookingRequest, At "));
  const stdair::LongDuration_T lBkgStamp = lBkg.getEventTimeStamp();
  lBkg.incrementEventTimeStamp();
  BOOST_CHECK_EQUAL (lBkg.getEventTimeStamp(), lBkgStamp + 1);
  const stdair::EventStruct lBkgCopy (lBkg);
  BOOST_CHECK_EQUAL (lBkgCopy.getEventTimeStamp(), lBkgStamp + 1);
  BOOST_CHECK_EQUAL (lBkgCopy.getBookingRequest().describe(),
                     lReqPtr->describe());
  std::istringstream lEmptyStream ("");
  lBkg.fromStream (lEmptyStream);

  // Cancellation
  stdair::SegmentPath_T lPath;
  lPath.push_back (BA9_KEY);
  const stdair::CancellationPtr_T lCxPtr =
    boost::make_shared<stdair::CancellationStruct> (lPath, "Y", 1, lDateTime);
  const stdair::EventStruct lCx (stdair::EventType::CX, lCxPtr);
  BOOST_CHECK_EQUAL (lCx.getEventTimeStamp(), lExpectedStamp);
  BOOST_CHECK_EQUAL (lCx.getEventTime(), lDateTime);
  BOOST_CHECK_EQUAL (lCx.getCancellation().getClassList(), "Y");
  BOOST_CHECK (contains (lCx.describe(), "Cancellation, Segment path: "));
  const stdair::EventStruct lCxCopy (lCx);
  BOOST_CHECK_EQUAL (lCxCopy.describe(), lCx.describe());

  // Optimisation notification
  const stdair::OptimisationNotificationPtr_T lONPtr =
    boost::make_shared<stdair::OptimisationNotificationStruct>
    ("LHR", "SYD", "LHR", stdair::Date_T (2011, 6, 10), lDateTime, "Y",
     1.0, "DN", "OW", 0, "M", stdair::Duration_T (8, 0, 0), 500.0, 10.0);
  const stdair::EventStruct lON (stdair::EventType::OPT_NOT_4_FD, lDateTime,
                                 lONPtr);
  BOOST_CHECK_EQUAL (lON.getEventTimeStamp(), lExpectedStamp);
  BOOST_CHECK_EQUAL (lON.getEventTime(), lDateTime);
  BOOST_CHECK_EQUAL (lON.getOptimisationNotificationStruct().getOrigin(),
                     "LHR");
  BOOST_CHECK (contains (lON.describe(),
                         "OptimisationNotificationForFlightDate"));
  const stdair::EventStruct lONCopy (lON);
  BOOST_CHECK_EQUAL (lONCopy.describe(), lON.describe());

  // Snapshot
  const stdair::SnapshotPtr_T lSnapPtr =
    boost::make_shared<stdair::SnapshotStruct> ("BA", lDateTime);
  const stdair::EventStruct lSnap (stdair::EventType::SNAPSHOT, lSnapPtr);
  BOOST_CHECK_EQUAL (lSnap.getEventTimeStamp(), lExpectedStamp);
  BOOST_CHECK_EQUAL (lSnap.getEventTime(), lDateTime);
  BOOST_CHECK_EQUAL (lSnap.getSnapshotStruct().getAirlineCode(), "BA");
  BOOST_CHECK (contains (lSnap.describe(), "Snapshot, BA, "));
  const stdair::EventStruct lSnapCopy (lSnap);
  BOOST_CHECK_EQUAL (lSnapCopy.describe(), lSnap.describe());

  // RM event
  const stdair::RMEventPtr_T lRMPtr =
    boost::make_shared<stdair::RMEventStruct> ("BA", "BA9", lDateTime);
  const stdair::EventStruct lRM (stdair::EventType::RM, lRMPtr);
  BOOST_CHECK_EQUAL (lRM.getEventTimeStamp(), lExpectedStamp);
  BOOST_CHECK_EQUAL (lRM.getEventTime(), lDateTime);
  BOOST_CHECK_EQUAL (lRM.getRMEvent().getAirlineCode(), "BA");
  BOOST_CHECK (contains (lRM.describe(), "RevenueManagement, BA, BA9"));
  const stdair::EventStruct lRMCopy (lRM);
  BOOST_CHECK_EQUAL (lRMCopy.describe(), lRM.describe());

  // Break point
  const stdair::BreakPointPtr_T lBPPtr =
    boost::make_shared<stdair::BreakPointStruct> (lDateTime);
  const stdair::EventStruct lBP (stdair::EventType::BRK_PT, lBPPtr);
  BOOST_CHECK_EQUAL (lBP.getEventTimeStamp(), lExpectedStamp);
  BOOST_CHECK_EQUAL (lBP.getEventTime(), lDateTime);
  BOOST_CHECK_EQUAL (lBP.getBreakPoint().getBreakPointTime(), lDateTime);
  BOOST_CHECK (contains (lBP.describe(), "BreakPoint, "));
  const stdair::EventStruct lBPCopy (lBP);
  BOOST_CHECK_EQUAL (lBPCopy.describe(), lBP.describe());

  // Event type without a dedicated description
  const stdair::EventStruct lSkd (stdair::EventType::SKD_CHG, lSnapPtr);
  BOOST_CHECK (contains (lSkd.describe(), "(not yet recognised)"));
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (dow_struct_test) {
  const stdair::DoWStruct lNone;
  BOOST_CHECK_EQUAL (lNone.describeShort(), "0000000");
  BOOST_CHECK_EQUAL (lNone.describe(), "");
  BOOST_CHECK (!lNone.isValid());

  stdair::DoWStruct lDoW ("1010100");
  BOOST_CHECK (lDoW.isValid());
  BOOST_CHECK_EQUAL (lDoW.describeShort(), "1010100");
  BOOST_CHECK_EQUAL (lDoW.describe(), "Mon.Wed.Fri.");
  BOOST_CHECK (lDoW.getDayOfWeek (0));
  BOOST_CHECK (!lDoW.getDayOfWeek (1));
  // Standard day-of-week: 0 is Sunday
  BOOST_CHECK (!lDoW.getStandardDayOfWeek (0));
  BOOST_CHECK (lDoW.getStandardDayOfWeek (1));
  BOOST_CHECK (lDoW.getStandardDayOfWeek (3));
  lDoW.setDayOfWeek (6, true);
  BOOST_CHECK (lDoW.getStandardDayOfWeek (0));
  BOOST_CHECK_EQUAL (lDoW.describeShort(), "1010101");
  BOOST_CHECK_THROW (lDoW.getDayOfWeek (7), std::out_of_range);

  // Shifts (positive, negative, wrap-around)
  const stdair::DoWStruct lMon ("1000000");
  BOOST_CHECK_EQUAL (lMon.shift (1).describeShort(), "0100000");
  BOOST_CHECK_EQUAL (lMon.shift (-1).describeShort(), "0000001");
  BOOST_CHECK_EQUAL (lMon.shift (8).describeShort(), "0100000");

  // Intersection
  const stdair::DoWStruct lA ("1110000");
  const stdair::DoWStruct lB ("0111000");
  BOOST_CHECK_EQUAL (lA.intersection (lB).describeShort(), "0110000");
  BOOST_CHECK (!lMon.intersection (lB).isValid());

  const stdair::DoWStruct lCopy (lA);
  BOOST_CHECK_EQUAL (lCopy.describeShort(), "1110000");
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (period_struct_test) {
  const stdair::PeriodStruct lDefault;
  BOOST_CHECK (!lDefault.isValid());  // no day of week set

  const stdair::DatePeriod_T lRange (stdair::Date_T (2011, 1, 1),
                                     stdair::Date_T (2011, 1, 31));
  const stdair::DoWStruct lDoW ("1111100");
  stdair::PeriodStruct lPeriod (lRange, lDoW);
  BOOST_CHECK (lPeriod.isValid());
  BOOST_CHECK_EQUAL (lPeriod.getDateRange(), lRange);
  BOOST_CHECK_EQUAL (lPeriod.getDoW().describeShort(), "1111100");
  BOOST_CHECK (contains (lPeriod.describeShort(), ", 1111100"));
  BOOST_CHECK (contains (lPeriod.describe(), ", Mon.Tue.Wed.Thu.Fri."));

  // Date offset: the day-of-week shifts accordingly
  const stdair::PeriodStruct lShifted =
    lPeriod.addDateOffset (stdair::DateOffset_T (2));
  BOOST_CHECK_EQUAL (lShifted.getDateRange().begin(),
                     stdair::Date_T (2011, 1, 3));
  BOOST_CHECK_EQUAL (lShifted.getDoW().describeShort(), "0011111");

  // Intersection
  const stdair::PeriodStruct lOther (stdair::DatePeriod_T
                                     (stdair::Date_T (2011, 1, 15),
                                      stdair::Date_T (2011, 2, 15)),
                                     stdair::DoWStruct ("1000001"));
  const stdair::PeriodStruct lInter = lPeriod.intersection (lOther);
  BOOST_CHECK_EQUAL (lInter.getDateRange().begin(),
                     stdair::Date_T (2011, 1, 15));
  BOOST_CHECK_EQUAL (lInter.getDoW().describeShort(), "1000000");
  BOOST_CHECK (lInter.isValid());

  // Disjoint date ranges give an invalid period
  const stdair::PeriodStruct lDisjoint (stdair::DatePeriod_T
                                        (stdair::Date_T (2012, 1, 1),
                                         stdair::Date_T (2012, 2, 1)), lDoW);
  BOOST_CHECK (!lPeriod.intersection (lDisjoint).isValid());

  // Setters and copy
  lPeriod.setDateRange (lOther.getDateRange());
  lPeriod.setDoW (lOther.getDoW());
  const stdair::PeriodStruct lCopy (lPeriod);
  BOOST_CHECK_EQUAL (lCopy.describeShort(), lOther.describeShort());
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (parsed_key_test) {
  const stdair::ParsedKey lKey = stdair::BomKeyManager::extractKeys (BA9_KEY);
  BOOST_CHECK_EQUAL (lKey._fullKey, BA9_KEY);
  BOOST_CHECK_EQUAL (lKey.getInventoryKey().getAirlineCode(), "BA");
  const stdair::FlightDateKey lFDKey = lKey.getFlightDateKey();
  BOOST_CHECK_EQUAL (lFDKey.getFlightNumber(), 9);
  BOOST_CHECK_EQUAL (lFDKey.getDepartureDate(), stdair::Date_T (2011, 6, 10));
  BOOST_CHECK_EQUAL (lKey.getLegKey().getBoardingPoint(), "LHR");
  BOOST_CHECK_EQUAL (lKey.getSegmentKey().getOffPoint(), "SYD");
  BOOST_CHECK_EQUAL (lKey.getBoardingTime(), stdair::Duration_T (21, 45, 0));
  BOOST_CHECK_EQUAL (lKey.toString(), "BA; 9, 2011-06-10; LHR, SYD; 21:45");
  BOOST_CHECK_EQUAL (streamed (lKey),
                     "ParsedKey: BA; 9, 2011-06-10; LHR, SYD; 21:45");

  const stdair::ParsedKey lEmpty;
  BOOST_CHECK_EQUAL (lEmpty.toString(), "; , ; , ; ");
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (logging_paths_test, ServiceFixture) {
  // The invalid-key and out-of-range paths log through the StdAir logger,
  // which is initialised by the fixture's STDAIR_Service.
  {
    // Invalid parsed keys
    const stdair::ParsedKey lNoAirline =
      stdair::BomKeyManager::extractKeys ("X");
    BOOST_CHECK_THROW (lNoAirline.getInventoryKey(),
                       stdair::KeyNotFoundException);
    BOOST_CHECK_THROW (lNoAirline.getFlightDateKey(),
                       stdair::KeyNotFoundException);
    BOOST_CHECK_THROW (lNoAirline.getLegKey(), stdair::KeyNotFoundException);
    BOOST_CHECK_THROW (lNoAirline.getSegmentKey(),
                       stdair::KeyNotFoundException);
    BOOST_CHECK_THROW (lNoAirline.getBoardingTime(),
                       stdair::KeyNotFoundException);
    const stdair::ParsedKey lLongAirline =
      stdair::BomKeyManager::extractKeys ("ABCD, 9, 2011-06-10, LHR, SYDX");
    BOOST_CHECK_THROW (lLongAirline.getInventoryKey(),
                       stdair::KeyNotFoundException);
    BOOST_CHECK_THROW (lLongAirline.getSegmentKey(),
                       stdair::KeyNotFoundException);

    // Time period, departure time outside of the range
    const stdair::TimePeriodKey lTPKey (stdair::Duration_T (6, 0, 0),
                                        stdair::Duration_T (12, 0, 0));
    stdair::TimePeriod& lTP =
      stdair::FacBom<stdair::TimePeriod>::instance().create (lTPKey);
    BOOST_CHECK (lTP.isDepartureTimeValid (stdair::Duration_T (8, 0, 0)));
    BOOST_CHECK (!lTP.isDepartureTimeValid (stdair::Duration_T (5, 0, 0)));
    BOOST_CHECK (!lTP.isDepartureTimeValid (stdair::Duration_T (13, 0, 0)));
  }
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (virtual_class_struct_test) {
  stdair::BookingClass& lY = createBookingClass ("Y", 10.0, 2.0);
  stdair::BookingClass& lM = createBookingClass ("M", 20.0, 3.0);
  BOOST_REQUIRE_EQUAL (lY.getGeneratedDemandVector().size(), 4U);

  stdair::BookingClassList_T lList;
  lList.push_back (&lY);
  stdair::VirtualClassStruct lVC (lList);
  lVC.addBookingClass (lM);
  lVC.setYield (500.0);
  lVC.setMean (30.0);
  lVC.setStdDev (5.0);
  lVC.setCumulatedBookingLimit (40.0);
  lVC.setCumulatedProtection (12.0);
  BOOST_CHECK_EQUAL (lVC.getBookingClassList().size(), 2U);
  BOOST_CHECK_EQUAL (lVC.getYield(), 500.0);
  BOOST_CHECK_EQUAL (lVC.getMean(), 30.0);
  BOOST_CHECK_EQUAL (lVC.getStdDev(), 5.0);
  BOOST_CHECK_EQUAL (lVC.getCumulatedBookingLimit(), 40.0);
  BOOST_CHECK_EQUAL (lVC.getCumulatedProtection(), 12.0);
  BOOST_CHECK_EQUAL (lVC.describe(), "Yield: 500, Demand N (30, 5)");
  BOOST_CHECK_EQUAL (streamed (lVC), lVC.describe());

  // Aggregated demand = sum of the booking class demand samples
  const stdair::GeneratedDemandVector_T lDemand =
    lVC.getGeneratedDemandVector();
  BOOST_REQUIRE_EQUAL (lDemand.size(), 4U);
  for (unsigned int i = 0; i < 4; ++i) {
    BOOST_CHECK_CLOSE (lDemand[i], lY.getGeneratedDemandVector()[i]
                       + lM.getGeneratedDemandVector()[i], 1e-9);
  }

  // Copy constructor (only list, yield, mean and std dev are copied)
  const stdair::VirtualClassStruct lCopy (lVC);
  BOOST_CHECK_EQUAL (lCopy.describe(), lVC.describe());
  BOOST_CHECK_EQUAL (lCopy.getBookingClassList().size(), 2U);

  // Empty booking class list: no demand
  const stdair::BookingClassList_T lEmptyList;
  const stdair::VirtualClassStruct lEmptyVC (lEmptyList);
  BOOST_CHECK (lEmptyVC.getGeneratedDemandVector().empty());
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (fare_and_yield_features_test) {
  // Round-trip fare, 7-day advance purchase, 3-day minimum stay
  const stdair::FareFeaturesKey lRTKey (stdair::TRIP_TYPE_ROUND_TRIP, 7, true,
                                        true, false, 3);
  stdair::FareFeatures& lRT =
    stdair::FacBom<stdair::FareFeatures>::instance().create (lRTKey);
  BOOST_CHECK (lRT.getParent() == NULL);
  BOOST_CHECK (lRT.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lRT.getKey().toString(), "RT -- 7-1-1-0-3");
  BOOST_CHECK_EQUAL (lRT.toString(), lRT.describeKey());
  BOOST_CHECK_EQUAL (streamed (lRT), lRT.describeKey());
  BOOST_CHECK_EQUAL (lRT.getTripType(), stdair::TRIP_TYPE_ROUND_TRIP);
  BOOST_CHECK_EQUAL (lRT.getAdvancePurchase(), 7);
  BOOST_CHECK_EQUAL (lRT.getSaturdayStay(), true);
  BOOST_CHECK_EQUAL (lRT.getChangeFees(), true);
  BOOST_CHECK_EQUAL (lRT.getRefundableOption(), false);
  BOOST_CHECK_EQUAL (lRT.getMinimumStay(), 3);

  BOOST_CHECK (lRT.isTripTypeValid (stdair::TRIP_TYPE_ROUND_TRIP));
  BOOST_CHECK (lRT.isTripTypeValid (stdair::TRIP_TYPE_INBOUND));
  BOOST_CHECK (lRT.isTripTypeValid (stdair::TRIP_TYPE_OUTBOUND));
  BOOST_CHECK (!lRT.isTripTypeValid (stdair::TRIP_TYPE_ONE_WAY));
  BOOST_CHECK (!lRT.isStayDurationValid (2));
  BOOST_CHECK (lRT.isStayDurationValid (3));

  const stdair::DateTime_T lFlight (stdair::Date_T (2011, 6, 10),
                                    stdair::Duration_T (8, 0, 0));
  const stdair::DateTime_T lEarly (stdair::Date_T (2011, 5, 1),
                                   stdair::Duration_T (8, 0, 0));
  const stdair::DateTime_T lLate (stdair::Date_T (2011, 6, 8),
                                  stdair::Duration_T (8, 0, 0));
  BOOST_CHECK (lRT.isAdvancePurchaseValid (lEarly, lFlight));
  BOOST_CHECK (!lRT.isAdvancePurchaseValid (lLate, lFlight));

  // One-way fare does not match inbound requests
  const stdair::FareFeaturesKey lOWKey (stdair::TRIP_TYPE_ONE_WAY, 0, false,
                                        false, true, 0);
  stdair::FareFeatures& lOW =
    stdair::FacBom<stdair::FareFeatures>::instance().create (lOWKey);
  BOOST_CHECK (!lOW.isTripTypeValid (stdair::TRIP_TYPE_INBOUND));
  const stdair::FareFeaturesKey lKeyCopy (lOWKey);
  BOOST_CHECK_EQUAL (streamed (lKeyCopy),
                     "FareFeaturesKey: " + lOWKey.toString() + "\n");

  // Yield features
  const stdair::YieldFeaturesKey lYRTKey (stdair::TRIP_TYPE_ROUND_TRIP, "Y");
  stdair::YieldFeatures& lYRT =
    stdair::FacBom<stdair::YieldFeatures>::instance().create (lYRTKey);
  BOOST_CHECK (lYRT.getParent() == NULL);
  BOOST_CHECK (lYRT.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lYRT.getCabinCode(), "Y");
  BOOST_CHECK_EQUAL (lYRT.getTripType(), stdair::TRIP_TYPE_ROUND_TRIP);
  BOOST_CHECK_EQUAL (lYRT.describeKey(), "RT -- Y");
  BOOST_CHECK_EQUAL (lYRT.toString(), "RT -- Y");
  BOOST_CHECK_EQUAL (streamed (lYRT), "RT -- Y");
  BOOST_CHECK_EQUAL (lYRT.getKey().getCabinCode(), "Y");
  BOOST_CHECK (lYRT.isTripTypeValid (stdair::TRIP_TYPE_ROUND_TRIP));
  BOOST_CHECK (lYRT.isTripTypeValid (stdair::TRIP_TYPE_OUTBOUND));
  BOOST_CHECK (!lYRT.isTripTypeValid (stdair::TRIP_TYPE_ONE_WAY));
  const stdair::YieldFeaturesKey lYOWKey (stdair::TRIP_TYPE_ONE_WAY, "M");
  stdair::YieldFeatures& lYOW =
    stdair::FacBom<stdair::YieldFeatures>::instance().create (lYOWKey);
  BOOST_CHECK (!lYOW.isTripTypeValid (stdair::TRIP_TYPE_INBOUND));
  const stdair::YieldFeaturesKey lYKeyCopy (lYOWKey);
  BOOST_CHECK_EQUAL (streamed (lYKeyCopy), "YieldFeaturesKey: OW -- M\n");
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (period_and_pair_bom_test) {
  // Date period
  const stdair::DatePeriod_T lRange (stdair::Date_T (2011, 1, 1),
                                     stdair::Date_T (2011, 2, 1));
  const stdair::DatePeriodKey lDPKey (lRange);
  stdair::DatePeriod& lDP =
    stdair::FacBom<stdair::DatePeriod>::instance().create (lDPKey);
  BOOST_CHECK (lDP.getParent() == NULL);
  BOOST_CHECK (lDP.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lDP.getDatePeriod(), lRange);
  BOOST_CHECK_EQUAL (lDP.describeKey(), "[2011-Jan-01/2011-Feb-01]");
  BOOST_CHECK_EQUAL (lDP.toString(), lDP.describeKey());
  BOOST_CHECK_EQUAL (streamed (lDP), lDP.describeKey());
  BOOST_CHECK_EQUAL (lDP.getKey().getDatePeriod(), lRange);
  BOOST_CHECK (lDP.isDepartureDateValid (stdair::Date_T (2011, 1, 15)));
  BOOST_CHECK (!lDP.isDepartureDateValid (stdair::Date_T (2011, 3, 1)));
  const stdair::DatePeriodKey lDPKeyCopy (lDPKey);
  BOOST_CHECK_EQUAL (streamed (lDPKeyCopy),
                     "DatePeriodKey: [2011-Jan-01/2011-Feb-01]\n");

  // Time period
  const stdair::TimePeriodKey lTPKey (stdair::Duration_T (0, 0, 0),
                                      stdair::Duration_T (23, 0, 0));
  stdair::TimePeriod& lTP =
    stdair::FacBom<stdair::TimePeriod>::instance().create (lTPKey);
  BOOST_CHECK (lTP.getParent() == NULL);
  BOOST_CHECK (lTP.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lTP.getTimeRangeStart(), stdair::Duration_T (0, 0, 0));
  BOOST_CHECK_EQUAL (lTP.getTimeRangeEnd(), stdair::Duration_T (23, 0, 0));
  BOOST_CHECK_EQUAL (lTP.describeKey(), "00:00:00-23:00:00");
  BOOST_CHECK_EQUAL (lTP.toString(), "00:00:00-23:00:00");
  BOOST_CHECK_EQUAL (streamed (lTP), "00:00:00-23:00:00");
  BOOST_CHECK_EQUAL (lTP.getKey().getTimeRangeEnd(),
                     stdair::Duration_T (23, 0, 0));
  const stdair::TimePeriodKey lTPKeyCopy (lTPKey);
  BOOST_CHECK_EQUAL (streamed (lTPKeyCopy),
                     "TimePeriodKey: 00:00:00-23:00:00\n");

  // Airport pair
  const stdair::AirportPairKey lAPKey ("LHR", "SYD");
  stdair::AirportPair& lAP =
    stdair::FacBom<stdair::AirportPair>::instance().create (lAPKey);
  BOOST_CHECK (lAP.getParent() == NULL);
  BOOST_CHECK (lAP.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lAP.getBoardingPoint(), "LHR");
  BOOST_CHECK_EQUAL (lAP.getOffPoint(), "SYD");
  BOOST_CHECK_EQUAL (lAP.describeKey(), "LHR, SYD");
  BOOST_CHECK_EQUAL (lAP.toString(), "LHR, SYD");
  BOOST_CHECK_EQUAL (streamed (lAP), "LHR, SYD");
  BOOST_CHECK_EQUAL (lAP.getKey().getOffPoint(), "SYD");
  const stdair::AirportPairKey lAPKeyCopy (lAPKey);
  BOOST_CHECK_EQUAL (streamed (lAPKeyCopy), "AirportPairKey: LHR, SYD\n");

  // POS / channel
  const stdair::PosChannelKey lPCKey ("NCE", "IN");
  stdair::PosChannel& lPC =
    stdair::FacBom<stdair::PosChannel>::instance().create (lPCKey);
  BOOST_CHECK (lPC.getParent() == NULL);
  BOOST_CHECK (lPC.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lPC.getPos(), "NCE");
  BOOST_CHECK_EQUAL (lPC.getChannel(), "IN");
  BOOST_CHECK_EQUAL (lPC.describeKey(), "NCE, IN");
  BOOST_CHECK_EQUAL (lPC.toString(), "NCE, IN");
  BOOST_CHECK_EQUAL (streamed (lPC), "NCE, IN");
  BOOST_CHECK_EQUAL (lPC.getKey().getPos(), "NCE");
  const stdair::PosChannelKey lPCKeyCopy (lPCKey);
  BOOST_CHECK_EQUAL (streamed (lPCKeyCopy), "PosChannelKey: NCE, IN\n");

  // Flight period
  const stdair::PeriodStruct lPeriod (lRange, stdair::DoWStruct ("1111111"));
  const stdair::FlightPeriodKey lFPKey (9, lPeriod);
  stdair::FlightPeriod& lFP =
    stdair::FacBom<stdair::FlightPeriod>::instance().create (lFPKey);
  BOOST_CHECK (lFP.getParent() == NULL);
  BOOST_CHECK (lFP.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lFP.getFlightNumber(), 9);
  BOOST_CHECK_EQUAL (lFP.getPeriod().getDoW().describeShort(), "1111111");
  BOOST_CHECK (contains (lFP.toString(), "9, "));
  BOOST_CHECK_EQUAL (streamed (lFP), lFP.toString());
  const stdair::FlightPeriodKey lFPKeyCopy (lFPKey);
  BOOST_CHECK_EQUAL (streamed (lFPKeyCopy),
                     "FlightPeriodKey: " + lFPKey.toString() + "\n");

  // Segment period
  const stdair::SegmentPeriodKey lSPKey ("LHR", "SYD");
  stdair::SegmentPeriod& lSP =
    stdair::FacBom<stdair::SegmentPeriod>::instance().create (lSPKey);
  lSP.setBoardingTime (stdair::Duration_T (21, 45, 0));
  lSP.setOffTime (stdair::Duration_T (6, 30, 0));
  lSP.setBoardingDateOffset (stdair::DateOffset_T (0));
  lSP.setOffDateOffset (stdair::DateOffset_T (2));
  lSP.setElapsedTime (stdair::Duration_T (22, 45, 0));
  lSP.addCabinBookingClassList ("Y", "YMQ");
  BOOST_CHECK (lSP.getParent() == NULL);
  BOOST_CHECK (lSP.getHolderMap().empty());
  BOOST_CHECK_EQUAL (lSP.getBoardingPoint(), "LHR");
  BOOST_CHECK_EQUAL (lSP.getOffPoint(), "SYD");
  BOOST_CHECK_EQUAL (lSP.getBoardingTime(), stdair::Duration_T (21, 45, 0));
  BOOST_CHECK_EQUAL (lSP.getOffTime(), stdair::Duration_T (6, 30, 0));
  BOOST_CHECK_EQUAL (lSP.getBoardingDateOffset().days(), 0);
  BOOST_CHECK_EQUAL (lSP.getOffDateOffset().days(), 2);
  BOOST_CHECK_EQUAL (lSP.getElapsedTime(), stdair::Duration_T (22, 45, 0));
  BOOST_CHECK_EQUAL (lSP.getCabinBookingClassMap().size(), 1U);
  BOOST_CHECK_EQUAL (lSP.toString(), "LHR-SYD");
  BOOST_CHECK_EQUAL (streamed (lSP), "LHR-SYD");
  const stdair::SegmentPeriodKey lSPKeyCopy (lSPKey);
  BOOST_CHECK_EQUAL (streamed (lSPKeyCopy), "SegmentPeriodKey: LHR-SYD\n");
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (simple_keys_test) {
  const stdair::AirlineFeatureKey lAFKey ("BA");
  BOOST_CHECK_EQUAL (lAFKey.getAirlineCode(), "BA");
  BOOST_CHECK_EQUAL (streamed (lAFKey), "AirlineFeatureKey: BA\n");

  const stdair::YieldStoreKey lYSKey ("AF");
  BOOST_CHECK_EQUAL (lYSKey.getAirlineCode(), "AF");
  BOOST_CHECK_EQUAL (lYSKey.toString(), "AF");
  const stdair::YieldStoreKey lYSKeyCopy (lYSKey);
  BOOST_CHECK_EQUAL (streamed (lYSKeyCopy), "YieldStoreKey: AF\n");

  const stdair::BomHolderKey lHolderKey;
  BOOST_CHECK_EQUAL (lHolderKey.toString(), " -- HOLDER -- ");
  BOOST_CHECK_EQUAL (streamed (lHolderKey), "BomHolderKey:  -- HOLDER -- \n");

  // OnD date key
  stdair::OnDStringList_T lOnDList;
  lOnDList.push_back (BA9_KEY);
  lOnDList.push_back ("BA, 15, 2011-06-11, SYD, AKL, 08:00");
  const stdair::OnDDateKey lOnDKey (lOnDList);
  BOOST_CHECK_EQUAL (lOnDKey.getNbOfSegments(), 2);
  BOOST_CHECK_EQUAL (lOnDKey.getDate(), stdair::Date_T (2011, 6, 10));
  BOOST_CHECK_EQUAL (lOnDKey.getOrigin(), "LHR");
  BOOST_CHECK_EQUAL (lOnDKey.getDestination(), "AKL");
  const stdair::OnDDateKey lOnDKeyCopy (lOnDKey);
  BOOST_CHECK_EQUAL (streamed (lOnDKeyCopy), "OnDDateKey: "
                     + BA9_KEY + " BA, 15, 2011-06-11, SYD, AKL, 08:00 ");
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (other_keys_test) {
  const stdair::BomRootKey lRootKey ("MyRoot");
  BOOST_CHECK_EQUAL (lRootKey.getID(), "MyRoot");
  BOOST_CHECK_EQUAL (streamed (lRootKey), "BomRootKey: MyRoot\n");
  const stdair::BomRootKey lRootKeyCopy (lRootKey);
  BOOST_CHECK_EQUAL (lRootKeyCopy.toString(), "MyRoot");

  const stdair::InventoryKey lInvKey ("BA");
  BOOST_CHECK_EQUAL (streamed (lInvKey), "InventoryKey: BA");

  const stdair::FlightDateKey lFDKey (9, stdair::Date_T (2011, 6, 10));
  BOOST_CHECK_EQUAL (streamed (lFDKey), "FlightDateKey: 9, 2011-06-10");

  const stdair::FareFamilyKey lFFKey ("EcoFlex");
  BOOST_CHECK_EQUAL (lFFKey.getFamilyCode(), "EcoFlex");
  BOOST_CHECK_EQUAL (streamed (lFFKey), "FareFamilyKey: EcoFlex");
  const stdair::FareFamilyKey lFFKeyCopy (lFFKey);
  BOOST_CHECK_EQUAL (lFFKeyCopy.toString(), "EcoFlex");
}

BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
