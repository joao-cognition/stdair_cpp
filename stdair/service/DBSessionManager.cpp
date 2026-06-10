// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <memory>
#include <string>
#include <sstream>
// SOCI
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/BasDBParams.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/service/Logger.hpp>

namespace stdair {
  
  // //////////////////////////////////////////////////////////////////////
  DBSessionManager::DBSessionManager () {
  }

  // //////////////////////////////////////////////////////////////////////
  DBSessionManager::~DBSessionManager () {
    // std::cout << "In DBSessionManager destructor" << std::endl;
    dbFinalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void DBSessionManager::dbInit (const BasDBParams& iDBParams) {

    // Database parameters
    std::ostringstream oStr;
    oStr << "db=" << iDBParams.getDBName() << " user=" << iDBParams.getUser()
         << " password=" << iDBParams.getPassword()
         << " port=" << iDBParams.getPort() << " host=" << iDBParams.getHost();
    const std::string lDBSessionConnectionString (oStr.str());

    // Instanciate the database session: nothing else is performed at that stage
    _dbSession = std::make_unique<DBSession_T>();
    
    try {
      // Open the connection to the database
      _dbSession->open (soci::mysql, lDBSessionConnectionString);
      
    } catch (std::exception const& lException) {
      std::ostringstream oMessage;
      oMessage <<"Error while opening a connection to database: "
               << lException.what() << std::endl
               << "Database parameters used:"
               << " db=" << iDBParams.getDBName()
               << " user=" << iDBParams.getUser()
               << " port=" << iDBParams.getPort()
               << " host=" << iDBParams.getHost();
      throw SQLDatabaseConnectionImpossibleException (oMessage.str());
    }
  }
    
  // //////////////////////////////////////////////////////////////////////
  void DBSessionManager::dbFinalise () {
    _dbSession.reset();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DBSessionManager::init (const BasDBParams& iDBParams) {
    DBSessionManager& lInstance = instance();
    lInstance.dbInit (iDBParams);
  }
  
  // //////////////////////////////////////////////////////////////////////
  DBSessionManager& DBSessionManager::instance() {
    static DBSessionManager _instance;
    return _instance;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DBSessionManager::clean() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  DBSession_T& DBSessionManager::getDBSession() const {
    if (!_dbSession) {
      throw NonInitialisedDBSessionManagerException ("");
    }
    assert (_dbSession != nullptr);
    return *_dbSession;
  }
  
}
