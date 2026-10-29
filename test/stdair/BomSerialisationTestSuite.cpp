/*!
 * \page BomSerialisationTestSuite_cpp Unit tests for the BOM JSON/INI/config serialisation and display
 * \code
 */
// STL
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
// Boost
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/archive/archive_exception.hpp>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_file.hpp>
#include <stdair/stdair_json.hpp>
#include <stdair/STDAIR_Service.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/JSonCommand.hpp>
#include <stdair/basic/EventType.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/bom/BomArchive.hpp>
#include <stdair/bom/BomDisplay.hpp>
#include <stdair/bom/BomINIImport.hpp>
#include <stdair/bom/BomJSONExport.hpp>
#include <stdair/bom/BomJSONImport.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/Bucket.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/bom/ConfigHolderStruct.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/FFDisutilityCurveHolderStruct.hpp>
#include <stdair/bom/FRAT5CurveHolderStruct.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/OnDDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/factory/FacBom.hpp>
#include <stdair/factory/FacBomManager.hpp>

namespace bpt = boost::property_tree;

namespace {

  // Kept alive for the whole process: the StdAir Logger singleton keeps a
  // pointer to the last stream it was given.
  std::ofstream& serialisationLogStream() {
    static std::ofstream lLogStream ("BomSerialisationTestSuite.log");
    return lLogStream;
  }

  struct ServiceFixture {
    ServiceFixture()
      : _logParams (stdair::LOG::DEBUG, serialisationLogStream()),
        _service (_logParams) {
    }
    stdair::BomRoot& root() { return _service.getPersistentBomRoot(); }
    stdair::BasLogParams _logParams;
    stdair::STDAIR_Service _service;
  };

  bpt::ptree parseJSON (const std::string& iJSON) {
    bpt::ptree oPT;
    std::istringstream lStr (iJSON);
    bpt::read_json (lStr, oPT);
    return oPT;
  }

  const stdair::Date_T kBA9Date (2011, boost::gregorian::Jun, 10);

  stdair::FlightDate& getFlightDate (const stdair::BomRoot& iRoot,
                                     const stdair::AirlineCode_T& iAirline,
                                     const stdair::FlightNumber_T& iNumber,
                                     const stdair::Date_T& iDate) {
    stdair::FlightDate* lFD_ptr =
      stdair::BomRetriever::retrieveFlightDateFromKeySet (iRoot, iAirline,
                                                          iNumber, iDate);
    BOOST_REQUIRE (lFD_ptr != NULL);
    return *lFD_ptr;
  }

  std::string writeTempFile (const std::string& iName,
                             const std::string& iContent) {
    std::ofstream lFile (iName.c_str());
    lFile << iContent;
    lFile.close();
    return iName;
  }
}

BOOST_AUTO_TEST_SUITE (bom_serialisation_test_suite)

// ////////////////////////////////////////////////////////////////////
// BomJSONExport
// ////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE (json_export_flight_date_list_test) {
  ServiceFixture lFixture;

  // An empty BOM tree produces no output at all
  std::ostringstream lEmptyStr;
  stdair::BomJSONExport::jsonExportFlightDateList (lEmptyStr, lFixture.root());
  BOOST_CHECK (lEmptyStr.str().empty());

  lFixture._service.buildSampleBom();
  const stdair::BomRoot& lRoot = lFixture.root();

  // All the inventories / flight-dates
  std::ostringstream lAllStr;
  stdair::BomJSONExport::jsonExportFlightDateList (lAllStr, lRoot);
  const bpt::ptree lAllPT = parseJSON (lAllStr.str());
  const bpt::ptree& lAllInv = lAllPT.get_child ("inventories");
  BOOST_CHECK (lAllInv.size() >= 4);
  bool hasBA9 = false;
  for (bpt::ptree::const_iterator itFD = lAllInv.begin();
       itFD != lAllInv.end(); ++itFD) {
    if (itFD->second.get<std::string> ("airline_code") == "BA"
        && itFD->second.get<unsigned short> ("number") == 9) {
      hasBA9 = true;
      BOOST_CHECK_EQUAL (itFD->second.get<std::string> ("date"), "2011-Jun-10");
    }
  }
  BOOST_CHECK (hasBA9);

  // Only the BA inventory, then only BA9
  std::ostringstream lBAStr;
  stdair::BomJSONExport::jsonExportFlightDateList (lBAStr, lRoot, "BA");
  const bpt::ptree lBAPT = parseJSON (lBAStr.str());
  const bpt::ptree& lBAInv = lBAPT.get_child ("inventories");
  BOOST_CHECK (lBAInv.size() >= 1);
  for (bpt::ptree::const_iterator itFD = lBAInv.begin();
       itFD != lBAInv.end(); ++itFD) {
    BOOST_CHECK_EQUAL (itFD->second.get<std::string> ("airline_code"), "BA");
  }

  std::ostringstream lBA9Str;
  stdair::BomJSONExport::jsonExportFlightDateList (lBA9Str, lRoot, "BA", 9);
  const bpt::ptree lBA9PT = parseJSON (lBA9Str.str());
  BOOST_CHECK_EQUAL (lBA9PT.get_child ("inventories").size(), 1u);

  // Unknown airline: an empty inventory list
  std::ostringstream lNoneStr;
  stdair::BomJSONExport::jsonExportFlightDateList (lNoneStr, lRoot, "ZZ");
  const bpt::ptree lNonePT = parseJSON (lNoneStr.str());
  BOOST_CHECK (lNonePT.get_child ("inventories").empty());

  // An inventory without flight-date: the export stops early
  stdair::BomRoot& lOtherRoot =
    stdair::FacBom<stdair::BomRoot>::instance().create();
  const stdair::InventoryKey lXXKey ("XX");
  stdair::Inventory& lXXInv =
    stdair::FacBom<stdair::Inventory>::instance().create (lXXKey);
  stdair::FacBomManager::addToListAndMap (lOtherRoot, lXXInv);
  std::ostringstream lNoFDStr;
  stdair::BomJSONExport::jsonExportFlightDateList (lNoFDStr, lOtherRoot);
  BOOST_CHECK (lNoFDStr.str().empty());

  // Same through the service (on the clone BOM tree)
  lFixture._service.clonePersistentBom();
  const std::string lServiceJSON =
    lFixture._service.jsonExportFlightDateList ("BA", 9);
  BOOST_CHECK_EQUAL (lServiceJSON, lBA9Str.str());
}

