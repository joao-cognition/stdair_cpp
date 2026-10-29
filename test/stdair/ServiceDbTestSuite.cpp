/*!
 * \page ServiceDbTestSuite_cpp Unit tests for the stdair service, command
 *       and dbadaptor layers
 * \code
 */
// STL
#include <fstream>
#include <sstream>
#include <string>
// Boost
#include <boost/make_shared.hpp>
#include <boost/archive/archive_exception.hpp>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// SOCI (in-memory SQLite back-end, so that no database server is needed)
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_db.hpp>
#include <stdair/stdair_file.hpp>
#include <stdair/stdair_json.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/EventType.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/basic/OptimisationMethod.hpp>
#include <stdair/basic/PartnershipTechnique.hpp>
#include <stdair/basic/PreOptimisationMethod.hpp>
#include <stdair/basic/ServiceInitialisationType.hpp>
#include <stdair/basic/UnconstrainingMethod.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/bom/EventStruct.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/command/DBManagerForAirlines.hpp>
#include <stdair/dbadaptor/DbaAirline.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/service/FacSTDAIRServiceContext.hpp>
#include <stdair/service/FacSupervisor.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/STDAIR_ServiceContext.hpp>
#include <stdair/STDAIR_Service.hpp>

namespace {

  /**
   * Standard-conforming access to private member functions: access
   * checking does not apply to the template arguments of an explicit
   * instantiation, so the member pointer can be captured there.
   */
  template <typename Tag, typename Tag::type M>
  struct PrivateMember {
    friend typename Tag::type get (Tag) { return M; }
  };

#define SERVICE_DB_PRIVATE_MEMBER(TAG, CLASS, SIGNATURE, MEMBER)        \
  struct TAG {                                                          \
    typedef SIGNATURE;                                                  \
    friend type get (TAG);                                              \
  };                                                                    \
  template struct PrivateMember<TAG, &CLASS::MEMBER>

  SERVICE_DB_PRIVATE_MEMBER (BomRootExport, stdair::BomRoot,
                             void (stdair::BomRoot::*type)() const,
                             serialisationImplementationExport);
  SERVICE_DB_PRIVATE_MEMBER (BomRootImport, stdair::BomRoot,
                             void (stdair::BomRoot::*type)(),
                             serialisationImplementationImport);
  SERVICE_DB_PRIVATE_MEMBER (InventoryExport, stdair::Inventory,
                             void (stdair::Inventory::*type)() const,
                             serialisationImplementationExport);
  SERVICE_DB_PRIVATE_MEMBER (InventoryImport, stdair::Inventory,
                             void (stdair::Inventory::*type)(),
                             serialisationImplementationImport);
  SERVICE_DB_PRIVATE_MEMBER (FlightDateExport, stdair::FlightDate,
                             void (stdair::FlightDate::*type)() const,
                             serialisationImplementationExport);
  SERVICE_DB_PRIVATE_MEMBER (FlightDateImport, stdair::FlightDate,
                             void (stdair::FlightDate::*type)(),
                             serialisationImplementationImport);
  SERVICE_DB_PRIVATE_MEMBER (SegmentDateExport, stdair::SegmentDate,
                             void (stdair::SegmentDate::*type)() const,
                             serialisationImplementationExport);
  SERVICE_DB_PRIVATE_MEMBER (SegmentDateImport, stdair::SegmentDate,
                             void (stdair::SegmentDate::*type)(),
                             serialisationImplementationImport);
  SERVICE_DB_PRIVATE_MEMBER (SegmentCabinExport, stdair::SegmentCabin,
                             void (stdair::SegmentCabin::*type)() const,
                             serialisationImplementationExport);
  SERVICE_DB_PRIVATE_MEMBER (SegmentCabinImport, stdair::SegmentCabin,
                             void (stdair::SegmentCabin::*type)(),
                             serialisationImplementationImport);
  SERVICE_DB_PRIVATE_MEMBER (ContextShortDisplay, stdair::STDAIR_ServiceContext,
                             const std::string
                             (stdair::STDAIR_ServiceContext::*type)() const,
                             shortDisplay);
  SERVICE_DB_PRIVATE_MEMBER (ContextDisplay, stdair::STDAIR_ServiceContext,
                             const std::string
                             (stdair::STDAIR_ServiceContext::*type)() const,
                             display);
  SERVICE_DB_PRIVATE_MEMBER (ContextDescribe, stdair::STDAIR_ServiceContext,
                             const std::string
                             (stdair::STDAIR_ServiceContext::*type)() const,
                             describe);
  SERVICE_DB_PRIVATE_MEMBER (ContextSetDBParams, stdair::STDAIR_ServiceContext,
                             void (stdair::STDAIR_ServiceContext::*type)
                             (const stdair::BasDBParams&),
                             setDBParams);

#undef SERVICE_DB_PRIVATE_MEMBER

