/*!
 * \page BomTreeTestSuite_cpp Unit tests for the stdair BOM tree objects and retrieval
 * \code
 */
// STL
#include <sstream>
#include <fstream>
#include <string>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// Boost Serialisation
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/STDAIR_Service.hpp>
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/basic/BasConst_BomDisplay.hpp>
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRetriever.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/BomRootKey.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/InventoryKey.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/bom/AirlineFeatureKey.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/FlightDateKey.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/LegDateKey.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/LegCabinKey.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/SegmentDateKey.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/SegmentCabinKey.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/FareFamilyKey.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/bom/BookingClassKey.hpp>
#include <stdair/bom/OnDDate.hpp>
#include <stdair/bom/OnDDateKey.hpp>
#include <stdair/bom/AirportPair.hpp>
#include <stdair/bom/DatePeriod.hpp>
#include <stdair/bom/YieldFeatures.hpp>
#include <stdair/bom/YieldFeaturesKey.hpp>
#include <stdair/bom/AirlineClassList.hpp>
#include <stdair/bom/AirlineClassListKey.hpp>
#include <stdair/bom/SegmentSnapshotTable.hpp>
#include <stdair/bom/SegmentSnapshotTableKey.hpp>
#include <stdair/bom/Policy.hpp>
#include <stdair/bom/PolicyKey.hpp>
#include <stdair/bom/NestingNode.hpp>
#include <stdair/bom/NestingNodeKey.hpp>
#include <stdair/bom/NestingStructureKey.hpp>
#include <stdair/bom/SimpleNestingStructure.hpp>
#include <stdair/bom/Bucket.hpp>
#include <stdair/bom/BucketKey.hpp>
#include <stdair/bom/VirtualClassStruct.hpp>
#include <stdair/factory/FacBom.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>

namespace {

  /**
   * Initialises a StdAir service (logger + BOM factories) for one test case.
   */
  struct BomTreeFixture {
    BomTreeFixture (const bool iBuildSample = true)
      : _logFile ("BomTreeTestSuite.log", std::ios::app),
        _service (stdair::BasLogParams (stdair::LOG::DEBUG, _logFile)) {
      if (iBuildSample == true) {
        _service.buildSampleBom();
      }
    }
    stdair::BomRoot& root() { return _service.getPersistentBomRoot(); }
    std::ofstream _logFile;
    stdair::STDAIR_Service _service;
  };

  struct EmptyBomTreeFixture : public BomTreeFixture {
    EmptyBomTreeFixture() : BomTreeFixture (false) {}
  };

  template <typename KEY>
  std::string streamKey (const KEY& iKey) {
    std::ostringstream oStr;
    iKey.toStream (oStr);
    std::istringstream iStr ("ignored");
    const_cast<KEY&> (iKey).fromStream (iStr);
    return oStr.str();
  }

  template <typename KEY>
  std::string archiveKey (KEY& ioKey) {
    std::ostringstream oStr;
    {
      boost::archive::text_oarchive oa (oStr);
      oa << ioKey;
    }
    std::istringstream iStr (oStr.str());
    {
      boost::archive::text_iarchive ia (iStr);
      ia >> ioKey;
    }
    return oStr.str();
  }

  template <typename BOM>
  std::string streamBom (BOM& ioBom) {
    std::ostringstream oStr;
    ioBom.toStream (oStr);
    std::istringstream iStr ("ignored");
    ioBom.fromStream (iStr);
    return oStr.str();
  }
}

