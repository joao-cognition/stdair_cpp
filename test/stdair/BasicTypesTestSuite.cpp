/*!
 * \page BasicTypesTestSuite_cpp Unit tests for the stdair/basic value types
 * \code
 */
// STL
#include <sstream>
#include <string>
#include <limits>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/basic/BasLogParams.hpp>
#include <stdair/basic/DemandGenerationMethod.hpp>
#include <stdair/basic/DictionaryManager.hpp>
#include <stdair/basic/EventType.hpp>
#include <stdair/basic/ForecastingMethod.hpp>
#include <stdair/basic/JSonCommand.hpp>
#include <stdair/basic/OptimisationMethod.hpp>
#include <stdair/basic/PartnershipTechnique.hpp>
#include <stdair/basic/PassengerChoiceModel.hpp>
#include <stdair/basic/PassengerType.hpp>
#include <stdair/basic/PreOptimisationMethod.hpp>
#include <stdair/basic/ProgressStatus.hpp>
#include <stdair/basic/ProgressStatusSet.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/basic/SampleType.hpp>
#include <stdair/basic/ServiceInitialisationType.hpp>
#include <stdair/basic/UnconstrainingMethod.hpp>
#include <stdair/basic/YieldRange.hpp>

BOOST_AUTO_TEST_SUITE (basic_types_test_suite)

BOOST_AUTO_TEST_CASE (event_type_test) {
  const char lChars[] = { 'B', 'X', 'F', 'N', 'C', 'S', 'R', 'P' };
  for (unsigned short idx = 0; idx != stdair::EventType::LAST_VALUE; ++idx) {
    const stdair::EventType::EN_EventType lEnum =
      static_cast<stdair::EventType::EN_EventType> (idx);
    const stdair::EventType lFromChar (lChars[idx]);
    const stdair::EventType lFromLabel (stdair::EventType::getLabel (lEnum));
    const stdair::EventType lCopy (lFromChar);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lFromLabel == lEnum);
    BOOST_CHECK_EQUAL (lCopy.getType(), lEnum);
    BOOST_CHECK_EQUAL (stdair::EventType::getTypeLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (stdair::EventType::getTypeLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getTypeAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(),
                       stdair::EventType::getLabel (lEnum));
  }
  BOOST_CHECK_EQUAL (stdair::EventType (stdair::EventType::BKG_REQ).describe(),
                     "BookingRequest");
  BOOST_CHECK (stdair::EventType::describeLabels().find ("Snapshot")
               != std::string::npos);
  BOOST_CHECK_THROW (stdair::EventType ('Z'),
                     stdair::CodeConversionException);
  BOOST_CHECK_THROW (stdair::EventType (std::string ("Unknown")),
                     stdair::CodeConversionException);
  std::ostringstream oStr;
  oStr << stdair::EventType (stdair::EventType::CX);
  BOOST_CHECK_EQUAL (oStr.str(), "Cancellation");
}