BOOST_AUTO_TEST_CASE (json_export_flight_date_objects_round_trip_test) {
  ServiceFixture lFixture;
  lFixture._service.buildSampleBom();
  const stdair::BomRoot& lRoot = lFixture.root();

  const stdair::FlightDate& lBA9 = getFlightDate (lRoot, "BA", 9, kBA9Date);
  std::ostringstream lStr;
  stdair::BomJSONExport::jsonExportFlightDateObjects (lStr, lBA9);
  const std::string lJSON (lStr.str());

  // Structure of the exported tree
  const bpt::ptree lPT = parseJSON (lJSON);
  BOOST_CHECK_EQUAL (lPT.get<std::string> ("flight_date.airline_code"), "BA");
  BOOST_CHECK_EQUAL (lPT.get<unsigned short> ("flight_date.flight_number"), 9);
  BOOST_CHECK_EQUAL (lPT.get<std::string> ("flight_date.departure_date"),
                     "2011-Jun-10");
  const bpt::ptree& lLegs = lPT.get_child ("flight_date.legs");
  const bpt::ptree& lSegments = lPT.get_child ("flight_date.segments");
  BOOST_CHECK_EQUAL (lLegs.size(),
                     stdair::BomManager::getList<stdair::LegDate> (lBA9).size());
  BOOST_CHECK_EQUAL (lSegments.size(),
                     stdair::BomManager::getList<stdair::SegmentDate> (lBA9).size());
  BOOST_CHECK (lJSON.find ("\"board_point\": \"LHR\"") != std::string::npos);
  BOOST_CHECK (lJSON.find ("\"buckets\"") != std::string::npos);
  BOOST_CHECK (lJSON.find ("\"sub_classes\"") != std::string::npos);

  // Round-trip of the flight-date keys through the importer
  const stdair::JSONString lJSONString (lJSON);
  stdair::AirlineCode_T lAirlineCode;
  BOOST_CHECK (stdair::BomJSONImport::jsonImportInventoryKey (lJSONString,
                                                              lAirlineCode));
  BOOST_CHECK_EQUAL (lAirlineCode, "BA");
  stdair::FlightNumber_T lFlightNumber = 0;
  BOOST_CHECK (stdair::BomJSONImport::jsonImportFlightNumber (lJSONString,
                                                              lFlightNumber));
  BOOST_CHECK_EQUAL (lFlightNumber, 9);
  stdair::Date_T lDate;
  BOOST_CHECK (stdair::BomJSONImport::jsonImportFlightDate (lJSONString,
                                                            lDate));
  BOOST_CHECK_EQUAL (lDate, kBA9Date);

  // Export every flight-date of the sample BOM (incl. the fare-family ones)
  const stdair::InventoryList_T& lInvList =
    stdair::BomManager::getList<stdair::Inventory> (lRoot);
  unsigned int lNbExported = 0;
  bool hasFareFamilies = false;
  for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
       itInv != lInvList.end(); ++itInv) {
    if (stdair::BomManager::hasList<stdair::FlightDate> (**itInv) == false) {
      continue;
    }
    const stdair::FlightDateList_T& lFDList =
      stdair::BomManager::getList<stdair::FlightDate> (**itInv);
    for (stdair::FlightDateList_T::const_iterator itFD = lFDList.begin();
         itFD != lFDList.end(); ++itFD) {
      std::ostringstream lFDStr;
      stdair::BomJSONExport::jsonExportFlightDateObjects (lFDStr, **itFD);
      const bpt::ptree lFDPT = parseJSON (lFDStr.str());
      BOOST_CHECK_EQUAL (lFDPT.get<stdair::FlightNumber_T>
                         ("flight_date.flight_number"),
                         (*itFD)->getFlightNumber());
      if (lFDStr.str().find ("\"family_code\"") != std::string::npos) {
        hasFareFamilies = true;
      }
      ++lNbExported;
    }
  }
  BOOST_CHECK (lNbExported >= 4);
  BOOST_CHECK (hasFareFamilies);

  // Through the service: existing and missing flight-dates
  lFixture._service.clonePersistentBom();
  BOOST_CHECK_EQUAL (lFixture._service.jsonExportFlightDateObjects ("BA", 9,
                                                                    kBA9Date),
                     lJSON);
  const std::string lMissing =
    lFixture._service.jsonExportFlightDateObjects ("BA", 9999, kBA9Date);
  BOOST_CHECK (parseJSON (lMissing).get<std::string> ("error").find ("BA9999")
               != std::string::npos);
}

BOOST_AUTO_TEST_CASE (json_export_event_objects_test) {
  ServiceFixture lFixture;

  // Booking request event
  const stdair::BookingRequestPtr_T lBookingRequest_ptr =
    boost::make_shared<stdair::BookingRequestStruct>
    (lFixture._service.buildSampleBookingRequest());
  const stdair::EventStruct lBkgEvent (stdair::EventType::BKG_REQ,
                                       lBookingRequest_ptr);
  std::ostringstream lBkgStr;
  stdair::BomJSONExport::jsonExportBookingRequestObject (lBkgStr, lBkgEvent);
  const bpt::ptree lBkgPT = parseJSON (lBkgStr.str());
  BOOST_CHECK_EQUAL (lBkgPT.get<std::string> ("event_type"), "BookingRequest");
  BOOST_CHECK_EQUAL (lBkgPT.get<std::string> ("org"),
                     lBookingRequest_ptr->getOrigin());
  BOOST_CHECK_EQUAL (lBkgPT.get<std::string> ("des"),
                     lBookingRequest_ptr->getDestination());
  BOOST_CHECK_EQUAL (lBkgPT.get<unsigned short> ("pax"),
                     lBookingRequest_ptr->getPartySize());
  BOOST_CHECK_EQUAL (lBkgPT.get<std::string> ("dep_date"), "2011-Jun-10");
  BOOST_CHECK_EQUAL (lBkgPT.get<std::string> ("bkg_date"), "2011-May-15");
  BOOST_CHECK_EQUAL (lBkgPT.get<std::string> ("cancel_date"), "xxxx-xx-xx");
  BOOST_CHECK_EQUAL (lFixture._service.jsonExportEventObject (lBkgEvent),
                     lBkgStr.str());

  // Break-point event
  const stdair::DateTime_T lBPTime (kBA9Date,
                                    stdair::Duration_T (12, 30, 0));
  const stdair::BreakPointPtr_T lBreakPoint_ptr =
    boost::make_shared<stdair::BreakPointStruct> (lBPTime);
  const stdair::EventStruct lBPEvent (stdair::EventType::BRK_PT,
                                      lBreakPoint_ptr);
  std::ostringstream lBPStr;
  stdair::BomJSONExport::jsonExportBreakPointObject (lBPStr, lBPEvent);
  const bpt::ptree lBPPT = parseJSON (lBPStr.str());
  BOOST_CHECK_EQUAL (lBPPT.get<std::string> ("event_type"), "BreakPoint");
  BOOST_CHECK (lBPPT.get<std::string> ("time_stamp").find ("12:30:00")
               != std::string::npos);
  BOOST_CHECK_EQUAL (lFixture._service.jsonExportEventObject (lBPEvent),
                     lBPStr.str());

  // Round-trip of the event type, wrapped in a command object
  std::ostringstream lWrapped;
  lWrapped << "{\"event\":" << lBPStr.str() << "}";
  stdair::EventType::EN_EventType lEventType = stdair::EventType::LAST_VALUE;
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportEventType (stdair::JSONString (lWrapped.str()),
                                    lEventType));
  BOOST_CHECK_EQUAL (lEventType, stdair::EventType::BRK_PT);
}