  /** Export must succeed; import from an empty stream must be rejected. */
  template <typename BOM, typename ExportTag, typename ImportTag>
  void checkSerialisationHelpers (BOM& ioObject) {
    BOOST_CHECK_NO_THROW ((ioObject.*get (ExportTag()))());
    BOOST_CHECK_THROW ((ioObject.*get (ImportTag()))(),
                       boost::archive::archive_exception);
  }

  const stdair::AirlineCode_T kBA ("BA");
  const stdair::FlightNumber_T kBA9 (9);
  const stdair::Date_T kBA9Date (2011, 6, 10);
  const stdair::AirportCode_T kLHR ("LHR");
  const stdair::AirportCode_T kSYD ("SYD");
  const stdair::AirportCode_T kCDG ("CDG");
  const stdair::AirportCode_T kSFO ("SFO");
  const stdair::Date_T kUnknownDate (1999, 1, 1);

  const char* kAirlinesDDL =
    "create table airlines (iata_code varchar(3), name varchar(100))";
}

BOOST_AUTO_TEST_SUITE (service_db_test_suite)

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (service_default_ctor_test) {
  stdair::STDAIR_Service lService;
  BOOST_CHECK (lService.getServiceInitialisationType()
               == stdair::ServiceInitialisationType::NOT_YET_INITIALISED);
  BOOST_CHECK (lService.getDBParams().getDBName().empty());
  BOOST_CHECK (&lService.getBomRoot() != &lService.getPersistentBomRoot());
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (service_sample_bom_test) {
  std::ofstream lLogStream ("ServiceDbTestSuite_sample.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  stdair::STDAIR_Service lService (lLogParams);

  // Log parameters are the ones given at construction time
  const stdair::BasLogParams lRetrievedLogParams = lService.getLogParams();
  BOOST_CHECK (lRetrievedLogParams.getLogLevel() == stdair::LOG::DEBUG);

  lService.buildSampleBom();
  lService.clonePersistentBom();

  // The clone BOM tree now holds BA9 / 10-JUN-2011
  BOOST_CHECK (lService.check (kBA, kBA9, kBA9Date));
  BOOST_CHECK (!lService.check (kBA, kBA9, kUnknownDate));
  BOOST_CHECK (lService.check (kLHR, kSYD, kBA9Date));
  BOOST_CHECK (!lService.check (kLHR, kSYD, kUnknownDate));

  // Displays
  const std::string lList = lService.list();
  BOOST_CHECK (lList.find ("BA") != std::string::npos);
  BOOST_CHECK (!lService.list (kBA, kBA9).empty());
  BOOST_CHECK (!lService.listAirportPairDateRange().empty());
  BOOST_CHECK (lService.csvDisplay().find ("BA") != std::string::npos);
  BOOST_CHECK (!lService.csvDisplay (lService.getBomRoot()).empty());
  BOOST_CHECK (!lService.csvDisplay (lService.getPersistentBomRoot()).empty());

  const std::string lFDFound = lService.csvDisplay (kBA, kBA9, kBA9Date);
  BOOST_CHECK (lFDFound.find ("No flight-date found") == std::string::npos);
  const std::string lFDNotFound =
    lService.csvDisplay (kBA, kBA9, kUnknownDate);
  BOOST_CHECK (lFDNotFound.find ("No flight-date found") != std::string::npos);

  const std::string lFRFound = lService.csvDisplay (kLHR, kSYD, kBA9Date);
  BOOST_CHECK (lFRFound.find ("No fare-rule found") == std::string::npos);
  const std::string lFRNotFound =
    lService.csvDisplay (kLHR, kSYD, kUnknownDate);
  BOOST_CHECK (lFRNotFound.find ("No fare-rule found") != std::string::npos);

  // JSON exports
  const std::string lFDList = lService.jsonExportFlightDateList();
  BOOST_CHECK (lFDList.find ("inventories") != std::string::npos);
  BOOST_CHECK (!lService.jsonExportFlightDateList (kBA, kBA9).empty());
  const std::string lFDObjects =
    lService.jsonExportFlightDateObjects (kBA, kBA9, kBA9Date);
  BOOST_CHECK (!lFDObjects.empty());
  BOOST_CHECK (lFDObjects.find ("error") == std::string::npos);
  const std::string lNoFD =
    lService.jsonExportFlightDateObjects (kBA, kBA9, kUnknownDate);
  BOOST_CHECK (lNoFD.find ("No flight-date found") != std::string::npos);
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (service_dummy_inventory_test) {
  std::ofstream lLogStream ("ServiceDbTestSuite_dummy.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  stdair::STDAIR_Service lService (lLogParams);

  lService.buildDummyInventory (300);
  stdair::BomRoot& lBomRoot = lService.getPersistentBomRoot();
  lService.buildDummyLegSegmentAccesses (lBomRoot);

  const stdair::SegmentCabin& lSegmentCabin =
    stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot);
  BOOST_CHECK (!lSegmentCabin.describeKey().empty());
  const stdair::SegmentCabin& lFFSegmentCabin =
    stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot, true);
  BOOST_CHECK (!lFFSegmentCabin.describeKey().empty());

  lService.clonePersistentBom();
  const stdair::Inventory* lXXInv_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lService.getBomRoot(),
                                                    "XX");
  BOOST_CHECK (lXXInv_ptr != NULL);
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (service_sample_structures_test) {
  std::ofstream lLogStream ("ServiceDbTestSuite_structs.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  stdair::STDAIR_Service lService (lLogParams);

  stdair::TravelSolutionList_T lTSList;
  lService.buildSampleTravelSolutions (lTSList);
  BOOST_CHECK (!lTSList.empty());
  BOOST_CHECK (!lService.csvDisplay (lTSList).empty());

  stdair::TravelSolutionList_T lTSPricingList;
  lService.buildSampleTravelSolutionForPricing (lTSPricingList);
  BOOST_CHECK (!lTSPricingList.empty());

  const stdair::BookingRequestStruct lStdRequest =
    lService.buildSampleBookingRequest (false);
  BOOST_CHECK_EQUAL (lStdRequest.getOrigin(), kLHR);
  BOOST_CHECK_EQUAL (lStdRequest.getDestination(), kSYD);

  const stdair::BookingRequestStruct lCRSRequest =
    lService.buildSampleBookingRequest (true);
  BOOST_CHECK_EQUAL (lCRSRequest.getOrigin(), stdair::AirportCode_T ("SIN"));
  BOOST_CHECK_EQUAL (lCRSRequest.getDestination(),
                     stdair::AirportCode_T ("BKK"));

  // Events: booking request, break point, and event types without JSON form
  const stdair::BookingRequestPtr_T lRequestPtr =
    boost::make_shared<stdair::BookingRequestStruct> (lStdRequest);
  const stdair::EventStruct lBkgEvent (stdair::EventType::BKG_REQ,
                                       lRequestPtr);
  const std::string lBkgJSON = lService.jsonExportEventObject (lBkgEvent);
  BOOST_CHECK (lBkgJSON.find ("LHR") != std::string::npos);

  const stdair::BreakPointPtr_T lBreakPointPtr =
    boost::make_shared<stdair::BreakPointStruct> (kBA9Date);
  const stdair::EventStruct lBrkEvent (stdair::EventType::BRK_PT,
                                       lBreakPointPtr);
  BOOST_CHECK (!lService.jsonExportEventObject (lBrkEvent).empty());

  const stdair::EventStruct lCxEvent (stdair::EventType::CX, lRequestPtr);
  BOOST_CHECK (lService.jsonExportEventObject (lCxEvent).empty());

  const stdair::EventStruct lOtherEvent (stdair::EventType::LAST_VALUE,
                                         lRequestPtr);
  BOOST_CHECK (lService.jsonExportEventObject (lOtherEvent).empty());
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (service_configuration_test) {
  std::ofstream lLogStream ("ServiceDbTestSuite_config.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  stdair::STDAIR_Service lService (lLogParams);
  lService.buildSampleBom();

  // JSON configuration
  const stdair::JSONString lBadJSON ("{ this is not JSON");
  BOOST_CHECK (!lService.jsonImportConfiguration (lBadJSON));
  const stdair::JSONString lGoodJSON
    ("{\"config\": {\"simulation\": {\"seed\": \"42\"}}}");
  BOOST_CHECK (lService.jsonImportConfiguration (lGoodJSON));
  BOOST_CHECK (lService.jsonExportConfiguration().find ("seed")
               != std::string::npos);

  // INI configuration: a missing file is silently ignored
  lService.importINIConfig (stdair::ConfigINIFile ("does_not_exist.ini"));

  const std::string lINIFilename ("ServiceDbTestSuite_airline.ini");
  {
    std::ofstream lINIStream (lINIFilename.c_str());
    lINIStream << "[airline_BA]\n"
               << "airline_code=BA\n"
               << "forecasting_method="
               << stdair::ForecastingMethod::getMethodLabel
                    (stdair::ForecastingMethod::NEW_QFF) << "\n"
               << "unconstraining_method="
               << stdair::UnconstrainingMethod::getMethodLabel
                    (stdair::UnconstrainingMethod::EM) << "\n"
               << "partnership_technique="
               << stdair::PartnershipTechnique::getTechniqueLabel
                    (stdair::PartnershipTechnique::RAE_DA) << "\n"
               << "pre_optimisation_method="
               << stdair::PreOptimisationMethod::getMethodLabel
                    (stdair::PreOptimisationMethod::MRT) << "\n"
               << "optimisation_method="
               << stdair::OptimisationMethod::getMethodLabel
                    (stdair::OptimisationMethod::LEG_BASED_EMSR_B) << "\n";
  }
  lService.importINIConfig (stdair::ConfigINIFile (lINIFilename));

  BOOST_CHECK (lService.jsonExportConfiguration().find ("airline_BA")
               != std::string::npos);

  // Single value import, shown by the configuration display
  lService.importConfigValue ("42", "random.seed");
  const std::string lConfigDisplay = lService.configDisplay();
  BOOST_CHECK (lConfigDisplay.find ("Configuration Display")
               != std::string::npos);
  BOOST_CHECK (lConfigDisplay.find ("Random Seed: 42") != std::string::npos);

  // ConfigHolderStruct::updateAirlineFeatures() browses the root of the
  // property tree, whose only child is "config", and then looks the keys up
  // under "config.config.<key>". Hence the airline sections imported above
  // are never applied (library bug, see PR description); the keys are thus
  // also given at the path the implementation actually reads.
  const std::string lConfigPath ("config.config.");
  lService.importConfigValue ("BA", lConfigPath + "airline_code");
  lService.importConfigValue
    (std::string (1, stdair::ForecastingMethod::getMethodLabel
                  (stdair::ForecastingMethod::NEW_QFF)),
     lConfigPath + "forecasting_method");
  lService.importConfigValue
    (std::string (1, stdair::UnconstrainingMethod::getMethodLabel
                  (stdair::UnconstrainingMethod::EM)),
     lConfigPath + "unconstraining_method");
  lService.importConfigValue
    (std::string (1, stdair::PartnershipTechnique::getTechniqueLabel
                  (stdair::PartnershipTechnique::RAE_DA)),
     lConfigPath + "partnership_technique");
  lService.importConfigValue
    (std::string (1, stdair::PreOptimisationMethod::getMethodLabel
                  (stdair::PreOptimisationMethod::MRT)),
     lConfigPath + "pre_optimisation_method");
  lService.importConfigValue
    (std::string (1, stdair::OptimisationMethod::getMethodLabel
                  (stdair::OptimisationMethod::LEG_BASED_EMSR_B)),
     lConfigPath + "optimisation_method");

  // Apply the airline features onto the persistent BOM tree
  lService.updateAirlineFeatures();
  const stdair::AirlineFeature* lFeature_ptr =
    stdair::BomRetriever::retrieveAirlineFeatureFromKey
    (lService.getPersistentBomRoot(), kBA);
  BOOST_REQUIRE (lFeature_ptr != NULL);
  BOOST_CHECK (lFeature_ptr->getForecastingMethod()
               == stdair::ForecastingMethod::NEW_QFF);
  BOOST_CHECK (lFeature_ptr->getUnconstrainingMethod()
               == stdair::UnconstrainingMethod::EM);
  BOOST_CHECK (lFeature_ptr->getPartnershipTechnique()
               == stdair::PartnershipTechnique::RAE_DA);
  BOOST_CHECK (lFeature_ptr->getPreOptimisationMethod()
               == stdair::PreOptimisationMethod::MRT);
  BOOST_CHECK (lFeature_ptr->getOptimisationMethod()
               == stdair::OptimisationMethod::LEG_BASED_EMSR_B);
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (service_context_test) {
  std::ofstream lLogStream ("ServiceDbTestSuite_context.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  stdair::STDAIR_Service lService (lLogParams);

  stdair::STDAIR_ServiceContext& lContext =
    stdair::FacSTDAIRServiceContext::instance().create();
  const stdair::BasDBParams lDBParams ("user", "pwd", "localhost", "3306",
                                       "stdair_db");
  (lContext.*get (ContextSetDBParams()))(lDBParams);

  const std::string lShort = (lContext.*get (ContextShortDisplay()))();
  BOOST_CHECK (lShort.find ("STDAIR_ServiceContext") != std::string::npos);
  BOOST_CHECK (lShort.find ("stdair_db") != std::string::npos);
  BOOST_CHECK_EQUAL ((lContext.*get (ContextDisplay()))(), lShort);
  BOOST_CHECK_EQUAL ((lContext.*get (ContextDescribe()))(), lShort);
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (bom_serialiser_helpers_test) {
  std::ofstream lLogStream ("ServiceDbTestSuite_serialiser.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  stdair::STDAIR_Service lService (lLogParams);
  // buildSampleBom() already adds the dummy (XX) inventories
  lService.buildSampleBom();

  stdair::BomRoot& lBomRoot = lService.getPersistentBomRoot();
  checkSerialisationHelpers<stdair::BomRoot, BomRootExport,
                            BomRootImport> (lBomRoot);

  stdair::Inventory* lInv_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lBomRoot, kBA);
  BOOST_REQUIRE (lInv_ptr != NULL);
  checkSerialisationHelpers<stdair::Inventory, InventoryExport,
                            InventoryImport> (*lInv_ptr);

  stdair::FlightDate* lFD_ptr =
    stdair::BomRetriever::retrieveFlightDateFromKey (*lInv_ptr, kBA9,
                                                     kBA9Date);
  BOOST_REQUIRE (lFD_ptr != NULL);
  checkSerialisationHelpers<stdair::FlightDate, FlightDateExport,
                            FlightDateImport> (*lFD_ptr);

  stdair::SegmentDate* lSD_ptr =
    stdair::BomRetriever::retrieveSegmentDateFromKey (*lFD_ptr, kLHR, kSYD);
  BOOST_REQUIRE (lSD_ptr != NULL);
  checkSerialisationHelpers<stdair::SegmentDate, SegmentDateExport,
                            SegmentDateImport> (*lSD_ptr);

  stdair::SegmentCabin& lSegmentCabin =
    stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot);
  checkSerialisationHelpers<stdair::SegmentCabin, SegmentCabinExport,
                            SegmentCabinImport> (lSegmentCabin);
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (db_session_manager_failure_test) {
  // No connection has been attempted yet: the session is not available
  BOOST_CHECK_THROW (stdair::DBSessionManager::instance().getDBSession(),
                     stdair::NonInitialisedDBSessionManagerException);

  // Nothing listens on port 1: the MySQL connection must be refused
  std::ofstream lLogStream ("ServiceDbTestSuite_db.log");
  const stdair::BasLogParams lLogParams (stdair::LOG::DEBUG, lLogStream);
  const stdair::BasDBParams lDBParams ("stdair", "stdair", "127.0.0.1", "1",
                                       "stdair_unit_test");
  BOOST_CHECK_THROW (stdair::STDAIR_Service lService (lLogParams, lDBParams),
                     stdair::SQLDatabaseConnectionImpossibleException);

  // The constructor did not complete, so reset the singletons ourselves
  stdair::FacSupervisor::cleanAll();

  // The session object has been allocated, even though it is not connected
  BOOST_CHECK_NO_THROW (stdair::DBSessionManager::instance().getDBSession());

  // A fresh service can be instantiated again afterwards
  stdair::STDAIR_Service lService (lLogParams);
  BOOST_CHECK (lService.getDBParams().getDBName().empty());
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (db_manager_for_airlines_test) {
  stdair::DBSession_T lSession (soci::sqlite3, ":memory:");
  lSession << kAirlinesDDL;

  // Insert through the DbaAirline type conversion (to_base)
  const stdair::AirlineStruct lBA ("BA", "British Airways");
  const stdair::AirlineStruct lAF ("AF", "Air France");
  const stdair::AirlineStruct lZZ ("ZZ", "");
  lSession << "insert into airlines (iata_code, name) "
           << "values (:iata_code, :name)", soci::use (lBA);
  lSession << "insert into airlines (iata_code, name) "
           << "values (:iata_code, :name)", soci::use (lAF);
  lSession << "insert into airlines (iata_code, name) "
           << "values (:iata_code, :name)", soci::use (lZZ);

  // An empty name is stored as NULL
  int lNbOfNullNames = -1;
  lSession << "select count(*) from airlines where name is null",
    soci::into (lNbOfNullNames);
  BOOST_CHECK_EQUAL (lNbOfNullNames, 1);

  // Full scan (from_base)
  stdair::AirlineStruct lAirline;
  stdair::DBRequestStatement_T lSelectStatement (lSession);
  stdair::DBManagerForAirlines::prepareSelectStatement (lSession,
                                                        lSelectStatement,
                                                        lAirline);
  std::ostringstream lCodes;
  unsigned short lNbOfRows = 0;
  while (stdair::DBManagerForAirlines::iterateOnStatement (lSelectStatement,
                                                           lAirline)) {
    lCodes << lAirline.getAirlineCode() << ":" << lAirline.getAirlineName()
           << ";";
    ++lNbOfRows;
  }
  BOOST_CHECK_EQUAL (lNbOfRows, 3);
  BOOST_CHECK_EQUAL (lCodes.str(), "BA:British Airways;AF:Air France;ZZ:;");

  // Retrieval by airline code
  stdair::AirlineStruct lRetrieved;
  BOOST_CHECK (stdair::DBManagerForAirlines::retrieveAirline (lSession, "AF",
                                                              lRetrieved));
  BOOST_CHECK_EQUAL (lRetrieved.getAirlineCode(), "AF");
  BOOST_CHECK_EQUAL (lRetrieved.getAirlineName(), "Air France");

  stdair::AirlineStruct lMissing;
  BOOST_CHECK (!stdair::DBManagerForAirlines::retrieveAirline (lSession, "QQ",
                                                               lMissing));

  // Update
  const stdair::AirlineStruct lBAUpdated ("BA", "British Airways Plc");
  stdair::DBManagerForAirlines::updateAirlineInDB (lSession, lBAUpdated);
  stdair::AirlineStruct lRetrievedBA;
  BOOST_CHECK (stdair::DBManagerForAirlines::retrieveAirline (lSession, "BA",
                                                              lRetrievedBA));
  BOOST_CHECK_EQUAL (lRetrievedBA.getAirlineName(), "British Airways Plc");
}

// ////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE (db_manager_for_airlines_error_test) {
  // No 'airlines' table: every statement fails at preparation time
  stdair::DBSession_T lSession (soci::sqlite3, ":memory:");
  stdair::AirlineStruct lAirline;

  stdair::DBRequestStatement_T lSelectStatement (lSession);
  BOOST_CHECK_THROW (stdair::DBManagerForAirlines::
                     prepareSelectStatement (lSession, lSelectStatement,
                                             lAirline),
                     stdair::SQLDatabaseException);
  BOOST_CHECK_THROW (stdair::DBManagerForAirlines::
                     retrieveAirline (lSession, "BA", lAirline),
                     stdair::SQLDatabaseException);
  const stdair::AirlineStruct lBA ("BA", "British Airways");
  BOOST_CHECK_THROW (stdair::DBManagerForAirlines::
                     updateAirlineInDB (lSession, lBA),
                     stdair::SQLDatabaseException);

  // A NULL airline code cannot be converted when the row is fetched
  stdair::DBSession_T lNullSession (soci::sqlite3, ":memory:");
  lNullSession << kAirlinesDDL;
  lNullSession << "insert into airlines (iata_code, name) "
               << "values (NULL, 'Nameless')";
  stdair::DBRequestStatement_T lNullStatement (lNullSession);
  stdair::DBManagerForAirlines::prepareSelectStatement (lNullSession,
                                                        lNullStatement,
                                                        lAirline);
  BOOST_CHECK_THROW (stdair::DBManagerForAirlines::
                     iterateOnStatement (lNullStatement, lAirline),
                     stdair::SQLDatabaseException);
}

BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
