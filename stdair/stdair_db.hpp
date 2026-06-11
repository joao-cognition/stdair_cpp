#ifndef STDAIR_STDAIR_DB_HPP
#define STDAIR_STDAIR_DB_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

// Forward declarations
namespace soci {
  class session;
  class statement;
}

namespace stdair {

  // //////// Type definitions /////////
  /** Database session handler. */
  using DBSession_T = soci::session;
  
  /** Database request statement handler. */
  using DBRequestStatement_T = soci::statement;
  
  /** Define the name of an database connection. */
  using DBConnectionName_T = std::string;
  
}
#endif // STDAIR_STDAIR_DB_HPP