BOOST_AUTO_TEST_CASE (json_export_hand_built_flight_date_test) {
  ServiceFixture lFixture;

  // Flight-date with a leg-cabin holding buckets and bid-prices, and a
  // segment-cabin holding booking-classes directly (no fare family)
  stdair::BomRoot& lRoot = lFixture.root();
  stdair::Inventory& lInv =
    stdair::FacBom<stdair::Inventory>::instance().create (stdair::InventoryKey ("XX"));
  stdair::FacBomManager::addToListAndMap (lRoot, lInv);
  stdair::FacBomManager::linkWithParent (lRoot, lInv);
  stdair::FlightDate& lFD = stdair::FacBom<stdair::FlightDate>::instance().
    create (stdair::FlightDateKey (101, kBA9Date));
  stdair::FacBomManager::addToListAndMap (lInv, lFD);
  stdair::FacBomManager::linkWithParent (lInv, lFD);

  stdair::LegDate& lLD = stdair::FacBom<stdair::LegDate>::instance().
    create (stdair::LegDateKey ("AAA"));
  stdair::FacBomManager::addToListAndMap (lFD, lLD);
  stdair::FacBomManager::linkWithParent (lFD, lLD);
  stdair::LegCabin& lLC = stdair::FacBom<stdair::LegCabin>::instance().
    create (stdair::LegCabinKey ("Y"));
  stdair::FacBomManager::addToListAndMap (lLD, lLC);
  stdair::FacBomManager::linkWithParent (lLD, lLC);
  lLC.getBidPriceVector().push_back (120.5);
  lLC.getBidPriceVector().push_back (80.0);
  for (stdair::SeatIndex_T idx = 1; idx <= 2; ++idx) {
    stdair::Bucket& lBucket = stdair::FacBom<stdair::Bucket>::instance().
      create (stdair::BucketKey (idx));
    stdair::FacBomManager::addToListAndMap (lLC, lBucket);
    stdair::FacBomManager::linkWithParent (lLC, lBucket);
  }

  stdair::SegmentDate& lSD = stdair::FacBom<stdair::SegmentDate>::instance().
    create (stdair::SegmentDateKey ("AAA", "BBB"));
  stdair::FacBomManager::addToListAndMap (lFD, lSD);
  stdair::FacBomManager::linkWithParent (lFD, lSD);
  stdair::SegmentCabin& lSC = stdair::FacBom<stdair::SegmentCabin>::instance().
    create (stdair::SegmentCabinKey ("Y"));
  stdair::FacBomManager::addToListAndMap (lSD, lSC);
  stdair::FacBomManager::linkWithParent (lSD, lSC);
  stdair::BookingClass& lBC = stdair::FacBom<stdair::BookingClass>::instance().
    create (stdair::BookingClassKey ("M"));
  stdair::FacBomManager::addToListAndMap (lSC, lBC);
  stdair::FacBomManager::linkWithParent (lSC, lBC);

  std::ostringstream lStr;
  stdair::BomJSONExport::jsonExportFlightDateObjects (lStr, lFD);
  const std::string lJSON (lStr.str());
  const bpt::ptree lPT = parseJSON (lJSON);
  BOOST_CHECK_EQUAL (lPT.get<std::string> ("flight_date.airline_code"), "XX");
  BOOST_CHECK (lJSON.find ("120.5,80") != std::string::npos);
  BOOST_CHECK (lJSON.find ("\"si\": \"2\"") != std::string::npos);
  BOOST_CHECK (lJSON.find ("\"family_code\": \"NoFF\"") != std::string::npos);

  // The CSV display of the same flight-date
  std::ostringstream lCSV;
  stdair::BomDisplay::csvDisplay (lCSV, lFD);
  BOOST_CHECK (lCSV.str().find ("XX101") != std::string::npos);
}

// ////////////////////////////////////////////////////////////////////
// BomJSONImport
// ////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE (json_import_command_test) {
  const char* lCommands[] = { "list", "flight_date", "event_list",
                              "break_point", "run", "reset", "status",
                              "config" };
  for (unsigned short idx = 0; idx != stdair::JSonCommand::LAST_VALUE; ++idx) {
    std::ostringstream lStr;
    lStr << "{\"" << lCommands[idx] << "\":{\"airline_code\":\"BA\"}}";
    stdair::JSonCommand::EN_JSonCommand lCommand =
      stdair::JSonCommand::LAST_VALUE;
    BOOST_CHECK (stdair::BomJSONImport::
                 jsonImportCommand (stdair::JSONString (lStr.str()), lCommand));
    BOOST_CHECK_EQUAL (lCommand, idx);
  }

  // Array-shaped details are accepted too
  stdair::JSonCommand::EN_JSonCommand lCommand = stdair::JSonCommand::LAST_VALUE;
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportCommand (stdair::JSONString
                                  ("{\"break_point\":[{\"bp\":\"2011-01-01\"}]}"),
                                  lCommand));
  BOOST_CHECK_EQUAL (lCommand, stdair::JSonCommand::BREAK_POINT);

  // Not a command
  lCommand = stdair::JSonCommand::LIST;
  BOOST_CHECK (!stdair::BomJSONImport::
               jsonImportCommand (stdair::JSONString ("not json"), lCommand));
  BOOST_CHECK_EQUAL (lCommand, stdair::JSonCommand::LIST);
  // Unknown command name
  BOOST_CHECK (!stdair::BomJSONImport::
               jsonImportCommand (stdair::JSONString ("{\"fly\":{}}"),
                                  lCommand));
}