BOOST_AUTO_TEST_SUITE (bom_tree_test_suite)

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (inventory_and_flight_date_retrieval_test,
                         BomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  BOOST_CHECK_EQUAL (lBomRoot.toString(), lBomRoot.describeKey());
  BOOST_CHECK (!streamBom (lBomRoot).empty());

  // Inventory retrieval
  stdair::Inventory* lBA_ptr =
    stdair::BomRetriever::retrieveInventoryFromKey (lBomRoot, "BA");
  BOOST_REQUIRE (lBA_ptr != NULL);
  BOOST_CHECK_EQUAL (lBA_ptr->getAirlineCode(), "BA");
  BOOST_CHECK_EQUAL (lBA_ptr->toString(), "BA");
  BOOST_CHECK_EQUAL (streamBom (*lBA_ptr), "BA");
  BOOST_CHECK (stdair::BomRetriever::
               retrieveInventoryFromKey (lBomRoot, stdair::InventoryKey ("AF"))
               != NULL);
  BOOST_CHECK (stdair::BomRetriever::retrieveInventoryFromKey (lBomRoot, "ZZ")
               == NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveInventoryFromLongKey (lBomRoot, "BA;9,2011-06-10")
               == lBA_ptr);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveInventoryFromLongKey (lBomRoot, "ZZ;9,2011-06-10")
               == NULL);
  BOOST_CHECK_THROW (stdair::BomRetriever::
                     retrieveInventoryFromLongKey (lBomRoot, "Z"),
                     stdair::KeyNotFoundException);

  // Inventory features (set up by the sample builder)
  BOOST_REQUIRE (lBA_ptr->getAirlineFeature() != NULL);
  const stdair::AirlineFeature& lFeature = *lBA_ptr->getAirlineFeature();
  BOOST_CHECK (lBA_ptr->getForecastingMethod()
               == lFeature.getForecastingMethod());
  BOOST_CHECK (lBA_ptr->getUnconstrainingMethod()
               == lFeature.getUnconstrainingMethod());
  BOOST_CHECK (lBA_ptr->getPreOptimisationMethod()
               == lFeature.getPreOptimisationMethod());
  BOOST_CHECK (lBA_ptr->getOptimisationMethod()
               == lFeature.getOptimisationMethod());
  BOOST_CHECK (lBA_ptr->getPartnershipTechnique()
               == lFeature.getPartnershipTechnique());
  BOOST_CHECK (lFeature.toString().find ("BA") == 0);
  BOOST_CHECK (!stdair::BomRetriever::
               retrieveAirlineFeatureFromKey (lBomRoot, "ZZ"));
  stdair::BomRetriever::retrieveAirlineFeatureFromKey (lBomRoot, "BA");

  // Flight-date retrieval
  const stdair::Date_T lDate (2011, 6, 10);
  stdair::FlightDate* lBA9_ptr =
    stdair::BomRetriever::retrieveFlightDateFromKeySet (lBomRoot, "BA", 9,
                                                        lDate);
  BOOST_REQUIRE (lBA9_ptr != NULL);
  BOOST_CHECK_EQUAL (lBA9_ptr->getFlightNumber(), 9);
  BOOST_CHECK (lBA9_ptr->getDepartureDate() == lDate);
  BOOST_CHECK_EQUAL (lBA9_ptr->getAirlineCode(), "BA");
  BOOST_CHECK_EQUAL (lBA9_ptr->toString(), lBA9_ptr->describeKey());
  BOOST_CHECK_EQUAL (streamBom (*lBA9_ptr), lBA9_ptr->describeKey());
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromKeySet (lBomRoot, "ZZ", 9, lDate) == NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromKeySet (lBomRoot, "BA", 1234, lDate)
               == NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromKey (*lBA_ptr,
                                          stdair::FlightDateKey (9, lDate))
               == lBA9_ptr);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromLongKey (lBomRoot, "BA;9,2011-06-10")
               == lBA9_ptr);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromLongKey (lBomRoot, "ZZ;9,2011-06-10")
               == NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromLongKey (*lBA_ptr, "BA;9,2011-06-10")
               == lBA9_ptr);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveFlightDateFromLongKey (*lBA_ptr, "BA;10,2011-06-10")
               == NULL);
  BOOST_CHECK_THROW (stdair::BomRetriever::
                     retrieveFlightDateFromLongKey (*lBA_ptr, "BA;9"),
                     stdair::KeyNotFoundException);

  // Partner inventory lookup from an inventory without partners
  BOOST_CHECK (stdair::BomRetriever::
               retrieveInventoryFromLongKey (*lBA_ptr, "AF;84,2011-03-20")
               == NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrievePartnerSegmentDateFromLongKey (*lBA_ptr,
                                                      "AF;84,2011-03-20;CDG,SFO")
               == NULL);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (segment_and_leg_retrieval_over_whole_tree_test,
                         BomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  BOOST_REQUIRE (stdair::BomManager::hasList<stdair::Inventory> (lBomRoot));
  BOOST_CHECK (stdair::BomManager::hasMap<stdair::Inventory> (lBomRoot));
  const stdair::InventoryList_T& lInvList =
    stdair::BomManager::getList<stdair::Inventory> (lBomRoot);
  BOOST_CHECK_EQUAL (lInvList.size(),
                     stdair::BomManager::getMap<stdair::Inventory> (lBomRoot)
                     .size());
  BOOST_CHECK (lInvList.size() >= 4);

  unsigned int lNbOfSegments = 0;
  unsigned int lNbOfClasses = 0;
  unsigned int lNbOfLegs = 0;
  unsigned int lNbOfPartnerSegments = 0;
  for (stdair::InventoryList_T::const_iterator itInv = lInvList.begin();
       itInv != lInvList.end(); ++itInv) {
    const stdair::Inventory& lInv = **itInv;
    BOOST_CHECK (stdair::BomManager::getParentPtr<stdair::BomRoot> (lInv)
                 == &lBomRoot);
    BOOST_CHECK (&stdair::BomManager::getObject<stdair::Inventory>
                 (lBomRoot, lInv.describeKey()) == &lInv);
    if (stdair::BomManager::hasList<stdair::FlightDate> (lInv) == false) {
      continue;
    }
    const stdair::FlightDateList_T& lFDList =
      stdair::BomManager::getList<stdair::FlightDate> (lInv);
    for (stdair::FlightDateList_T::const_iterator itFD = lFDList.begin();
         itFD != lFDList.end(); ++itFD) {
      const stdair::FlightDate& lFD = **itFD;
      BOOST_CHECK (&stdair::BomManager::getParent<stdair::Inventory> (lFD)
                   == &lInv);
      BOOST_CHECK (stdair::BomRetriever::
                   retrieveFlightDateFromKey (lInv, lFD.getKey()) == &lFD);

      // Legs
      if (stdair::BomManager::hasList<stdair::LegDate> (lFD)) {
        const stdair::LegDateList_T& lLDList =
          stdair::BomManager::getList<stdair::LegDate> (lFD);
        for (stdair::LegDateList_T::const_iterator itLD = lLDList.begin();
             itLD != lLDList.end(); ++itLD) {
          stdair::LegDate& lLD = **itLD;
          ++lNbOfLegs;
          BOOST_CHECK_EQUAL (lLD.getAirlineCode(), lInv.getAirlineCode());
          BOOST_CHECK_EQUAL (lLD.toString(), lLD.describeKey());
          BOOST_CHECK_EQUAL (streamBom (lLD), lLD.describeKey());
          BOOST_CHECK (lLD.getParent() == &lFD);
          const std::string lLegFullKey = lInv.describeKey()
            + stdair::DEFAULT_KEY_FLD_DELIMITER + lFD.describeKey()
            + stdair::DEFAULT_KEY_FLD_DELIMITER + lLD.getBoardingPoint()
            + stdair::DEFAULT_KEY_SUB_FLD_DELIMITER + lLD.getOffPoint();
          BOOST_CHECK (stdair::BomRetriever::
                       retrieveOperatingLegDateFromLongKey (lFD, lLegFullKey)
                       == &lLD);
          lLD.setOperatingAirlineCode (lInv.getAirlineCode());
          lLD.setOperatingFlightNumber (lFD.getFlightNumber());
          const std::string lRoutingKey = lLD.describeRoutingKey();
          BOOST_CHECK (lRoutingKey.find (lLD.describeKey())
                       != std::string::npos);
          BOOST_CHECK (lRoutingKey.find (lInv.getAirlineCode()) == 0);
          lLD.getTimeOffset();
          if (stdair::BomManager::hasList<stdair::LegCabin> (lLD)) {
            const stdair::LegCabinList_T& lLCList =
              stdair::BomManager::getList<stdair::LegCabin> (lLD);
            for (stdair::LegCabinList_T::const_iterator itLC = lLCList.begin();
                 itLC != lLCList.end(); ++itLC) {
              const stdair::LegCabin& lLC = **itLC;
              BOOST_CHECK (lLD.getLegCabin (lLC.getCabinCode()) == &lLC);
              BOOST_CHECK (lLD.getLegCabin (stdair::LegCabinKey
                                            (lLC.getCabinCode())) == &lLC);
              BOOST_CHECK_EQUAL (lLC.getFullerKey(), lLD.describeKey()
                                 + stdair::DEFAULT_KEY_FLD_DELIMITER
                                 + lLC.getCabinCode());
              BOOST_CHECK (lLC.getParent() == &lLD);
            }
          }
          BOOST_CHECK (lLD.getLegCabin ("Unknown") == NULL);
        }
      }

      // Segments
      if (stdair::BomManager::hasList<stdair::SegmentDate> (lFD) == false) {
        continue;
      }
      const stdair::SegmentDateList_T& lSDList =
        stdair::BomManager::getList<stdair::SegmentDate> (lFD);
      for (stdair::SegmentDateList_T::const_iterator itSD = lSDList.begin();
           itSD != lSDList.end(); ++itSD) {
        stdair::SegmentDate& lSD = **itSD;
        ++lNbOfSegments;
        BOOST_CHECK (lSD.getParent() == &lFD);
        BOOST_CHECK_EQUAL (lSD.toString(), lSD.describeKey());
        BOOST_CHECK_EQUAL (streamBom (lSD), lSD.describeKey());
        lSD.getTimeOffset();
        const std::string lFullKey =
          stdair::BomRetriever::retrieveFullKeyFromSegmentDate (lSD);
        BOOST_CHECK (lFullKey.find (lSD.describeKey()) != std::string::npos);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lBomRoot, lFullKey)
                     == &lSD);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lInv, lFullKey) == &lSD);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lFD, lFullKey) == &lSD);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromKey (lFD, lSD.getKey()) == &lSD);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromKey (lFD, lSD.getBoardingPoint(),
                                                 lSD.getOffPoint()) == &lSD);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromKey (lFD, "ZZZ", "YYY") == NULL);

        // Booking classes reachable from the segment-date
        if (stdair::BomManager::hasList<stdair::BookingClass> (lSD)) {
          const stdair::BookingClassList_T& lBCList =
            stdair::BomManager::getList<stdair::BookingClass> (lSD);
          for (stdair::BookingClassList_T::const_iterator itBC =
                 lBCList.begin(); itBC != lBCList.end(); ++itBC) {
            const stdair::BookingClass& lBC = **itBC;
            ++lNbOfClasses;
            BOOST_CHECK (stdair::BomRetriever::
                         retrieveBookingClassFromLongKey (lInv, lFullKey,
                                                          lBC.getClassCode())
                         == &lBC);
          }
        }
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveBookingClassFromLongKey (lInv, lFullKey, "?")
                     == NULL);

        // Segment cabins
        if (stdair::BomManager::hasList<stdair::SegmentCabin> (lSD)) {
          const stdair::SegmentCabinList_T& lSCList =
            stdair::BomManager::getList<stdair::SegmentCabin> (lSD);
          for (stdair::SegmentCabinList_T::const_iterator itSC =
                 lSCList.begin(); itSC != lSCList.end(); ++itSC) {
            const stdair::SegmentCabin& lSC = **itSC;
            BOOST_CHECK (lSC.getParent() == &lSD);
            BOOST_CHECK_EQUAL (lSC.getFullerKey(), lSD.describeKey()
                               + stdair::DEFAULT_KEY_FLD_DELIMITER
                               + lSC.getCabinCode());
            if (stdair::BomManager::hasList<stdair::FareFamily> (lSC)) {
              const stdair::FareFamilyList_T& lFFList =
                stdair::BomManager::getList<stdair::FareFamily> (lSC);
              for (stdair::FareFamilyList_T::const_iterator itFF =
                     lFFList.begin(); itFF != lFFList.end(); ++itFF) {
                stdair::FareFamily& lFF = **itFF;
                BOOST_CHECK_EQUAL (lFF.toString(), lFF.describeKey());
                BOOST_CHECK_EQUAL (streamBom (lFF), lFF.describeKey());
                BOOST_CHECK (lFF.getParent() == &lSC);
              }
            }
          }
        }

        // Inventory-level lookups that must fail
        const std::string lWrongFlight = lInv.describeKey()
          + ";9876,2011-06-10;" + lSD.getBoardingPoint() + ","
          + lSD.getOffPoint();
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lInv, lWrongFlight)
                     == NULL);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveBookingClassFromLongKey (lInv, lWrongFlight, "Y")
                     == NULL);
        const std::string lWrongSegment = lInv.describeKey()
          + stdair::DEFAULT_KEY_FLD_DELIMITER + lFD.describeKey()
          + ";QQQ,RRR";
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lInv, lWrongSegment)
                     == NULL);
        const std::string lWrongAirline = "ZZ;9,2011-06-10;LHR,SYD";
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lInv, lWrongAirline)
                     == NULL);
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveSegmentDateFromLongKey (lBomRoot, lWrongAirline)
                     == NULL || lInv.getAirlineCode() != "ZZ");
      }
    }

    // Partner inventories (partnership sample)
    if (stdair::BomManager::hasList<stdair::Inventory> (lInv)) {
      const stdair::InventoryList_T& lPartnerList =
        stdair::BomManager::getList<stdair::Inventory> (lInv);
      for (stdair::InventoryList_T::const_iterator itPInv =
             lPartnerList.begin(); itPInv != lPartnerList.end(); ++itPInv) {
        const stdair::Inventory& lPartnerInv = **itPInv;
        BOOST_CHECK (stdair::BomRetriever::
                     retrieveInventoryFromLongKey (lInv,
                                                   lPartnerInv.describeKey())
                     == &lPartnerInv);
        if (!stdair::BomManager::hasList<stdair::FlightDate> (lPartnerInv)) {
          continue;
        }
        const stdair::FlightDateList_T& lPFDList =
          stdair::BomManager::getList<stdair::FlightDate> (lPartnerInv);
        for (stdair::FlightDateList_T::const_iterator itPFD = lPFDList.begin();
             itPFD != lPFDList.end(); ++itPFD) {
          const stdair::FlightDate& lPFD = **itPFD;
          if (!stdair::BomManager::hasList<stdair::SegmentDate> (lPFD)) {
            continue;
          }
          const stdair::SegmentDateList_T& lPSDList =
            stdair::BomManager::getList<stdair::SegmentDate> (lPFD);
          for (stdair::SegmentDateList_T::const_iterator itPSD =
                 lPSDList.begin(); itPSD != lPSDList.end(); ++itPSD) {
            const stdair::SegmentDate& lPSD = **itPSD;
            const std::string lPKey =
              stdair::BomRetriever::retrieveFullKeyFromSegmentDate (lPSD);
            BOOST_CHECK (stdair::BomRetriever::
                         retrievePartnerSegmentDateFromLongKey (lInv, lPKey)
                         == &lPSD);
            ++lNbOfPartnerSegments;
          }
        }
      }
    }
  }
  BOOST_CHECK (lNbOfSegments >= 5);
  BOOST_CHECK (lNbOfLegs >= 4);
  BOOST_CHECK (lNbOfClasses > 0);
  BOOST_TEST_MESSAGE ("Partner segment-dates: " << lNbOfPartnerSegments);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (operating_and_marketing_segment_test,
                         EmptyBomTreeFixture) {
  stdair::SegmentDate& lOperating = stdair::FacBom<stdair::SegmentDate>::
    instance().create (stdair::SegmentDateKey ("SIN", "BKK"));
  stdair::SegmentDate& lMarketing = stdair::FacBom<stdair::SegmentDate>::
    instance().create (stdair::SegmentDateKey ("SIN", "BKK"));
  BOOST_CHECK (lMarketing.getOperatingSegmentDate() == NULL);
  stdair::FacBomManager::linkWithOperating (lMarketing, lOperating);
  stdair::FacBomManager::addToList (lOperating, lMarketing);
  BOOST_CHECK (lMarketing.getOperatingSegmentDate() == &lOperating);
  // NB: BomManager::hasList/getList<SegmentDate,SegmentDate> are inline
  // specialisations: calling them from here would interpose the library's
  // (instrumented) copies, hence the direct accessor.
  BOOST_CHECK_EQUAL (lOperating.getMarketingSegmentDateList().size(), 1U);
  BOOST_CHECK (lOperating.getMarketingSegmentDateList().front()
               == &lMarketing);
  BOOST_CHECK (lMarketing.getMarketingSegmentDateList().empty());

  // A segment-date without any parent has no full key
  BOOST_CHECK_EQUAL (stdair::BomRetriever::
                     retrieveFullKeyFromSegmentDate (lOperating), "");

  // Time offsets
  lOperating.setBoardingDate (stdair::Date_T (2010, 3, 8));
  lOperating.setOffDate (stdair::Date_T (2010, 3, 9));
  lOperating.setBoardingTime (stdair::Duration_T (22, 0, 0));
  lOperating.setOffTime (stdair::Duration_T (1, 0, 0));
  lOperating.setElapsedTime (stdair::Duration_T (2, 0, 0));
  BOOST_CHECK_EQUAL (lOperating.getDateOffset().days(), 1);
  BOOST_CHECK (lOperating.getTimeOffset() == stdair::Duration_T (1, 0, 0));

  // A segment-date inside a flight-date but without an inventory parent
  stdair::FlightDate& lFD = stdair::FacBom<stdair::FlightDate>::instance()
    .create (stdair::FlightDateKey (11, stdair::Date_T (2010, 3, 8)));
  stdair::FacBomManager::linkWithParent (lFD, lMarketing);
  BOOST_CHECK_EQUAL (stdair::BomRetriever::
                     retrieveFullKeyFromSegmentDate (lMarketing), "");

  // A leg-date and its time offset / distance
  stdair::LegDate& lLD =
    stdair::FacBom<stdair::LegDate>::instance().create (stdair::LegDateKey
                                                        ("SIN"));
  lLD.setOffPoint ("BKK");
  lLD.setBoardingDate (stdair::Date_T (2010, 3, 8));
  lLD.setOffDate (stdair::Date_T (2010, 3, 8));
  lLD.setBoardingTime (stdair::Duration_T (8, 0, 0));
  lLD.setOffTime (stdair::Duration_T (10, 0, 0));
  lLD.setElapsedTime (stdair::Duration_T (3, 0, 0));
  BOOST_CHECK (lLD.getTimeOffset() == stdair::Duration_T (-1, 0, 0));
  BOOST_CHECK (lLD.getDistance() > 0);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (partner_inventory_retrieval_test,
                         EmptyBomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  stdair::Inventory& lSQInv = stdair::FacBom<stdair::Inventory>::instance()
    .create (stdair::InventoryKey ("SQ"));
  stdair::FacBomManager::addToListAndMap (lBomRoot, lSQInv);
  stdair::FacBomManager::linkWithParent (lBomRoot, lSQInv);

  // CX inventory, as seen (as a partner) from the SQ inventory
  stdair::Inventory& lCXPartnerInv =
    stdair::FacBom<stdair::Inventory>::instance()
    .create (stdair::InventoryKey ("CX"));
  stdair::FacBomManager::addToListAndMap (lSQInv, lCXPartnerInv);
  stdair::FacBomManager::linkWithParent (lSQInv, lCXPartnerInv);
  stdair::FlightDate& lCXFD = stdair::FacBom<stdair::FlightDate>::instance()
    .create (stdair::FlightDateKey (12, stdair::Date_T (2010, 3, 8)));
  stdair::FacBomManager::addToListAndMap (lCXPartnerInv, lCXFD);
  stdair::FacBomManager::linkWithParent (lCXPartnerInv, lCXFD);
  stdair::SegmentDate& lCXSD = stdair::FacBom<stdair::SegmentDate>::instance()
    .create (stdair::SegmentDateKey ("BKK", "HKG"));
  stdair::FacBomManager::addToListAndMap (lCXFD, lCXSD);
  stdair::FacBomManager::linkWithParent (lCXFD, lCXSD);

  BOOST_CHECK (stdair::BomRetriever::
               retrieveInventoryFromLongKey (lSQInv, "CX") == &lCXPartnerInv);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveInventoryFromLongKey (lSQInv, "BA") == NULL);
  const std::string lFullKey =
    stdair::BomRetriever::retrieveFullKeyFromSegmentDate (lCXSD);
  BOOST_CHECK_EQUAL (lFullKey, "CX,12, 2010-03-08,BKK, HKG");
  BOOST_CHECK (stdair::BomRetriever::
               retrievePartnerSegmentDateFromLongKey (lSQInv, lFullKey)
               == &lCXSD);
  BOOST_CHECK (stdair::BomRetriever::
               retrievePartnerSegmentDateFromLongKey
               (lSQInv, "CX,12, 2010-03-08,BKK, SIN") == NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrievePartnerSegmentDateFromLongKey
               (lSQInv, "BA,12, 2010-03-08,BKK, HKG") == NULL);
  // The CX partner inventory is not a top-level inventory
  BOOST_CHECK (stdair::BomRetriever::
               retrieveSegmentDateFromLongKey (lBomRoot, lFullKey) == NULL);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (airport_pair_and_date_period_retrieval_test,
                         BomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  stdair::AirportPair* lLHRSYD_ptr = stdair::BomRetriever::
    retrieveAirportPairFromKeySet (lBomRoot, "LHR", "SYD");
  BOOST_REQUIRE (lLHRSYD_ptr != NULL);
  BOOST_CHECK (stdair::BomRetriever::
               retrieveAirportPairFromKeySet (lBomRoot, "QQQ", "RRR") == NULL);

  stdair::DatePeriodList_T lValidList;
  stdair::BomRetriever::
    retrieveDatePeriodListFromKeySet (lBomRoot, "LHR", "SYD",
                                      stdair::Date_T (2011, 6, 10), lValidList);
  BOOST_CHECK (!lValidList.empty());

  stdair::DatePeriodList_T lOutOfRangeList;
  stdair::BomRetriever::
    retrieveDatePeriodListFromKey (*lLHRSYD_ptr, stdair::Date_T (2030, 1, 1),
                                   lOutOfRangeList);
  BOOST_CHECK (lOutOfRangeList.empty());

  stdair::DatePeriodList_T lUnknownList;
  stdair::BomRetriever::
    retrieveDatePeriodListFromKeySet (lBomRoot, "QQQ", "RRR",
                                      stdair::Date_T (2011, 6, 10),
                                      lUnknownList);
  BOOST_CHECK (lUnknownList.empty());
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (dummy_cabin_retrieval_test, BomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  stdair::LegCabin& lLC = stdair::BomRetriever::retrieveDummyLegCabin (lBomRoot);
  BOOST_CHECK_EQUAL (lLC.getCabinCode(), stdair::DEFAULT_CABIN_CODE);
  stdair::LegCabin& lLCFF =
    stdair::BomRetriever::retrieveDummyLegCabin (lBomRoot, true);
  BOOST_CHECK (&lLCFF != &lLC);
  stdair::SegmentCabin& lSC =
    stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot);
  BOOST_CHECK_EQUAL (lSC.getCabinCode(), stdair::DEFAULT_CABIN_CODE);
  stdair::SegmentCabin& lSCFF =
    stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot, true);
  BOOST_CHECK (&lSCFF != &lSC);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (dummy_cabin_retrieval_failure_test,
                         EmptyBomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  // No inventory at all
  BOOST_CHECK_THROW (stdair::BomRetriever::retrieveDummyLegCabin (lBomRoot),
                     stdair::ObjectNotFoundException);
  BOOST_CHECK_THROW (stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot),
                     stdair::ObjectNotFoundException);

  // Default inventory, but no flight-date
  stdair::Inventory& lInv = stdair::FacBom<stdair::Inventory>::instance()
    .create (stdair::InventoryKey (stdair::DEFAULT_AIRLINE_CODE));
  stdair::FacBomManager::addToListAndMap (lBomRoot, lInv);
  stdair::FacBomManager::linkWithParent (lBomRoot, lInv);
  for (unsigned short isFF = 0; isFF != 2; ++isFF) {
    BOOST_CHECK_THROW (stdair::BomRetriever::
                       retrieveDummyLegCabin (lBomRoot, isFF == 1),
                       stdair::ObjectNotFoundException);
    BOOST_CHECK_THROW (stdair::BomRetriever::
                       retrieveDummySegmentCabin (lBomRoot, isFF == 1),
                       stdair::ObjectNotFoundException);
  }

  // Default flight-date, but no leg/segment
  stdair::FlightDate& lFD = stdair::FacBom<stdair::FlightDate>::instance()
    .create (stdair::FlightDateKey (stdair::DEFAULT_FLIGHT_NUMBER,
                                    stdair::DEFAULT_DEPARTURE_DATE));
  stdair::FacBomManager::addToListAndMap (lInv, lFD);
  stdair::FacBomManager::linkWithParent (lInv, lFD);
  BOOST_CHECK_THROW (stdair::BomRetriever::retrieveDummyLegCabin (lBomRoot),
                     stdair::ObjectNotFoundException);
  BOOST_CHECK_THROW (stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot),
                     stdair::ObjectNotFoundException);

  // Leg and segment, but no cabins
  stdair::LegDate& lLD = stdair::FacBom<stdair::LegDate>::instance()
    .create (stdair::LegDateKey (stdair::DEFAULT_ORIGIN));
  stdair::FacBomManager::addToListAndMap (lFD, lLD);
  stdair::FacBomManager::linkWithParent (lFD, lLD);
  stdair::SegmentDate& lSD = stdair::FacBom<stdair::SegmentDate>::instance()
    .create (stdair::SegmentDateKey (stdair::DEFAULT_ORIGIN,
                                     stdair::DEFAULT_DESTINATION));
  stdair::FacBomManager::addToListAndMap (lFD, lSD);
  stdair::FacBomManager::linkWithParent (lFD, lSD);
  BOOST_CHECK_THROW (stdair::BomRetriever::retrieveDummyLegCabin (lBomRoot),
                     stdair::ObjectNotFoundException);
  BOOST_CHECK_THROW (stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot),
                     stdair::ObjectNotFoundException);
  BOOST_CHECK (!stdair::BomRetriever::retrieveFullKeyFromSegmentDate (lSD)
               .empty());

  // Linking the same key twice into a map is refused
  stdair::LegDate& lDuplicateLD = stdair::FacBom<stdair::LegDate>::instance()
    .create (stdair::LegDateKey (stdair::DEFAULT_ORIGIN));
  BOOST_CHECK_THROW (stdair::FacBomManager::addToMap (lFD, lDuplicateLD),
                     stdair::ObjectLinkingException);

  // Lists of children which were never added
  BOOST_CHECK_THROW (stdair::BomManager::getList<stdair::LegCabin> (lLD),
                     stdair::NonInitialisedContainerException);
  BOOST_CHECK (!stdair::BomManager::hasMap<stdair::LegCabin> (lLD));
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (leg_cabin_mutators_test, BomTreeFixture) {
  stdair::LegCabin& lLC =
    stdair::BomRetriever::retrieveDummyLegCabin (root());
  BOOST_CHECK_EQUAL (lLC.toString(), lLC.describeKey());
  BOOST_CHECK_EQUAL (streamBom (lLC), lLC.describeKey());
  BOOST_CHECK (lLC.getParent() != NULL);

  lLC.setCapacities (100.0);
  lLC.setCommittedSpace (0.0);
  lLC.setAvailabilityPool (100.0);
  BOOST_CHECK_EQUAL (lLC.getOfferedCapacity(), 100.0);
  BOOST_CHECK_EQUAL (lLC.getPhysicalCapacity(), 100.0);
  lLC.updateFromReservation (10.0);
  BOOST_CHECK_EQUAL (lLC.getCommittedSpace(), 10.0);
  BOOST_CHECK_EQUAL (lLC.getAvailabilityPool(), 90.0);

  // Bid-price vector: current bid price picked from the availability pool
  lLC.emptyBidPriceVector();
  stdair::BidPriceVector_T& lBPV = lLC.getBidPriceVector();
  for (unsigned short idx = 0; idx != 100; ++idx) {
    lBPV.push_back (1000.0 - idx);
  }
  const stdair::LegCabin& lConstLC = lLC;
  BOOST_CHECK_EQUAL (lConstLC.getBidPriceVector().size(), 100U);
  lLC.setPreviousBidPrice (1.0);
  lLC.setCurrentBidPrice (5.0);
  lLC.updatePreviousBidPrice();
  BOOST_CHECK_EQUAL (lLC.getPreviousBidPrice(), 5.0);
  lLC.updateCurrentBidPrice();
  BOOST_CHECK_EQUAL (lLC.getCurrentBidPrice(), 1000.0 - 89);
  // Availability larger than the bid-price vector: unchanged
  lLC.setAvailabilityPool (500.0);
  lLC.updateCurrentBidPrice();
  BOOST_CHECK_EQUAL (lLC.getCurrentBidPrice(), 1000.0 - 89);
  // No availability: unchanged
  lLC.setAvailabilityPool (0.0);
  lLC.updateCurrentBidPrice();
  BOOST_CHECK_EQUAL (lLC.getCurrentBidPrice(), 1000.0 - 89);
  lLC.emptyBidPriceVector();
  BOOST_CHECK (lConstLC.getBidPriceVector().empty());

  // Simple setters/getters
  lLC.setSoldSeat (3.0);
  BOOST_CHECK_EQUAL (lLC.getSoldSeat(), 3.0);
  lLC.setAvailability (7.0);
  BOOST_CHECK_EQUAL (lLC.getAvailability(), 7.0);
  lLC.setRegradeAdjustment (2.0);
  BOOST_CHECK_EQUAL (lLC.getRegradeAdjustment(), 2.0);
  lLC.setAuthorizationLevel (120.0);
  BOOST_CHECK_EQUAL (lLC.getAuthorizationLevel(), 120.0);
  lLC.setUPR (4.0);
  BOOST_CHECK_EQUAL (lLC.getUPR(), 4.0);
  lLC.setNetAvailability (5.0);
  BOOST_CHECK_EQUAL (lLC.getNetAvailability(), 5.0);
  lLC.setGrossAvailability (6.0);
  BOOST_CHECK_EQUAL (lLC.getGrossAvailability(), 6.0);
  lLC.setAvgCancellationPercentage (0.1);
  BOOST_CHECK_EQUAL (lLC.getAvgCancellationPercentage(), 0.1);
  lLC.setETB (8.0);
  BOOST_CHECK_EQUAL (lLC.getETB(), 8.0);
  lLC.setStaffNbOfSeats (1.0);
  BOOST_CHECK_EQUAL (lLC.getStaffNbOfSeats(), 1.0);
  lLC.setWLNbOfSeats (2.0);
  BOOST_CHECK_EQUAL (lLC.getWLNbOfSeats(), 2.0);
  lLC.setGroupNbOfSeats (9.0);
  BOOST_CHECK_EQUAL (lLC.getGroupNbOfSeats(), 9.0);

  // Virtual classes
  lLC.emptyVirtualClassList();
  BOOST_CHECK_EQUAL (lLC.displayVirtualClassList(), "");
  stdair::BookingClassList_T lEmptyBCList;
  stdair::VirtualClassStruct lVC (lEmptyBCList);
  lVC.setYield (150.0);
  lVC.setCumulatedProtection (10.0);
  lVC.setCumulatedBookingLimit (90.0);
  lLC.addVirtualClass (lVC);
  BOOST_CHECK_EQUAL (lLC.getVirtualClassList().size(), 1U);
  const std::string lVCDisplay = lLC.displayVirtualClassList();
  BOOST_CHECK (lVCDisplay.find ("Yield: 150.00") != std::string::npos);
  // NB: VirtualClassStruct's copy constructor does not copy the cumulated
  // booking limit/protection, so only the yield survives addVirtualClass().
  BOOST_CHECK (lVCDisplay.find ("Booking limit: ") != std::string::npos);
  lLC.emptyVirtualClassList();
  BOOST_CHECK (lLC.getVirtualClassList().empty());

  // Demand information aggregated per yield level
  lLC.emptyYieldLevelDemandMap();
  lLC.addDemandInformation (99.6, 10.0, 3.0);
  lLC.addDemandInformation (100.2, 5.0, 4.0);
  lLC.addDemandInformation (200.0, 1.0, 1.0);
  const stdair::YieldLevelDemandMap_T& lDemandMap =
    lLC.getYieldLevelDemandMap();
  BOOST_REQUIRE_EQUAL (lDemandMap.size(), 2U);
  const stdair::MeanStdDevPair_T& lPair = lDemandMap.find (100)->second;
  BOOST_CHECK_CLOSE (lPair.first, 15.0, 1e-9);
  BOOST_CHECK_CLOSE (lPair.second, 5.0, 1e-9);
  lLC.emptyYieldLevelDemandMap();
  BOOST_CHECK (lLC.getYieldLevelDemandMap().empty());
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (segment_cabin_and_booking_class_test, BomTreeFixture) {
  stdair::SegmentCabin& lSC =
    stdair::BomRetriever::retrieveDummySegmentCabin (root(), true);
  BOOST_CHECK_EQUAL (lSC.toString(), lSC.describeKey());
  BOOST_CHECK_EQUAL (streamBom (lSC), lSC.describeKey());
  BOOST_CHECK (lSC.getParent() != NULL);

  lSC.setCommittedSpace (0.0);
  lSC.updateFromReservation (4.0);
  lSC.updateFromReservation (2.0);
  BOOST_CHECK_EQUAL (lSC.getCommittedSpace(), 6.0);
  lSC.setCapacity (50.0);
  BOOST_CHECK_EQUAL (lSC.getCapacity(), 50.0);
  lSC.setBlockSpace (3.0);
  BOOST_CHECK_EQUAL (lSC.getBlockSpace(), 3.0);
  lSC.setMIN (1.0);
  BOOST_CHECK_EQUAL (lSC.getMIN(), 1.0);
  lSC.setUPR (2.0);
  BOOST_CHECK_EQUAL (lSC.getUPR(), 2.0);
  lSC.setBookingCounter (11.0);
  BOOST_CHECK_EQUAL (lSC.getBookingCounter(), 11.0);
  lSC.setAvailabilityPool (44.0);
  BOOST_CHECK_EQUAL (lSC.getAvailabilityPool(), 44.0);
  stdair::BidPriceVector_T lBPV;
  lBPV.push_back (300.0);
  lBPV.push_back (200.0);
  lSC.setBidPriceVector (lBPV);
  BOOST_CHECK_EQUAL (lSC.getBidPriceVector().size(), 2U);
  lSC.getCurrentBidPrice();
  lSC.activateFareFamily();
  BOOST_CHECK (lSC.getFareFamilyStatus());

  // Convex hull made of policies
  stdair::Policy& lPolicy1 =
    stdair::FacBom<stdair::Policy>::instance().create (stdair::PolicyKey ("P1"));
  stdair::Policy& lPolicy2 =
    stdair::FacBom<stdair::Policy>::instance().create (stdair::PolicyKey ("P2"));
  lSC.resetConvexHull();
  BOOST_CHECK_EQUAL (lSC.describeConvexHull(), "");
  lSC.addPolicy (lPolicy1);
  lSC.addPolicy (lPolicy2);
  BOOST_CHECK_EQUAL (lSC.getConvexHull().size(), 2U);
  const std::string lHull = lSC.describeConvexHull();
  BOOST_CHECK (lHull.find ("P1") != std::string::npos);
  BOOST_CHECK (lHull.find ("P2") != std::string::npos);
  lSC.resetConvexHull();
  BOOST_CHECK (lSC.getConvexHull().empty());

  // Booking classes of the cabin
  BOOST_REQUIRE (stdair::BomManager::hasList<stdair::FareFamily> (lSC));
  const stdair::FareFamilyList_T& lFFList =
    stdair::BomManager::getList<stdair::FareFamily> (lSC);
  stdair::FareFamily& lFF = *lFFList.front();
  BOOST_REQUIRE (stdair::BomManager::hasList<stdair::BookingClass> (lFF));
  stdair::BookingClass& lBC =
    *stdair::BomManager::getList<stdair::BookingClass> (lFF).front();
  BOOST_CHECK_EQUAL (lBC.toString(), lBC.describeKey());
  BOOST_CHECK_EQUAL (streamBom (lBC), lBC.describeKey());
  BOOST_CHECK (lBC.getParent() == &lFF);

  const stdair::NbOfBookings_T lInitialBookings = lBC.getNbOfBookings();
  lBC.sell (5.0);
  lBC.cancel (2.0);
  BOOST_CHECK_EQUAL (lBC.getNbOfBookings(), lInitialBookings + 3.0);
  BOOST_CHECK_EQUAL (lBC.getNbOfCancellations(), 2.0);

  lBC.setStdDev (0.0);
  lBC.generateDemandSamples (10);
  BOOST_CHECK (lBC.getGeneratedDemandVector().empty());
  lBC.setMean (20.0);
  lBC.setStdDev (4.0);
  lBC.generateDemandSamples (10);
  BOOST_CHECK_EQUAL (lBC.getGeneratedDemandVector().size(), 10U);
  const stdair::GeneratedDemandVector_T lFirstDraw =
    lBC.getGeneratedDemandVector();
  lBC.generateDemandSamples (10, stdair::DEFAULT_RANDOM_SEED);
  BOOST_CHECK (lBC.getGeneratedDemandVector() == lFirstDraw);

  lBC.setAuthorizationLevel (30.0);
  BOOST_CHECK_EQUAL (lBC.getAuthorizationLevel(), 30.0);
  lBC.setProtection (4.0);
  BOOST_CHECK_EQUAL (lBC.getProtection(), 4.0);
  lBC.setCumulatedProtection (8.0);
  BOOST_CHECK_EQUAL (lBC.getCumulatedProtection(), 8.0);
  lBC.setCumulatedBookingLimit (25.0);
  BOOST_CHECK_EQUAL (lBC.getCumulatedBookingLimit(), 25.0);
  lBC.setSegmentAvailability (12.0);
  BOOST_CHECK_EQUAL (lBC.getSegmentAvailability(), 12.0);
  lBC.setYield (120.0);
  lBC.setAdjustedYield (110.0);
  BOOST_CHECK_EQUAL (lBC.getYield(), 120.0);
  BOOST_CHECK_EQUAL (lBC.getAdjustedYield(), 110.0);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (segment_snapshot_table_test, BomTreeFixture) {
  stdair::BomRoot& lBomRoot = root();
  stdair::SegmentCabin& lSC =
    stdair::BomRetriever::retrieveDummySegmentCabin (lBomRoot);

  const stdair::SegmentSnapshotTableKey lKey (7);
  BOOST_CHECK_EQUAL (lKey.toString(), "7");
  BOOST_CHECK_EQUAL (streamKey (lKey), "SegmentSnapshotTableKey: 7");
  stdair::SegmentSnapshotTableKey lCopyKey (lKey);
  BOOST_CHECK (!archiveKey (lCopyKey).empty());

  stdair::SegmentSnapshotTable& lTable =
    stdair::FacBom<stdair::SegmentSnapshotTable>::instance().create (lKey);
  stdair::FacBomManager::addToListAndMap (lBomRoot, lTable);
  stdair::FacBomManager::linkWithParent (lBomRoot, lTable);
  BOOST_CHECK_EQUAL (lTable.getTableID(), 7);
  BOOST_CHECK_EQUAL (lTable.toString(), "7");
  BOOST_CHECK_EQUAL (streamBom (lTable), "7");
  BOOST_CHECK (lTable.getParent() == &lBomRoot);
  BOOST_CHECK (stdair::BomManager::getObjectPtr<stdair::SegmentSnapshotTable>
               (lBomRoot, "7") == &lTable);
  lSC.setSegmentSnapshotTable (lTable);
  BOOST_CHECK (&lSC.getSegmentSnapshotTable() == &lTable);

  stdair::SegmentCabinIndexMap_T lSCIndexMap;
  lSCIndexMap.insert (stdair::SegmentCabinIndexMap_T::value_type (&lSC, 0));
  stdair::ClassIndexMap_T lClassIndexMap;
  lClassIndexMap.insert (stdair::ClassIndexMap_T::value_type ("X;Q", 0));
  lClassIndexMap.insert (stdair::ClassIndexMap_T::value_type ("X;Y", 1));
  lClassIndexMap.insert (stdair::ClassIndexMap_T::value_type ("X", 2));
  lTable.initSnapshotBlocks (lSCIndexMap, lClassIndexMap);
  BOOST_CHECK_EQUAL (lTable.getSegmentCabinIndexMap().size(), 1U);
  BOOST_CHECK_EQUAL (lTable.getClassIndexMap().size(), 3U);
  BOOST_CHECK_EQUAL (lTable.getClassIndex ("X;Y"), 1);
  BOOST_CHECK_EQUAL (lTable.getSegmentDataID (lSC), 0);

  const stdair::DTD_T lDTD = 10;
  const stdair::SegmentCabinDTDSnapshotView_T lBkgView =
    lTable.getSegmentCabinDTDBookingSnapshotView (0, 0, lDTD);
  BOOST_CHECK_EQUAL (lBkgView.size(), 3U);
  lTable.getSegmentCabinDTDBookingSnapshotView (0, 0, lDTD)[1] = 4.0;
  BOOST_CHECK_EQUAL (lTable.getConstSegmentCabinDTDBookingSnapshotView
                     (0, 0, lDTD)[1], 4.0);
  BOOST_CHECK_EQUAL (lTable.getSegmentCabinDTDRangeBookingSnapshotView
                     (0, 0, lDTD, lDTD + 2)[1][0], 4.0);
  BOOST_CHECK_EQUAL (lTable.getConstSegmentCabinDTDRangeBookingSnapshotView
                     (0, 0, lDTD, lDTD + 2).shape()[1], 3U);

#define STDAIR_CHECK_SNAPSHOT_VIEWS(NAME)                                    \
  {                                                                          \
    lTable.getSegmentCabinDTD##NAME##SnapshotView (0, 0, lDTD)[2] = 5.0;     \
    BOOST_CHECK_EQUAL (lTable.getConstSegmentCabinDTD##NAME##SnapshotView    \
                       (0, 0, lDTD)[2], 5.0);                                \
    BOOST_CHECK_EQUAL (lTable.getSegmentCabinDTDRange##NAME##SnapshotView    \
                       (0, 0, lDTD - 1, lDTD)[2][1], 5.0);                   \
    BOOST_CHECK_EQUAL (lTable.getConstSegmentCabinDTDRange##NAME##SnapshotView \
                       (0, 0, lDTD - 1, lDTD)[2][1], 5.0);                   \
  }
  STDAIR_CHECK_SNAPSHOT_VIEWS (Cancellation)
  STDAIR_CHECK_SNAPSHOT_VIEWS (ProductOrientedNetBooking)
  STDAIR_CHECK_SNAPSHOT_VIEWS (PriceOrientedNetBooking)
  STDAIR_CHECK_SNAPSHOT_VIEWS (ProductOrientedGrossBooking)
  STDAIR_CHECK_SNAPSHOT_VIEWS (PriceOrientedGrossBooking)
  STDAIR_CHECK_SNAPSHOT_VIEWS (Availability)
#undef STDAIR_CHECK_SNAPSHOT_VIEWS
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (policy_test, EmptyBomTreeFixture) {
  const stdair::PolicyKey lKey ("0;1;2");
  BOOST_CHECK_EQUAL (lKey.toString(), "0;1;2");
  BOOST_CHECK_EQUAL (streamKey (lKey), "PolicyKey: 0;1;2");
  stdair::PolicyKey lCopyKey (lKey);
  BOOST_CHECK (!archiveKey (lCopyKey).empty());

  stdair::Policy& lPolicy =
    stdair::FacBom<stdair::Policy>::instance().create (lKey);
  BOOST_CHECK (lPolicy.getParent() == NULL);
  BOOST_CHECK_EQUAL (lPolicy.describeKey(), "0;1;2");
  BOOST_CHECK (streamBom (lPolicy).find ("0;1;2") == 0);
  lPolicy.setDemand (12.5);
  lPolicy.setStdDev (3.25);
  lPolicy.setYield (150.0);
  BOOST_CHECK_EQUAL (lPolicy.getDemand(), 12.5);
  BOOST_CHECK_EQUAL (lPolicy.getStdDev(), 3.25);
  BOOST_CHECK_EQUAL (lPolicy.getYield(), 150.0);
  BOOST_CHECK_EQUAL (lPolicy.toString(), "0;1;2; 12.50; 3.25; 150.00\n");

  BOOST_CHECK_EQUAL (lPolicy.getTotalRevenue(), 0.0);
  lPolicy.addYieldDemand (100.0, 2.0);
  lPolicy.addYieldDemand (50.0, 4.0);
  lPolicy.addYieldDemand (100.0, 1.0);
  BOOST_CHECK_CLOSE (lPolicy.getTotalRevenue(), 100.0 * 3.0 + 50.0 * 4.0, 1e-9);
  lPolicy.resetDemandForecast();
  BOOST_CHECK_EQUAL (lPolicy.getTotalRevenue(), 0.0);

  // Booking classes attached to the policy
  stdair::BookingClass& lQ = stdair::FacBom<stdair::BookingClass>::instance()
    .create (stdair::BookingClassKey ("Q"));
  stdair::FacBomManager::addToList (lPolicy, lQ);
  BOOST_CHECK_EQUAL (lPolicy.getBookingClassList().size(), 1U);
  BOOST_CHECK (lPolicy.getBookingClassList().front() == &lQ);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (nesting_structure_test, BomTreeFixture) {
  const stdair::NestingStructureKey lNSKey
    (stdair::YIELD_BASED_NESTING_STRUCTURE_CODE);
  BOOST_CHECK_EQUAL (lNSKey.toString(),
                     stdair::YIELD_BASED_NESTING_STRUCTURE_CODE);
  BOOST_CHECK_EQUAL (streamKey (lNSKey), "NestingStructureKey: "
                     + stdair::YIELD_BASED_NESTING_STRUCTURE_CODE);
  stdair::NestingStructureKey lCopyNSKey (lNSKey);
  BOOST_CHECK (!archiveKey (lCopyNSKey).empty());

  const stdair::NestingNodeKey lNodeKey ("1");
  BOOST_CHECK_EQUAL (lNodeKey.toString(), "1");
  BOOST_CHECK_EQUAL (streamKey (lNodeKey), "NestingNodeKey: 1");
  stdair::NestingNodeKey lCopyNodeKey (lNodeKey);
  BOOST_CHECK (!archiveKey (lCopyNodeKey).empty());

  stdair::SegmentCabin& lSC =
    stdair::BomRetriever::retrieveDummySegmentCabin (root());
  stdair::SimpleNestingStructure& lNS =
    stdair::FacBom<stdair::SimpleNestingStructure>::instance().create (lNSKey);
  stdair::FacBomManager::addToListAndMap (lSC, lNS);
  stdair::FacBomManager::linkWithParent (lSC, lNS);
  BOOST_CHECK (lNS.getParent() == &lSC);
  BOOST_CHECK_EQUAL (lNS.toString(), stdair::YIELD_BASED_NESTING_STRUCTURE_CODE);
  BOOST_CHECK_EQUAL (streamBom (lNS),
                     stdair::YIELD_BASED_NESTING_STRUCTURE_CODE);

  stdair::NestingNode& lNode1 =
    stdair::FacBom<stdair::NestingNode>::instance().create (lNodeKey);
  stdair::NestingNode& lNode2 = stdair::FacBom<stdair::NestingNode>::instance()
    .create (stdair::NestingNodeKey ("2"));
  stdair::FacBomManager::addToListAndMap (lNS, lNode1);
  stdair::FacBomManager::addToListAndMap (lNS, lNode2);
  stdair::FacBomManager::linkWithParent (lNS, lNode1);
  stdair::FacBomManager::linkWithParent (lNS, lNode2);
  BOOST_CHECK_EQUAL (lNS.getNestingNodeList().size(), 2U);
  BOOST_CHECK (lNode1.getParent() == &lNS);
  lNode1.setYield (200.0);
  BOOST_CHECK_EQUAL (lNode1.getYield(), 200.0);
  BOOST_CHECK_EQUAL (lNode1.toString(), "1200\n");
  BOOST_CHECK (streamBom (lNode1).find ("1") == 0);

  stdair::BookingClass& lBC = stdair::FacBom<stdair::BookingClass>::instance()
    .create (stdair::BookingClassKey ("Y"));
  stdair::FacBomManager::addToList (lNode1, lBC);
  stdair::FacBomManager::addToList (lNode2, lBC);
  BOOST_CHECK_EQUAL (stdair::BomManager::getList<stdair::BookingClass>
                     (lNode1).size(), 1U);

  stdair::FacBomManager::resetYieldBasedNestingStructure (lSC);
  BOOST_CHECK_EQUAL (lNode1.getYield(), -1.0);
  BOOST_CHECK_EQUAL (lNode2.getYield(), -1.0);
  BOOST_CHECK (stdair::BomManager::getList<stdair::BookingClass>
               (lNode1).empty());
  BOOST_CHECK (stdair::BomManager::getList<stdair::BookingClass>
               (lNode2).empty());

  // A node without any booking-class holder can not be reset
  stdair::NestingNode& lNode3 = stdair::FacBom<stdair::NestingNode>::instance()
    .create (stdair::NestingNodeKey ("3"));
  stdair::FacBomManager::addToListAndMap (lNS, lNode3);
  BOOST_CHECK_THROW (stdair::FacBomManager::resetYieldBasedNestingStructure
                     (lSC), stdair::NonInitialisedContainerException);
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (bucket_test, BomTreeFixture) {
  const stdair::BucketKey lKey (3);
  BOOST_CHECK_EQUAL (lKey.toString(), "3");
  BOOST_CHECK_EQUAL (streamKey (lKey), "BucketKey: 3\n");
  stdair::BucketKey lCopyKey (lKey);
  BOOST_CHECK (!archiveKey (lCopyKey).empty());

  stdair::LegCabin& lLC = stdair::BomRetriever::retrieveDummyLegCabin (root());
  stdair::Bucket& lBucket =
    stdair::FacBom<stdair::Bucket>::instance().create (lKey);
  stdair::FacBomManager::addToListAndMap (lLC, lBucket);
  stdair::FacBomManager::linkWithParent (lLC, lBucket);
  BOOST_CHECK (lBucket.getParent() == &lLC);
  BOOST_CHECK_EQUAL (lBucket.getSeatIndex(), 3);
  BOOST_CHECK_EQUAL (lBucket.toString(), "3");
  BOOST_CHECK_EQUAL (streamBom (lBucket), "3");
  lBucket.setYieldRangeUpperValue (250.0);
  lBucket.setAvailability (12.0);
  lBucket.setSoldSeats (4.0);
  BOOST_CHECK_EQUAL (lBucket.getYieldRangeUpperValue(), 250.0);
  BOOST_CHECK_EQUAL (lBucket.getAvailability(), 12.0);
  BOOST_CHECK_EQUAL (lBucket.getSoldSeats(), 4.0);
  BOOST_CHECK (stdair::BomManager::getObjectPtr<stdair::Bucket> (lLC, "3")
               == &lBucket);

  // Cloning the whole persistent BOM tree duplicates the bucket too
  _service.clonePersistentBom();
  stdair::BomRoot& lCloneRoot = _service.getBomRoot();
  BOOST_CHECK (&lCloneRoot != &root());
  stdair::LegCabin& lCloneLC =
    stdair::BomRetriever::retrieveDummyLegCabin (lCloneRoot);
  BOOST_CHECK (&lCloneLC != &lLC);
  BOOST_REQUIRE (stdair::BomManager::hasList<stdair::Bucket> (lCloneLC));
  const stdair::Bucket& lCloneBucket =
    *stdair::BomManager::getList<stdair::Bucket> (lCloneLC).front();
  BOOST_CHECK (&lCloneBucket != &lBucket);
  BOOST_CHECK_EQUAL (lCloneBucket.getSeatIndex(), 3);
  BOOST_CHECK_EQUAL (lCloneBucket.getYieldRangeUpperValue(), 250.0);
  BOOST_CHECK_EQUAL (lCloneBucket.getSoldSeats(), 4.0);
  BOOST_CHECK_EQUAL (stdair::BomManager::getList<stdair::Inventory>
                     (lCloneRoot).size(),
                     stdair::BomManager::getList<stdair::Inventory>
                     (root()).size());
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (airline_class_list_test, EmptyBomTreeFixture) {
  stdair::AirlineCodeList_T lAirlines;
  lAirlines.push_back ("SQ");
  lAirlines.push_back ("CX");
  stdair::ClassList_StringList_T lClasses;
  lClasses.push_back ("Y");
  lClasses.push_back ("M");
  stdair::AirlineClassListKey lKey (lAirlines, lClasses);
  BOOST_CHECK_EQUAL (lKey.toString(), "SQ Y, CX M");
  BOOST_CHECK_EQUAL (streamKey (lKey), "AirlineClassListKey: SQ Y, CX M\n");
  stdair::AirlineClassListKey lCopyKey (lKey);
  BOOST_CHECK_EQUAL (lCopyKey.toString(), lKey.toString());
  BOOST_CHECK (!archiveKey (lCopyKey).empty());

  stdair::AirlineClassList& lACL =
    stdair::FacBom<stdair::AirlineClassList>::instance().create (lKey);
  BOOST_CHECK (lACL.getParent() == NULL);
  BOOST_CHECK_EQUAL (lACL.getAirlineCodeList().size(), 2U);
  BOOST_CHECK_EQUAL (lACL.getClassCodeList().size(), 2U);
  lACL.setYield (100.0);
  lACL.setFare (150.0);
  BOOST_CHECK_EQUAL (lACL.getYield(), 100.0);
  BOOST_CHECK_EQUAL (lACL.getFare(), 150.0);
  BOOST_CHECK_EQUAL (lACL.toString(), "SQ Y, CX M, 100, 150");
  BOOST_CHECK_EQUAL (streamBom (lACL), "SQ Y, CX M, 100, 150");
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (tree_keys_test, EmptyBomTreeFixture) {
  stdair::InventoryKey lInvKey ("BA");
  BOOST_CHECK_EQUAL (streamKey (lInvKey), "InventoryKey: BA");
  BOOST_CHECK (!archiveKey (lInvKey).empty());

  stdair::FlightDateKey lFDKey (9, stdair::Date_T (2011, 6, 10));
  BOOST_CHECK (streamKey (lFDKey).find ("FlightDateKey: 9") == 0);
  BOOST_CHECK (!archiveKey (lFDKey).empty());

  stdair::SegmentDateKey lSDKey ("LHR", "SYD");
  BOOST_CHECK_EQUAL (lSDKey.toString(), "LHR, SYD");
  BOOST_CHECK_EQUAL (streamKey (lSDKey), "SegmentDateKey: LHR, SYD\n");
  BOOST_CHECK (!archiveKey (lSDKey).empty());
  BOOST_CHECK_EQUAL (lSDKey.toString(), "LHR, SYD");

  stdair::LegDateKey lLDKey ("LHR");
  BOOST_CHECK_EQUAL (streamKey (lLDKey), "LegDateKey: LHR");

  stdair::LegCabinKey lLCKey ("Y");
  BOOST_CHECK_EQUAL (streamKey (lLCKey), "LegCabinKey: Y\n");
  BOOST_CHECK (!archiveKey (lLCKey).empty());

  stdair::SegmentCabinKey lSCKey ("Y");
  BOOST_CHECK_EQUAL (streamKey (lSCKey), "SegmentCabinKey: Y");
  BOOST_CHECK (!archiveKey (lSCKey).empty());

  stdair::FareFamilyKey lFFKey ("EcoSaver");
  BOOST_CHECK_EQUAL (streamKey (lFFKey), "FareFamilyKey: EcoSaver");
  BOOST_CHECK (!archiveKey (lFFKey).empty());

  stdair::BookingClassKey lBCKey ("Q");
  BOOST_CHECK_EQUAL (streamKey (lBCKey), "BookingClassKey: Q");

  stdair::AirlineFeatureKey lAFKey ("BA");
  BOOST_CHECK_EQUAL (streamKey (lAFKey), "AirlineFeatureKey: BA\n");

  stdair::YieldFeaturesKey lYFKey (stdair::TRIP_TYPE_ROUND_TRIP, "Y");
  BOOST_CHECK (streamKey (lYFKey).find ("YieldFeaturesKey: ") == 0);

  stdair::BomRootKey lRootKey ("Persistent");
  BOOST_CHECK_EQUAL (lRootKey.toString(), "Persistent");
  BOOST_CHECK_EQUAL (streamKey (lRootKey), "BomRootKey: Persistent\n");
  BOOST_CHECK (!archiveKey (lRootKey).empty());

  stdair::OnDStringList_T lOnDList;
  lOnDList.push_back ("SQ;11,2010-Mar-08;SIN,BKK;08:20:00");
  stdair::OnDDateKey lOnDKey (lOnDList);
  BOOST_CHECK (streamKey (lOnDKey).find ("OnDDateKey: ") == 0);
  BOOST_CHECK (!archiveKey (lOnDKey).empty());
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (airline_feature_and_yield_features_test,
                         EmptyBomTreeFixture) {
  stdair::AirlineFeature& lFeature =
    stdair::FacBom<stdair::AirlineFeature>::instance()
    .create (stdair::AirlineFeatureKey ("SQ"));
  lFeature.init (stdair::ForecastingMethod::Q_FORECASTING,
                 stdair::UnconstrainingMethod::EM,
                 stdair::PreOptimisationMethod::NONE,
                 stdair::OptimisationMethod::LEG_BASED_EMSR_B,
                 365, "A",
                 stdair::PartnershipTechnique::NONE);
  BOOST_CHECK (lFeature.getForecastingMethod()
               == stdair::ForecastingMethod::Q_FORECASTING);
  BOOST_CHECK (lFeature.getOptimisationMethod()
               == stdair::OptimisationMethod::LEG_BASED_EMSR_B);
  BOOST_CHECK (lFeature.toString().find ("SQ") == 0);
  BOOST_CHECK (lFeature.toString().find ("365") != std::string::npos);

  stdair::YieldFeatures& lRT = stdair::FacBom<stdair::YieldFeatures>::instance()
    .create (stdair::YieldFeaturesKey (stdair::TRIP_TYPE_ROUND_TRIP, "Y"));
  BOOST_CHECK_EQUAL (lRT.toString(), lRT.describeKey());
  BOOST_CHECK_EQUAL (streamBom (lRT), lRT.describeKey());
  BOOST_CHECK_EQUAL (lRT.getTripType(), stdair::TRIP_TYPE_ROUND_TRIP);
  BOOST_CHECK (lRT.isTripTypeValid (stdair::TRIP_TYPE_ROUND_TRIP));
  BOOST_CHECK (lRT.isTripTypeValid (stdair::TRIP_TYPE_INBOUND));
  BOOST_CHECK (lRT.isTripTypeValid (stdair::TRIP_TYPE_OUTBOUND));
  BOOST_CHECK (!lRT.isTripTypeValid (stdair::TRIP_TYPE_ONE_WAY));
  stdair::YieldFeatures& lOW = stdair::FacBom<stdair::YieldFeatures>::instance()
    .create (stdair::YieldFeaturesKey (stdair::TRIP_TYPE_ONE_WAY, "Y"));
  BOOST_CHECK (lOW.isTripTypeValid (stdair::TRIP_TYPE_ONE_WAY));
  BOOST_CHECK (!lOW.isTripTypeValid (stdair::TRIP_TYPE_INBOUND));
}

// ////////////////////////////////////////////////////////////////////
BOOST_FIXTURE_TEST_CASE (ond_date_test, EmptyBomTreeFixture) {
  stdair::OnDStringList_T lOnDList;
  lOnDList.push_back ("SQ;11,2010-Mar-08;SIN,BKK;08:20:00");
  stdair::OnDDate& lOnD = stdair::FacBom<stdair::OnDDate>::instance()
    .create (stdair::OnDDateKey (lOnDList));
  BOOST_CHECK_EQUAL (lOnD.toString(), lOnD.describeKey());
  BOOST_CHECK_EQUAL (streamBom (lOnD), lOnD.describeKey());
  BOOST_CHECK_EQUAL (lOnD.getNbOfSegments(), 1);

  stdair::Inventory& lInv = stdair::FacBom<stdair::Inventory>::instance()
    .create (stdair::InventoryKey ("SQ"));
  stdair::FacBomManager::addToList (lInv, lOnD);
  stdair::FacBomManager::linkWithParent (lInv, lOnD);
  BOOST_CHECK_EQUAL (lOnD.getAirlineCode(), "SQ");

  stdair::CabinClassPairList_T lPath;
  lPath.push_back (stdair::CabinClassPair_T ("Y", "M"));
  const stdair::YieldDemandPair_T lFirst (100.0,
                                          stdair::MeanStdDevPair_T (5.0, 1.0));
  const stdair::YieldDemandPair_T lSecond (120.0,
                                           stdair::MeanStdDevPair_T (6.0, 2.0));
  lOnD.setDemandInformation (lPath, lFirst);
  lOnD.setDemandInformation (lPath, lSecond);
  BOOST_REQUIRE_EQUAL (lOnD.getDemandInfoMap().size(), 1U);
  BOOST_CHECK_EQUAL (lOnD.getDemandInfoMap().begin()->second.first, 120.0);
  BOOST_CHECK_EQUAL (lOnD.getCabinClassPairList ("Y:M;").size(), 1U);

  lOnD.setTotalForecast ("Y", stdair::WTPDemandPair_T
                         (300.0, stdair::MeanStdDevPair_T (10.0, 3.0)));
  BOOST_CHECK_EQUAL (lOnD.getTotalForecast ("Y").first, 300.0);
  BOOST_CHECK_EQUAL (lOnD.getTotalForecastMap().size(), 1U);
}

BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