BOOST_AUTO_TEST_CASE (demand_generation_method_test) {
  typedef stdair::DemandGenerationMethod DGM;
  const char lChars[] = { 'P', 'S' };
  for (unsigned short idx = 0; idx != DGM::LAST_VALUE; ++idx) {
    const DGM::EN_DemandGenerationMethod lEnum =
      static_cast<DGM::EN_DemandGenerationMethod> (idx);
    const DGM lFromChar (lChars[idx]);
    const DGM lFromEnum (lEnum);
    const DGM lFromLabel (DGM::getMethodLabelAsString (lEnum));
    const DGM lCopy (lFromLabel);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getMethod(), lEnum);
    BOOST_CHECK_EQUAL (DGM::getMethod (lChars[idx]), lEnum);
    BOOST_CHECK_EQUAL (DGM::getMethodLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (DGM::getMethodLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getMethodAsChar(), lChars[idx]);
    BOOST_CHECK_EQUAL (lFromChar.getMethodAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), DGM::getLabel (lEnum));
  }
  BOOST_CHECK_EQUAL (DGM::describeLabels(), "PoissonProcess, StatisticsOrder");
  BOOST_CHECK_THROW (DGM::getMethod ('Z'), stdair::CodeConversionException);
  BOOST_CHECK_THROW (DGM ('Z'), stdair::CodeConversionException);
  BOOST_CHECK_THROW (DGM (std::string ("Z")),
                     stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (service_initialisation_type_test) {
  typedef stdair::ServiceInitialisationType SIT;
  const char lChars[] = { 'N', 'F', 'B' };
  for (unsigned short idx = 0; idx != SIT::LAST_VALUE; ++idx) {
    const SIT::EN_ServiceInitialisationType lEnum =
      static_cast<SIT::EN_ServiceInitialisationType> (idx);
    const SIT lFromChar (lChars[idx]);
    const SIT lFromEnum (lEnum);
    const SIT lFromLabel (SIT::getTypeLabelAsString (lEnum));
    const SIT lCopy (lFromLabel);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getType(), lEnum);
    BOOST_CHECK_EQUAL (SIT::getType (lChars[idx]), lEnum);
    BOOST_CHECK_EQUAL (SIT::getTypeLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (SIT::getTypeLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getTypeAsChar(), lChars[idx]);
    BOOST_CHECK_EQUAL (lFromChar.getTypeAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), SIT::getLabel (lEnum));
  }
  BOOST_CHECK (SIT::describeLabels().find ("File parsing")
               != std::string::npos);
  BOOST_CHECK_THROW (SIT::getType ('Z'), stdair::CodeConversionException);
  BOOST_CHECK_THROW (SIT ('Z'), stdair::CodeConversionException);
  BOOST_CHECK_THROW (SIT (std::string ("Z")),
                     stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (partnership_technique_test) {
  typedef stdair::PartnershipTechnique PT;
  for (unsigned short idx = 0; idx != PT::LAST_VALUE; ++idx) {
    const PT::EN_PartnershipTechnique lEnum =
      static_cast<PT::EN_PartnershipTechnique> (idx);
    const char lChar = PT::getTechniqueLabel (lEnum);
    const PT lFromChar (lChar);
    const PT lFromEnum (lEnum);
    const PT lFromLabel (PT::getTechniqueLabelAsString (lEnum));
    const PT lCopy (lFromLabel);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getTechnique(), lEnum);
    BOOST_CHECK_EQUAL (PT::getTechnique (lChar), lEnum);
    BOOST_CHECK_EQUAL (PT::getTechniqueLabelAsString (lEnum),
                       std::string (1, lChar));
    BOOST_CHECK_EQUAL (lFromChar.getTechniqueAsChar(), lChar);
    BOOST_CHECK_EQUAL (lFromChar.getTechniqueAsString(),
                       std::string (1, lChar));
    BOOST_CHECK_EQUAL (lFromChar.describe(), PT::getLabel (lEnum));
  }
  BOOST_CHECK_EQUAL (PT::getTechniqueLabel (PT::NONE), 'N');
  BOOST_CHECK (PT::describeLabels().find ("None") != std::string::npos);
  BOOST_CHECK_THROW (PT::getTechnique ('Z'), stdair::CodeConversionException);
  BOOST_CHECK_THROW (PT ('Z'), stdair::CodeConversionException);
  BOOST_CHECK_THROW (PT (std::string ("Z")),
                     stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (sample_type_test) {
  typedef stdair::SampleType ST;
  const char lChars[] = { 'A', 'P', 'R', 'I', 'S', 'T', 'F', 'C', 'D', 'E', 'M' };
  for (unsigned short idx = 0; idx != ST::LAST_VALUE; ++idx) {
    const ST::EN_SampleType lEnum = static_cast<ST::EN_SampleType> (idx);
    const ST lFromChar (lChars[idx]);
    const ST lFromEnum (lEnum);
    const ST lCopy (lFromChar);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getType(), lEnum);
    BOOST_CHECK_EQUAL (ST::getTypeLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (ST::getTypeLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getTypeAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), ST::getLabel (lEnum));
  }
  BOOST_CHECK_EQUAL (ST::getLabel (ST::INV), "Inventory");
  BOOST_CHECK (ST::describeLabels().find ("CustomerChoice")
               != std::string::npos);
  BOOST_CHECK_THROW (ST ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (passenger_choice_model_test) {
  typedef stdair::PassengerChoiceModel PCM;
  const char lChars[] = { 'R', 'P', 'H' };
  for (unsigned short idx = 0; idx != PCM::LAST_VALUE; ++idx) {
    const PCM::EN_PassengerChoiceModel lEnum =
      static_cast<PCM::EN_PassengerChoiceModel> (idx);
    const PCM lFromChar (lChars[idx]);
    const PCM lFromEnum (lEnum);
    const PCM lCopy (lFromChar);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getModel(), lEnum);
    BOOST_CHECK_EQUAL (PCM::getModelLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (PCM::getModelLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getModelAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), PCM::getLabel (lEnum));
  }
  BOOST_CHECK (PCM::describeLabels().find ("HardRestrictionModel") != std::string::npos);
  BOOST_CHECK_THROW (PCM ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (forecasting_method_test) {
  typedef stdair::ForecastingMethod FM;
  const char lChars[] = { 'Q', 'H', 'O', 'N', 'B' };
  for (unsigned short idx = 0; idx != FM::LAST_VALUE; ++idx) {
    const FM::EN_ForecastingMethod lEnum =
      static_cast<FM::EN_ForecastingMethod> (idx);
    const FM lFromChar (lChars[idx]);
    const FM lFromEnum (lEnum);
    const FM lCopy (lFromChar);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getMethod(), lEnum);
    BOOST_CHECK_EQUAL (FM::getMethodLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (FM::getMethodLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getMethodAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), FM::getLabel (lEnum));
  }
  BOOST_CHECK (FM::describeLabels().find ("New QFF") != std::string::npos);
  BOOST_CHECK_THROW (FM ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (passenger_type_test) {
  typedef stdair::PassengerType PT;
  const char lChars[] = { 'L', 'B', 'F' };
  for (unsigned short idx = 0; idx != PT::LAST_VALUE; ++idx) {
    const PT::EN_PassengerType lEnum = static_cast<PT::EN_PassengerType> (idx);
    const PT lFromChar (lChars[idx]);
    const PT lFromEnum (lEnum);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getType(), lEnum);
    BOOST_CHECK_EQUAL (PT::getTypeLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (PT::getTypeLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getTypeAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), PT::getLabel (lEnum));
  }
  BOOST_CHECK_EQUAL (PT::describeLabels(), "Leisure, Business, First");
  BOOST_CHECK_THROW (PT ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (pre_optimisation_method_test) {
  typedef stdair::PreOptimisationMethod POM;
  const char lChars[] = { 'N', 'F', 'M' };
  for (unsigned short idx = 0; idx != POM::LAST_VALUE; ++idx) {
    const POM::EN_PreOptimisationMethod lEnum =
      static_cast<POM::EN_PreOptimisationMethod> (idx);
    const POM lFromChar (lChars[idx]);
    const POM lFromEnum (lEnum);
    const POM lCopy (lFromChar);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getMethod(), lEnum);
    BOOST_CHECK_EQUAL (POM::getMethodLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (POM::getMethodLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getMethodAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), POM::getLabel (lEnum));
  }
  BOOST_CHECK (POM::describeLabels().find ("Fare Adjustment")
               != std::string::npos);
  BOOST_CHECK_THROW (POM ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (optimisation_method_test) {
  typedef stdair::OptimisationMethod OM;
  const char lChars[] = { 'M', 'E' };
  for (unsigned short idx = 0; idx != OM::LAST_VALUE; ++idx) {
    const OM::EN_OptimisationMethod lEnum =
      static_cast<OM::EN_OptimisationMethod> (idx);
    const OM lFromChar (lChars[idx]);
    const OM lFromEnum (lEnum);
    const OM lCopy (lFromChar);
    BOOST_CHECK (lFromChar == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lFromEnum.getMethod(), lEnum);
    BOOST_CHECK_EQUAL (OM::getMethodLabel (lEnum), lChars[idx]);
    BOOST_CHECK_EQUAL (OM::getMethodLabelAsString (lEnum),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.getMethodAsString(),
                       std::string (1, lChars[idx]));
    BOOST_CHECK_EQUAL (lFromChar.describe(), OM::getLabel (lEnum));
  }
  BOOST_CHECK (OM::describeLabels().find ("Leg based Monte Carlo") != std::string::npos);
  BOOST_CHECK_THROW (OM ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (unconstraining_method_test) {
  typedef stdair::UnconstrainingMethod UM;
  const UM lFromChar ('E');
  const UM lFromEnum (UM::EM);
  const UM lCopy (lFromChar);
  BOOST_CHECK (lFromChar == UM::EM);
  BOOST_CHECK (lCopy == UM::EM);
  BOOST_CHECK_EQUAL (lFromEnum.getMethod(), UM::EM);
  BOOST_CHECK_EQUAL (UM::getMethodLabel (UM::EM), 'E');
  BOOST_CHECK_EQUAL (UM::getMethodLabelAsString (UM::EM), "E");
  BOOST_CHECK_EQUAL (lFromChar.getMethodAsString(), "E");
  BOOST_CHECK_EQUAL (lFromChar.describe(), "Expectation-Maximisation");
  BOOST_CHECK (UM::describeLabels().find ("Expectation-Maximisation") != std::string::npos);
  BOOST_CHECK_THROW (UM ('Z'), stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (json_command_test) {
  typedef stdair::JSonCommand JC;
  const std::string lLabels[] = { "list", "flight_date", "event_list",
                                  "break_point", "run", "reset", "status",
                                  "config" };
  for (unsigned short idx = 0; idx != JC::LAST_VALUE; ++idx) {
    const JC::EN_JSonCommand lEnum = static_cast<JC::EN_JSonCommand> (idx);
    const JC lFromLabel (lLabels[idx]);
    const JC lCopy (lFromLabel);
    BOOST_CHECK (lFromLabel == lEnum);
    BOOST_CHECK (lCopy == lEnum);
    BOOST_CHECK_EQUAL (lCopy.getCommand(), lEnum);
    BOOST_CHECK_EQUAL (JC::getCommand (lLabels[idx]), lEnum);
    BOOST_CHECK_EQUAL (JC::getLabel (lEnum), lLabels[idx]);
    BOOST_CHECK_EQUAL (lFromLabel.describe(), lLabels[idx]);
  }
  BOOST_CHECK (JC::describeLabels().find ("break_point") != std::string::npos);
  BOOST_CHECK_THROW (JC::getCommand ("unknown"),
                     stdair::CodeConversionException);
  BOOST_CHECK_THROW (JC (std::string ("unknown")),
                     stdair::CodeConversionException);
}

BOOST_AUTO_TEST_CASE (yield_range_test) {
  const stdair::YieldRange lDefault;
  BOOST_CHECK_EQUAL (lDefault.getUpperYield(),
                     std::numeric_limits<stdair::Yield_T>::max());
  BOOST_CHECK_EQUAL (lDefault.getLowerYield(),
                     std::numeric_limits<stdair::Yield_T>::min());

  const stdair::YieldRange lSingle (100.0);
  BOOST_CHECK_EQUAL (lSingle.getUpperYield(), 100.0);
  BOOST_CHECK_EQUAL (lSingle.getAverageYield(), 100.0);
  BOOST_CHECK_EQUAL (lSingle.getLowerYield(), 100.0);

  const stdair::YieldRange lPair (200.0, 150.0);
  BOOST_CHECK_EQUAL (lPair.getUpperYield(), 200.0);
  BOOST_CHECK_EQUAL (lPair.getAverageYield(), 150.0);

  stdair::YieldRange lTriple (300.0, 200.0, 100.0);
  const stdair::YieldRange lCopy (lTriple);
  BOOST_CHECK_EQUAL (lCopy.getUpperYield(), 300.0);
  BOOST_CHECK_EQUAL (lCopy.getAverageYield(), 200.0);

  lTriple.setUpperYield (310.0);
  lTriple.setAverageYield (210.0);
  lTriple.setLowerYield (110.0);
  std::ostringstream oStr;
  lTriple.toStream (oStr);
  BOOST_CHECK_EQUAL (oStr.str(), "210([110, 310])");
  std::istringstream iStr ("ignored");
  lTriple.fromStream (iStr);
  BOOST_CHECK (lTriple.describe().empty());
}

BOOST_AUTO_TEST_CASE (progress_status_test) {
  stdair::ProgressStatus lStatus (10, 100, 50);
  BOOST_CHECK_EQUAL (lStatus.count(), 10);
  BOOST_CHECK_EQUAL (lStatus.getCurrentNb(), 10);
  BOOST_CHECK_EQUAL (lStatus.getExpectedNb(), 100);
  BOOST_CHECK_EQUAL (lStatus.getActualNb(), 50);
  BOOST_CHECK_CLOSE (lStatus.progress(), 20.0, 0.001);
  BOOST_CHECK_EQUAL (lStatus.describe(), "10 / {100, 50}");
  BOOST_CHECK (lStatus.toString().find ("(10/50)") != std::string::npos);

  lStatus += 5;
  ++lStatus;
  BOOST_CHECK_EQUAL (lStatus.getCurrentNb(), 16);
  lStatus.setCurrentNb (1);
  lStatus.setExpectedNb (2);
  lStatus.setActualNb (4);
  BOOST_CHECK_EQUAL (lStatus.describe(), "1 / {2, 4}");
  lStatus.reset();
  BOOST_CHECK_EQUAL (lStatus.getCurrentNb(), 0);
  BOOST_CHECK_EQUAL (lStatus.getActualNb(), 0);
  BOOST_CHECK_EQUAL (lStatus.progress(), 0.0);

  const stdair::ProgressStatus lPair (20, 30);
  BOOST_CHECK_EQUAL (lPair.getExpectedNb(), 20);
  BOOST_CHECK_EQUAL (lPair.getActualNb(), 30);
  const stdair::ProgressStatus lSingle (40);
  BOOST_CHECK_EQUAL (lSingle.getActualNb(), 40);
  const stdair::ProgressStatus lDefault;
  BOOST_CHECK_EQUAL (lDefault.getExpectedNb(), 0);
  const stdair::ProgressStatus lCopy (lSingle);
  BOOST_CHECK_EQUAL (lCopy.getExpectedNb(), 40);
}

BOOST_AUTO_TEST_CASE (progress_status_set_test) {
  stdair::ProgressStatusSet lSet (stdair::EventType::BKG_REQ);
  lSet.setTypeSpecificStatus (stdair::ProgressStatus (1, 10, 10));
  lSet.setSpecificGeneratorStatus (stdair::ProgressStatus (2, 20, 20), "GEN");
  lSet.setOverallStatus (stdair::ProgressStatus (3, 30, 30));
  BOOST_CHECK_EQUAL (lSet.getTypeSpecificStatus().getCurrentNb(), 1);
  BOOST_CHECK_EQUAL (lSet.getSpecificGeneratorStatus().getCurrentNb(), 2);
  BOOST_CHECK_EQUAL (lSet.getOverallStatus().getCurrentNb(), 3);

  const stdair::ProgressStatusSet lCopy (lSet);
  const std::string lDescription = lCopy.describe();
  BOOST_CHECK (lDescription.find ("[Overall][3/{30,30}]") != std::string::npos);
  BOOST_CHECK (lDescription.find ("BookingRequest") != std::string::npos);
  BOOST_CHECK (lDescription.find ("GEN") != std::string::npos);
  std::istringstream iStr ("ignored");
  lSet.fromStream (iStr);
}

BOOST_AUTO_TEST_CASE (random_generation_test) {
  stdair::RandomGeneration lGen1 (42);
  stdair::RandomGeneration lGen2 (42);
  for (unsigned short idx = 0; idx != 20; ++idx) {
    const stdair::RealNumber_T lValue = lGen1.generateUniform01();
    BOOST_CHECK (lValue >= 0.0 && lValue < 1.0);
    BOOST_CHECK_EQUAL (lValue, lGen2());
  }
  const stdair::RealNumber_T lUniform = lGen1.generateUniform (10.0, 20.0);
  BOOST_CHECK (lUniform >= 10.0 && lUniform <= 20.0);
  const stdair::RealNumber_T lExp = lGen1.generateExponential (2.0);
  BOOST_CHECK (lExp >= 0.0);
  const stdair::RealNumber_T lNormal = lGen1.generateNormal (100.0, 1.0);
  BOOST_CHECK (lNormal > 80.0 && lNormal < 120.0);

  stdair::RandomGeneration lDefault;
  lDefault.init (42);
  stdair::RandomGeneration lFresh (42);
  BOOST_CHECK_EQUAL (lDefault.describe(), lFresh.describe());
  BOOST_CHECK_EQUAL (lDefault.generateUniform01(), lFresh.generateUniform01());
  BOOST_CHECK (&lDefault.getBaseGenerator() != NULL);
}

BOOST_AUTO_TEST_CASE (db_params_test) {
  stdair::BasDBParams lParams ("user", "pass", "localhost", "3306", "stdair");
  BOOST_CHECK (lParams.check());
  BOOST_CHECK_EQUAL (lParams.getUser(), "user");
  BOOST_CHECK_EQUAL (lParams.getPassword(), "pass");
  BOOST_CHECK_EQUAL (lParams.getHost(), "localhost");
  BOOST_CHECK_EQUAL (lParams.getPort(), "3306");
  BOOST_CHECK_EQUAL (lParams.getDBName(), "stdair");
  BOOST_CHECK_EQUAL (lParams.toShortString(), "stdair.user@localhost:3306");
  BOOST_CHECK_EQUAL (lParams.toString(), "stdair.user@localhost:3306");
  BOOST_CHECK_EQUAL (lParams.describe(), "stdair.user@localhost:3306");

  stdair::BasDBParams lCopy (lParams);
  lCopy.setUser ("u2");
  lCopy.setPassword ("p2");
  lCopy.setHost ("h2");
  lCopy.setPort ("1");
  lCopy.setDBName ("db2");
  BOOST_CHECK_EQUAL (lCopy.toString(), "db2.u2@h2:1");
  lCopy.setPassword ("");
  BOOST_CHECK (lCopy.check() == false);
  BOOST_CHECK (stdair::BasDBParams().check() == false);
}

BOOST_AUTO_TEST_CASE (log_params_test) {
  std::ostringstream oLog;
  stdair::BasLogParams lParams (stdair::LOG::NOTIFICATION, oLog);
  BOOST_CHECK_EQUAL (lParams.getLogLevel(), stdair::LOG::NOTIFICATION);
  BOOST_CHECK (&lParams.getLogStream() == &oLog);
  BOOST_CHECK (lParams.getForcedInitialisationFlag() == false);
  BOOST_CHECK_EQUAL (lParams.toShortString(), lParams.toString());
  BOOST_CHECK_EQUAL (lParams.describe(), lParams.toString());

  stdair::BasLogParams lCopy (lParams);
  lCopy.setForcedInitialisationFlag (true);
  BOOST_CHECK (lCopy.getForcedInitialisationFlag());
  BOOST_CHECK (lCopy.toShortString().find ("(forced)") != std::string::npos);
}

BOOST_AUTO_TEST_CASE (file_mgr_chronometer_dictionary_test) {
  BOOST_CHECK (stdair::BasFileMgr::doesExistAndIsReadable ("/no/such/file")
               == false);
  BOOST_CHECK (stdair::BasFileMgr::doesExistAndIsReadable (__FILE__));

  stdair::BasChronometer lChrono;
  lChrono.start();
  BOOST_CHECK (lChrono.elapsed() >= 0.0);
  BOOST_CHECK (lChrono.getStart().empty() == false);

  BOOST_CHECK_CLOSE (stdair::DictionaryManager::keyToValue (500), 0.5, 0.001);
  BOOST_CHECK_EQUAL (stdair::DictionaryManager::valueToKey (1.0), 1000);
}

BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