BOOST_AUTO_TEST_CASE (json_import_keys_test) {
  const stdair::JSONString
    lList ("{\"list\":{\"airline_code\":\"AF\",\"flight_number\":\"1234\"}}");
  stdair::AirlineCode_T lAirlineCode;
  BOOST_CHECK (stdair::BomJSONImport::jsonImportInventoryKey (lList,
                                                              lAirlineCode));
  BOOST_CHECK_EQUAL (lAirlineCode, "AF");
  stdair::FlightNumber_T lFlightNumber = 0;
  BOOST_CHECK (stdair::BomJSONImport::jsonImportFlightNumber (lList,
                                                              lFlightNumber));
  BOOST_CHECK_EQUAL (lFlightNumber, 1234);

  // Missing keys / malformed JSON
  const stdair::JSONString lEmpty ("{\"list\":{}}");
  const stdair::JSONString lBroken ("{\"list\":");
  BOOST_CHECK (!stdair::BomJSONImport::jsonImportInventoryKey (lEmpty,
                                                               lAirlineCode));
  BOOST_CHECK (!stdair::BomJSONImport::jsonImportInventoryKey (lBroken,
                                                               lAirlineCode));
  BOOST_CHECK (!stdair::BomJSONImport::jsonImportFlightNumber (lEmpty,
                                                               lFlightNumber));
  const stdair::JSONString
    lBadNumber ("{\"list\":{\"flight_number\":\"abc\"}}");
  BOOST_CHECK (!stdair::BomJSONImport::jsonImportFlightNumber (lBadNumber,
                                                               lFlightNumber));
  BOOST_CHECK_EQUAL (lFlightNumber, 1234);

  stdair::Date_T lDate;
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportFlightDate (stdair::JSONString
                                     ("{\"flight_date\":{\"departure_date\":"
                                      "\"2012-03-04\"}}"), lDate));
  BOOST_CHECK_EQUAL (lDate, stdair::Date_T (2012, boost::gregorian::Mar, 4));
  BOOST_CHECK (!stdair::BomJSONImport::jsonImportFlightDate (lEmpty, lDate));

  // Event types
  stdair::EventType::EN_EventType lEventType = stdair::EventType::LAST_VALUE;
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportEventType (stdair::JSONString
                                    ("{\"e\":{\"event_type\":\"BookingRequest\"}}"),
                                    lEventType));
  BOOST_CHECK_EQUAL (lEventType, stdair::EventType::BKG_REQ);
  BOOST_CHECK (!stdair::BomJSONImport::
               jsonImportEventType (stdair::JSONString
                                    ("{\"e\":{\"event_type\":\"Unknown\"}}"),
                                    lEventType));
  BOOST_CHECK (!stdair::BomJSONImport::jsonImportEventType (lEmpty,
                                                            lEventType));
  BOOST_CHECK_EQUAL (lEventType, stdair::EventType::BKG_REQ);
}

BOOST_AUTO_TEST_CASE (json_import_break_points_test) {
  stdair::BreakPointList_T lBPList;
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportBreakPoints (stdair::JSONString
                                      ("{\"break_point\":["
                                       "{\"bp\":\"2011-01-01\"},"
                                       "{\"bp\":\"\"},"
                                       "{\"bp\":\"2011-05-14\"}]}"), lBPList));
  BOOST_REQUIRE_EQUAL (lBPList.size(), 2u);
  BOOST_CHECK_EQUAL (lBPList.front().getBreakPointTime().date(),
                     stdair::Date_T (2011, boost::gregorian::Jan, 1));
  BOOST_CHECK_EQUAL (lBPList.back().getBreakPointTime().date(),
                     stdair::Date_T (2011, boost::gregorian::May, 14));

  stdair::BreakPointList_T lOtherList;
  BOOST_CHECK (!stdair::BomJSONImport::
               jsonImportBreakPoints (stdair::JSONString ("[1,"), lOtherList));
  // An invalid month escapes as a boost::gregorian::bad_month exception
  // (neither caught nor converted into a 'false' return value).
  BOOST_CHECK_THROW (stdair::BomJSONImport::
                     jsonImportBreakPoints (stdair::JSONString
                                            ("{\"break_point\":[{\"bp\":\"2011-AB-01\"}]}"),
                                            lOtherList),
                     std::out_of_range);
  BOOST_CHECK (lOtherList.empty());
}

