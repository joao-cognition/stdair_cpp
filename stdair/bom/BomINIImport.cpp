// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <sstream>
// Boost Property Tree
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomINIImport.hpp>
#include <stdair/bom/ConfigHolderStruct.hpp>
#include <stdair/service/Logger.hpp>

namespace bpt = boost::property_tree;

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  void BomINIImport::importINIConfig (ConfigHolderStruct& iConfigHolder,
				      const ConfigINIFile& iConfigINIFile) {

    // Get the config file name.
    const stdair::Filename_T lFilename = iConfigINIFile.name();

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (lFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_DEBUG ("The config input file '" << lFilename
                       << "' can not be retrieved on the file-system.");
      return;
    }
    STDAIR_LOG_DEBUG ("Load the config input file '" << lFilename
                      << "' content into the configuration holder.");
    

    // Transform the INI file into a BOOST property tree.
    bpt::ptree pt;
    bpt::ini_parser::read_ini(lFilename, pt); 
    // Add the property tree to the configuration structure.
    iConfigHolder.add(pt);

  }
  
}
