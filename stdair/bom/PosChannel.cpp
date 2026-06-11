// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_Request.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/bom/PosChannel.hpp>

namespace stdair {

  // ////////////////////////////////////////////////////////////////////
  PosChannel::PosChannel()
    : _key (DEFAULT_POS,
            DEFAULT_CHANNEL),
      _parent (nullptr)  {
    // That constructor is used by the serialisation process
  }

  // ////////////////////////////////////////////////////////////////////
  PosChannel::PosChannel (const PosChannel& iPosChannel)
    : BomAbstract(), _key (iPosChannel.getKey()), _parent (nullptr)  {
  }

  // ////////////////////////////////////////////////////////////////////
  PosChannel::PosChannel (const Key_T& iKey)
    : _key (iKey), _parent (nullptr)  {
  }

  // ////////////////////////////////////////////////////////////////////
  PosChannel::~PosChannel () {
  }
  
  // ////////////////////////////////////////////////////////////////////
  std::string PosChannel::toString() const {
    std::ostringstream oStr;
    oStr << describeKey();
    return oStr.str();
  }
}