// ////////////////////////////////////////////////////////////////////
// ConfigHolderStruct / BomINIImport
// ////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE (config_holder_json_round_trip_test) {
  ServiceFixture lFixture;

  const std::string lConfigJSON =
    "{\"config\":{"
    "\"date\":{\"start\":\"2011-01-01\",\"end\":\"2011-12-31\"},"
    "\"random\":{\"seed\":\"120765987\"},"
    "\"demand generation\":{\"method\":\"S\"},"
    "\"runs\":{\"number\":\"5\"},"
    "\"input\":{\"schedule\":\"schedule.csv\",\"ond\":\"ond.csv\","
    "\"frat5\":\"frat5.csv\",\"ffdisutility\":\"ffdis.csv\","
    "\"yield\":\"yield.csv\",\"fare\":\"fare.csv\",\"demand\":\"demand.csv\"}"
    "}}";

  stdair::ConfigHolderStruct lHolder;
  BOOST_CHECK_EQUAL (lHolder.describe(), "Configuration Display:\n");
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportConfig (stdair::JSONString (lConfigJSON), lHolder));
  BOOST_CHECK (!stdair::BomJSONImport::
               jsonImportConfig (stdair::JSONString ("{\"config\":"), lHolder));

  stdair::Date_T lStartDate;
  BOOST_CHECK (lHolder.exportValue<stdair::Date_T> (lStartDate, "date.start"));
  BOOST_CHECK_EQUAL (lStartDate, stdair::Date_T (2011, boost::gregorian::Jan, 1));
  stdair::RandomSeed_T lSeed = 0;
  BOOST_CHECK (lHolder.exportValue<stdair::RandomSeed_T> (lSeed, "random.seed"));
  BOOST_CHECK_EQUAL (lSeed, 120765987u);
  std::string lMissing;
  BOOST_CHECK (!lHolder.exportValue<std::string> (lMissing, "no.such.path"));

  const std::string lDescription = lHolder.describe();
  BOOST_CHECK (lDescription.find ("Start date: 2011-Jan-01") != std::string::npos);
  BOOST_CHECK (lDescription.find ("End date: 2011-Dec-31") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Random Seed: 120765987") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Demand Generation method: S")
               != std::string::npos);
  BOOST_CHECK (lDescription.find ("Number Of Runs: 5") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Schedule input file: schedule.csv")
               != std::string::npos);
  BOOST_CHECK (lDescription.find ("OnD input file: ond.csv") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Frat5 input file: frat5.csv")
               != std::string::npos);
  BOOST_CHECK (lDescription.find ("FFdisutility input file: ffdis.csv")
               != std::string::npos);
  BOOST_CHECK (lDescription.find ("Yield input file: yield.csv")
               != std::string::npos);
  BOOST_CHECK (lDescription.find ("Fare input file: fare.csv") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Demand input file: demand.csv")
               != std::string::npos);
  std::ostringstream lStreamed;
  lHolder.toStream (lStreamed);
  BOOST_CHECK_EQUAL (lStreamed.str(), lDescription);
  std::istringstream lIn ("ignored");
  lHolder.fromStream (lIn);

  // JSON export, then import into a fresh holder: same content
  const std::string lExported = lHolder.jsonExport();
  stdair::ConfigHolderStruct lReimported;
  BOOST_CHECK (stdair::BomJSONImport::
               jsonImportConfig (stdair::JSONString (lExported), lReimported));
  BOOST_CHECK_EQUAL (lReimported.jsonExport(), lExported);
  BOOST_CHECK_EQUAL (lReimported.describe(), lDescription);

  // Copy constructor
  const stdair::ConfigHolderStruct lCopy (lHolder);
  BOOST_CHECK_EQUAL (lCopy.jsonExport(), lExported);

  // addValue: paths without the 'config' prefix get it added; existing
  // values are replaced
  BOOST_CHECK (lHolder.addValue ("7", "runs.number"));
  stdair::Count_T lNbRuns = 0;
  BOOST_CHECK (lHolder.exportValue<stdair::Count_T> (lNbRuns, "runs.number"));
  BOOST_CHECK_EQUAL (lNbRuns, 7u);
  BOOST_CHECK (lHolder.addValue ("42", "config.answer"));
  int lAnswer = 0;
  BOOST_CHECK (lHolder.exportValue<int> (lAnswer, "answer"));
  BOOST_CHECK_EQUAL (lAnswer, 42);
  // The bare prefix is ignored
  BOOST_CHECK (lHolder.addValue ("dropped", "config"));
  BOOST_CHECK (lHolder.describe().find ("dropped") == std::string::npos);

  // Through the service
  BOOST_CHECK (lFixture._service.jsonImportConfiguration
               (stdair::JSONString (lConfigJSON)));
  lFixture._service.importConfigValue ("9", "runs.number");
  BOOST_CHECK (lFixture._service.configDisplay().find ("Number Of Runs: 9")
               != std::string::npos);
  BOOST_CHECK (lFixture._service.jsonExportConfiguration().find ("schedule.csv")
               != std::string::npos);
}

BOOST_AUTO_TEST_CASE (config_holder_update_airline_features_test) {
  ServiceFixture lFixture;
  lFixture._service.buildSampleBom();
  stdair::BomRoot& lRoot = lFixture.root();

  stdair::AirlineFeature* lBAFeature_ptr =
    stdair::BomRetriever::retrieveAirlineFeatureFromKey (lRoot, "BA");
  BOOST_REQUIRE (lBAFeature_ptr != NULL);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getForecastingMethod(),
                     stdair::ForecastingMethod::Q_FORECASTING);

  // updateAirlineFeatures() browses the top-level nodes of the tree (i.e.,
  // 'config'), and looks up '<node>.airline_code' below the 'config.'
  // prefix: the features must therefore sit under 'config.config'.
  stdair::ConfigHolderStruct lHolder;
  lHolder.addValue ("BA", "config.config.airline_code");
  lHolder.addValue ("B", "config.config.forecasting_method");
  lHolder.addValue ("E", "config.config.unconstraining_method");
  lHolder.addValue ("r", "config.config.partnership_technique");
  lHolder.addValue ("F", "config.config.pre_optimisation_method");
  lHolder.addValue ("E", "config.config.optimisation_method");
  lHolder.updateAirlineFeatures (lRoot);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getForecastingMethod(),
                     stdair::ForecastingMethod::BASED_FORECASTING);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getUnconstrainingMethod(),
                     stdair::UnconstrainingMethod::EM);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getPartnershipTechnique(),
                     stdair::PartnershipTechnique::RAE_DA);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getPreOptimisationMethod(),
                     stdair::PreOptimisationMethod::FA);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getOptimisationMethod(),
                     stdair::OptimisationMethod::LEG_BASED_EMSR_B);

  // Unknown airline: nothing happens
  stdair::ConfigHolderStruct lUnknown;
  lUnknown.addValue ("ZZ", "config.config.airline_code");
  lUnknown.addValue ("Q", "config.config.forecasting_method");
  lUnknown.updateAirlineFeatures (lRoot);
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getForecastingMethod(),
                     stdair::ForecastingMethod::BASED_FORECASTING);

  // Wrong method code: CodeConversionException
  stdair::ConfigHolderStruct lWrong;
  lWrong.addValue ("BA", "config.config.airline_code");
  lWrong.addValue ("#", "config.config.forecasting_method");
  BOOST_CHECK_THROW (lWrong.updateAirlineFeatures (lRoot),
                     stdair::CodeConversionException);

  // Through the service (persistent BOM)
  lFixture._service.importConfigValue ("BA", "config.config.airline_code");
  lFixture._service.importConfigValue ("Q", "config.config.forecasting_method");
  lFixture._service.updateAirlineFeatures();
  BOOST_CHECK_EQUAL (lBAFeature_ptr->getForecastingMethod(),
                     stdair::ForecastingMethod::Q_FORECASTING);
}

