// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/AirportPair.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  AirportPair::AirportPair()
    : _key (DEFAULT_ORIGIN, DEFAULT_DESTINATION),
      _parent (nullptr)  {
    // That constructor is used by the serialisation process
  }

  // ////////////////////////////////////////////////////////////////////
  AirportPair::AirportPair (const AirportPair& iAirportPair)
    : BomAbstract(), _key (iAirportPair.getKey()), _parent (nullptr)  {
  }

  // ////////////////////////////////////////////////////////////////////
  AirportPair::AirportPair (const Key_T& iKey)
    : _key (iKey), _parent (nullptr)  {
  }

  // ////////////////////////////////////////////////////////////////////
  AirportPair::~AirportPair () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  std::string AirportPair::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    return oStr.str();
  }
}


