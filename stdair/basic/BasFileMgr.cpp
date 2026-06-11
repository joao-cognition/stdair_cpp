// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost (STL Extension)
// Boost Filesystem (http://www.boost.org/doc/libs/1_41_0/libs/filesystem/doc/index.htm)
#include <boost/version.hpp>
#include <boost/filesystem.hpp>
// StdAir
#include <stdair/basic/BasFileMgr.hpp>

namespace boostfs = boost::filesystem;

namespace stdair {

  // //////////////////////////////////////////////////////////////////////
  bool BasFileMgr::doesExistAndIsReadable (const std::string& iFilepath) {
    bool oFine = false;
    
    boostfs::path lPath (iFilepath);

    if (boostfs::exists (lPath) == false) {
      return oFine;
    }

    if (boostfs::is_regular_file (lPath) == true) {
      oFine = true;
    }

    return oFine;
  }
  
}