BOOST_AUTO_TEST_CASE (ini_import_test) {
  ServiceFixture lFixture;

  const std::string lFilename =
    writeTempFile ("BomSerialisationTestSuite_config.ini",
                   "[date]\nstart = 2010-02-03\nend = 2010-04-05\n"
                   "[random]\nseed = 42\n"
                   "[runs]\nnumber = 3\n"
                   "[input]\nschedule = sched.csv\n");

  stdair::ConfigHolderStruct lHolder;
  stdair::BomINIImport::importINIConfig (lHolder,
                                         stdair::ConfigINIFile (lFilename));
  const std::string lDescription = lHolder.describe();
  BOOST_CHECK (lDescription.find ("Start date: 2010-Feb-03") != std::string::npos);
  BOOST_CHECK (lDescription.find ("End date: 2010-Apr-05") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Random Seed: 42") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Number Of Runs: 3") != std::string::npos);
  BOOST_CHECK (lDescription.find ("Schedule input file: sched.csv")
               != std::string::npos);

  // A missing file is silently ignored
  stdair::ConfigHolderStruct lEmpty;
  stdair::BomINIImport::
    importINIConfig (lEmpty, stdair::ConfigINIFile ("does/not/exist.ini"));
  BOOST_CHECK_EQUAL (lEmpty.describe(), "Configuration Display:\n");

  // Through the service
  lFixture._service.importINIConfig (stdair::ConfigINIFile (lFilename));
  BOOST_CHECK_EQUAL (lFixture._service.configDisplay(), lDescription);

  std::remove (lFilename.c_str());
}

// ////////////////////////////////////////////////////////////////////
// FRAT5 / FF disutility curve holders, AirlineFeature
// ////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE (frat5_curve_holder_test) {
  ServiceFixture lFixture;

  stdair::FRAT5CurveHolderStruct lHolder;
  BOOST_CHECK (lHolder.describe().empty());
  stdair::FRAT5Curve_T lCurve;
  lCurve[63] = 1.4;
  lCurve[0] = 2.5;
  lHolder.addCurve ("FRAT5_A", lCurve);
  stdair::FRAT5Curve_T lOther;
  lOther[7] = 3.0;
  lHolder.addCurve ("FRAT5_B", lOther);

  const stdair::FRAT5Curve_T& lRetrieved = lHolder.getFRAT5Curve ("FRAT5_A");
  BOOST_CHECK_EQUAL (lRetrieved.size(), 2u);
  BOOST_CHECK_CLOSE (lRetrieved.at (63), 1.4, 1e-9);
  // DTDs are displayed in decreasing order
  BOOST_CHECK_EQUAL (lHolder.describe(), "FRAT5_A; 63:1.4;0:2.5;\n"
                     "FRAT5_B; 7:3;\n");
  std::ostringstream lStr;
  lHolder.toStream (lStr);
  BOOST_CHECK_EQUAL (lStr.str(), lHolder.describe());
  std::istringstream lIn ("ignored");
  lHolder.fromStream (lIn);

  const stdair::FRAT5CurveHolderStruct lCopy (lHolder);
  BOOST_CHECK_EQUAL (lCopy.describe(), lHolder.describe());
}

BOOST_AUTO_TEST_CASE (ffdisutility_curve_holder_test) {
  ServiceFixture lFixture;

  stdair::FFDisutilityCurveHolderStruct lHolder;
  BOOST_CHECK (lHolder.describe().empty());
  stdair::FFDisutilityCurve_T lCurve;
  lCurve[30] = 0.5;
  lCurve[1] = 0.25;
  lHolder.addCurve ("FFD_A", lCurve);

  const stdair::FFDisutilityCurve_T& lRetrieved =
    lHolder.getFFDisutilityCurve ("FFD_A");
  BOOST_CHECK_EQUAL (lRetrieved.size(), 2u);
  BOOST_CHECK_CLOSE (lRetrieved.at (1), 0.25, 1e-9);
  BOOST_CHECK_EQUAL (lHolder.describe(), "FFD_A; 30:0.5;1:0.25;\n");
  std::ostringstream lStr;
  lHolder.toStream (lStr);
  BOOST_CHECK_EQUAL (lStr.str(), lHolder.describe());
  std::istringstream lIn ("ignored");
  lHolder.fromStream (lIn);

  const stdair::FFDisutilityCurveHolderStruct lCopy (lHolder);
  BOOST_CHECK_EQUAL (lCopy.describe(), lHolder.describe());
}

BOOST_AUTO_TEST_CASE (airline_feature_test) {
  ServiceFixture lFixture;
  lFixture._service.buildSampleBom();
  stdair::BomRoot& lRoot = lFixture.root();

  stdair::AirlineFeature* lAF_ptr =
    stdair::BomRetriever::retrieveAirlineFeatureFromKey (lRoot, "AF");
  BOOST_REQUIRE (lAF_ptr != NULL);
  BOOST_CHECK_EQUAL (lAF_ptr->describeKey(), "AF");
  BOOST_CHECK_EQUAL (lAF_ptr->getKey().toString(), "AF");
  BOOST_CHECK (lAF_ptr->getParent() != NULL);
  BOOST_CHECK_EQUAL (lAF_ptr->getUnconstrainingMethod(),
                     stdair::UnconstrainingMethod::EM);
  BOOST_CHECK_EQUAL (lAF_ptr->getPartnershipTechnique(),
                     stdair::PartnershipTechnique::NONE);
  BOOST_CHECK_EQUAL (lAF_ptr->getPreOptimisationMethod(),
                     stdair::PreOptimisationMethod::NONE);
  BOOST_CHECK_EQUAL (lAF_ptr->getOptimisationMethod(),
                     stdair::OptimisationMethod::LEG_BASED_MC);
  BOOST_CHECK (lAF_ptr->getHolderMap().empty());
  BOOST_CHECK (stdair::BomRetriever::retrieveAirlineFeatureFromKey (lRoot, "ZZ")
               == NULL);

  lAF_ptr->init (stdair::ForecastingMethod ('H'),
                 stdair::UnconstrainingMethod ('E'),
                 stdair::PreOptimisationMethod ('M'),
                 stdair::OptimisationMethod ('E'),
                 7, "manual", stdair::PartnershipTechnique ('r'));
  BOOST_CHECK_EQUAL (lAF_ptr->getForecastingMethod(),
                     stdair::ForecastingMethod::HYBRID_FORECASTING);
  BOOST_CHECK_EQUAL (lAF_ptr->getPreOptimisationMethod(),
                     stdair::PreOptimisationMethod::MRT);
  BOOST_CHECK_EQUAL (lAF_ptr->getOptimisationMethod(),
                     stdair::OptimisationMethod::LEG_BASED_EMSR_B);

  lAF_ptr->setForecastingMethod (stdair::ForecastingMethod ('O'));
  lAF_ptr->setUnconstrainingMethod (stdair::UnconstrainingMethod ('E'));
  lAF_ptr->setPartnershipTechnique (stdair::PartnershipTechnique ('N'));
  lAF_ptr->setPreOptimisationMethod (stdair::PreOptimisationMethod ('N'));
  lAF_ptr->setOptimisationMethod (stdair::OptimisationMethod ('M'));
  BOOST_CHECK_EQUAL (lAF_ptr->getForecastingMethod(),
                     stdair::ForecastingMethod::OLD_QFF);
  BOOST_CHECK_EQUAL (lAF_ptr->getPartnershipTechnique(),
                     stdair::PartnershipTechnique::NONE);

  const std::string lStr = lAF_ptr->toString();
  BOOST_CHECK_EQUAL (lStr.substr (0, 4), "AF, ");
  BOOST_CHECK (lStr.find (", 7, ") != std::string::npos);
  std::ostringstream lOut;
  lAF_ptr->toStream (lOut);
  BOOST_CHECK_EQUAL (lOut.str(), lStr);
  std::istringstream lIn ("ignored");
  lAF_ptr->fromStream (lIn);
}

// ////////////////////////////////////////////////////////////////////
// BomDisplay
// ////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE (bom_display_empty_bom_test) {
  ServiceFixture lFixture;
  const stdair::BomRoot& lRoot = lFixture.root();

  std::ostringstream lList;
  stdair::BomDisplay::list (lList, lRoot);
  BOOST_CHECK (lList.str().empty());
  std::ostringstream lAirportPairs;
  stdair::BomDisplay::listAirportPairDateRange (lAirportPairs, lRoot);
  BOOST_CHECK (lAirportPairs.str().empty());
  std::ostringstream lCSV;
  stdair::BomDisplay::csvDisplay (lCSV, lRoot);
  BOOST_CHECK (lCSV.str().find ("BomRoot: ") != std::string::npos);
  std::ostringstream lPricing;
  stdair::BomDisplay::csvSimFQTAirRACDisplay (lPricing, lRoot);
  BOOST_CHECK (lPricing.str().find ("BomRoot: ") != std::string::npos);
  BOOST_CHECK (lPricing.str().find ("AirportPair") == std::string::npos);

  // Empty inventory, empty flight-date
  stdair::BomRoot& lOtherRoot =
    stdair::FacBom<stdair::BomRoot>::instance().create();
  const stdair::InventoryKey lXXKey ("XX");
  stdair::Inventory& lXXInv =
    stdair::FacBom<stdair::Inventory>::instance().create (lXXKey);
  stdair::FacBomManager::addToListAndMap (lOtherRoot, lXXInv);
  std::ostringstream lInvList;
  stdair::BomDisplay::list (lInvList, lOtherRoot);
  BOOST_CHECK (lInvList.str().empty());
  std::ostringstream lInvCSV;
  stdair::BomDisplay::csvDisplay (lInvCSV, lXXInv);
  BOOST_CHECK (lInvCSV.str().find ("Inventory: XX") != std::string::npos);

  const stdair::FlightDateKey lFDKey (1, kBA9Date);
  stdair::FlightDate& lFD =
    stdair::FacBom<stdair::FlightDate>::instance().create (lFDKey);
  stdair::FacBomManager::addToListAndMap (lXXInv, lFD);
  stdair::FacBomManager::linkWithParent (lXXInv, lFD);
  std::ostringstream lFDCSV;
  stdair::BomDisplay::csvDisplay (lFDCSV, lFD);
  BOOST_CHECK (lFDCSV.str().find ("FlightDate: XX") != std::string::npos);
  BOOST_CHECK (lFDCSV.str().find ("Segments:") == std::string::npos);

  // Empty lists
  std::ostringstream lTS;
  stdair::BomDisplay::csvDisplay (lTS, stdair::TravelSolutionList_T());
  BOOST_CHECK_EQUAL (lTS.str(), "Travel solutions:");
  std::ostringstream lDP;
  stdair::BomDisplay::csvDisplay (lDP, stdair::DatePeriodList_T());
  BOOST_CHECK (lDP.str().empty());
}

BOOST_AUTO_TEST_CASE (bom_display_sample_bom_test) {
  ServiceFixture lFixture;
  lFixture._service.buildSampleBom();
  stdair::BomRoot& lRoot = lFixture.root();

  // Listing of the flight-dates, with and without filters
  std::ostringstream lAll;
  stdair::BomDisplay::list (lAll, lRoot);
  BOOST_CHECK (lAll.str().find ("BA9 / 2011-Jun-10") != std::string::npos);
  BOOST_CHECK (lAll.str().find ("AF") != std::string::npos);
  std::ostringstream lBA9;
  stdair::BomDisplay::list (lBA9, lRoot, "BA", 9);
  BOOST_CHECK (lBA9.str().find ("BA9 / 2011-Jun-10") != std::string::npos);
  BOOST_CHECK (lBA9.str().find ("AF") == std::string::npos);
  std::ostringstream lNone;
  stdair::BomDisplay::list (lNone, lRoot, "BA", 9999);
  BOOST_CHECK (lNone.str().find ("BA9 /") == std::string::npos);

  // Airport pairs / date ranges and pricing tree
  std::ostringstream lAirportPairs;
  stdair::BomDisplay::listAirportPairDateRange (lAirportPairs, lRoot);
  BOOST_CHECK (lAirportPairs.str().find ("LHR") != std::string::npos);
  std::ostringstream lPricing;
  stdair::BomDisplay::csvSimFQTAirRACDisplay (lPricing, lRoot);
  const std::string lPricingStr = lPricing.str();
  BOOST_CHECK (lPricingStr.find ("AirportPair: ") != std::string::npos);
  BOOST_CHECK (lPricingStr.find ("DatePeriod: ") != std::string::npos);
  BOOST_CHECK (lPricingStr.find ("PosChannel: ") != std::string::npos);
  BOOST_CHECK (lPricingStr.find ("TimePeriod: ") != std::string::npos);
  BOOST_CHECK (lPricingStr.find ("Fare/yield-Features: ") != std::string::npos);
  BOOST_CHECK (lPricingStr.find ("AirlineClassList: ") != std::string::npos);

  // Full CSV dump of the inventory tree (partners, O&D dates, fare families)
  stdair::Inventory* lBAInv_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lRoot, "BA");
  BOOST_REQUIRE (lBAInv_ptr != NULL);
  const stdair::InventoryKey lPartnerKey ("QF");
  stdair::Inventory& lPartnerInv =
    stdair::FacBom<stdair::Inventory>::instance().create (lPartnerKey);
  stdair::FacBomManager::addToListAndMap (*lBAInv_ptr, lPartnerInv);
  std::ostringstream lCSV;
  stdair::BomDisplay::csvDisplay (lCSV, lRoot);
  const std::string lCSVStr = lCSV.str();
  BOOST_CHECK (lCSVStr.find ("Inventory: BA") != std::string::npos);
  BOOST_CHECK (lCSVStr.find ("Partner inventory:") != std::string::npos);
  BOOST_CHECK (lCSVStr.find ("O&D-Date:") != std::string::npos);
  BOOST_CHECK (lCSVStr.find ("LegCabins:") != std::string::npos);
  BOOST_CHECK (lCSVStr.find ("Buckets:") != std::string::npos);
  BOOST_CHECK (lCSVStr.find ("Cabin, FF, Bkgs") != std::string::npos);
  BOOST_CHECK (lCSVStr.find ("Subclasses:") != std::string::npos);
  BOOST_CHECK_EQUAL (lFixture._service.csvDisplay(), lCSVStr);

  // Date-period list display, through the service
  lFixture._service.clonePersistentBom();
  const std::string lFareRules =
    lFixture._service.csvDisplay ("LHR", "SYD", kBA9Date);
  BOOST_CHECK (lFareRules.find ("DatePeriod: ") != std::string::npos);
  BOOST_CHECK (lFixture._service.csvDisplay ("XXX", "YYY", kBA9Date)
               .find ("No fare-rule found") != std::string::npos);
  BOOST_CHECK_EQUAL (lFixture._service.list ("BA", 9), lBA9.str());
  BOOST_CHECK_EQUAL (lFixture._service.listAirportPairDateRange(),
                     lAirportPairs.str());

  // Travel solutions
  stdair::TravelSolutionList_T lTSList;
  lFixture._service.buildSampleTravelSolutions (lTSList);
  std::ostringstream lTS;
  stdair::BomDisplay::csvDisplay (lTS, lTSList);
  BOOST_CHECK (lTS.str().find ("[0]") != std::string::npos);
  BOOST_CHECK (lTS.str().find ("[1] ") != std::string::npos);
  BOOST_CHECK (lTS.str().find ("LHR") != std::string::npos);
}

BOOST_AUTO_TEST_CASE (bom_display_codeshare_and_ond_demand_test) {
  ServiceFixture lFixture;
  lFixture._service.buildSampleBom();
  stdair::BomRoot& lRoot = lFixture.root();

  // Link the first AF segment-date as marketing carrier of the first BA9
  // segment-date (code-share).
  stdair::FlightDate& lBA9 = getFlightDate (lRoot, "BA", 9, kBA9Date);
  stdair::Inventory* lAFInv_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lRoot, "AF");
  BOOST_REQUIRE (lAFInv_ptr != NULL);
  const stdair::FlightDateList_T& lAFFDList =
    stdair::BomManager::getList<stdair::FlightDate> (*lAFInv_ptr);
  BOOST_REQUIRE (!lAFFDList.empty());
  stdair::FlightDate& lAFFD = *lAFFDList.front();
  stdair::SegmentDate& lOperatingSD =
    *stdair::BomManager::getList<stdair::SegmentDate> (lBA9).front();
  stdair::SegmentDate& lMarketingSD =
    *stdair::BomManager::getList<stdair::SegmentDate> (lAFFD).front();
  stdair::FacBomManager::addToList<stdair::SegmentDate,
                                   stdair::SegmentDate> (lOperatingSD,
                                                         lMarketingSD);
  stdair::FacBomManager::linkWithOperating (lMarketingSD, lOperatingSD);

  std::ostringstream lOpStr;
  stdair::BomDisplay::csvSegmentDateDisplay (lOpStr, lBA9);
  BOOST_CHECK (lOpStr.str().find ("*** Marketed by AF") != std::string::npos);
  std::ostringstream lMktStr;
  stdair::BomDisplay::csvSegmentDateDisplay (lMktStr, lAFFD);
  BOOST_CHECK (lMktStr.str().find ("*** Operated by BA") != std::string::npos);

  // Demand information on an O&D-date
  stdair::Inventory* lBAInv_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lRoot, "BA");
  BOOST_REQUIRE (lBAInv_ptr != NULL);
  BOOST_REQUIRE (stdair::BomManager::hasList<stdair::OnDDate> (*lBAInv_ptr));
  stdair::OnDDate& lOnDDate =
    *stdair::BomManager::getList<stdair::OnDDate> (*lBAInv_ptr).front();
  std::ostringstream lNoDemand;
  stdair::BomDisplay::csvDisplay (lNoDemand, lOnDDate);
  BOOST_CHECK (lNoDemand.str().find ("Demand mean") == std::string::npos);

  stdair::CabinClassPairList_T lCabinClassPairs;
  lCabinClassPairs.push_back (stdair::CabinClassPair_T ("Y", "M"));
  const stdair::MeanStdDevPair_T lMeanStdDev (20.0, 4.0);
  const stdair::YieldDemandPair_T lYieldDemand (350.0, lMeanStdDev);
  lOnDDate.setDemandInformation (lCabinClassPairs, lYieldDemand);
  std::ostringstream lDemand;
  stdair::BomDisplay::csvDisplay (lDemand, lOnDDate);
  BOOST_CHECK (lDemand.str().find ("Demand mean") != std::string::npos);
  BOOST_CHECK (lDemand.str().find ("20, 4, 350") != std::string::npos);
}

// ////////////////////////////////////////////////////////////////////
// BomArchive
// ////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE (bom_archive_test) {
  ServiceFixture lFixture;
  lFixture._service.buildSampleBom();
  stdair::BomRoot& lRoot = lFixture.root();

  stdair::Inventory* lBAInv_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lRoot, "BA");
  BOOST_REQUIRE (lBAInv_ptr != NULL);
  const std::string lArchive = stdair::BomArchive::archive (*lBAInv_ptr);
  BOOST_CHECK (lArchive.find ("serialization::archive") != std::string::npos);
  BOOST_CHECK (lArchive.find ("BA") != std::string::npos);

  // No-op overloads
  stdair::BomArchive::archive (lRoot);
  stdair::BomArchive::archive (getFlightDate (lRoot, "BA", 9, kBA9Date));

  // restore() ignores its input string and reads from an empty stream
  // (library bug), so the Boost archive constructor fails.
  BOOST_CHECK_THROW (stdair::BomArchive::restore (lArchive, *lBAInv_ptr),
                     boost::archive::archive_exception);
}

BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
