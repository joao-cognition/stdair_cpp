// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/factory/FacAbstract.hpp>
#include <stdair/service/FacServiceAbstract.hpp>
#include <stdair/service/FacSupervisor.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/DBSessionManager.hpp>

namespace stdair {

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor& FacSupervisor::instance() {
    static FacSupervisor inst;
    return inst;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::~FacSupervisor() {
    cleanPersistentBomLayer();
    cleanCloneBomLayer();
    cleanServiceLayer();
  }  

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::registerPersistentBomFactory (FacAbstract* ioFac_ptr) {
    _persistentBomPool.push_back (ioFac_ptr);
  } 

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::registerCloneBomFactory (FacAbstract* ioFac_ptr) {
    _cloneBomPool.push_back (ioFac_ptr);
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::registerServiceFactory (FacServiceAbstract* ioFac_ptr) {
    _svcPool.push_back (ioFac_ptr);
  } 

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanPersistentBomLayer() { 
    for (FacAbstract* lFactory_ptr : _persistentBomPool) {
      assert (lFactory_ptr != nullptr);
      lFactory_ptr->clean();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanCloneBomLayer() {
    for (FacAbstract* lFactory_ptr : _cloneBomPool) {
      assert (lFactory_ptr != nullptr);
      lFactory_ptr->clean();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanServiceLayer() {
    for (FacServiceAbstract* lFactory_ptr : _svcPool) {
      assert (lFactory_ptr != nullptr);
      lFactory_ptr->clean();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanLoggerService() {
    // Clean the static instance of the log service
    Logger::clean();
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanDBSessionManager() {
    // Clean the static instance of the database service
    DBSessionManager::clean();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanAll() {

    // Clean the static instance of the database session manager
    cleanDBSessionManager();

    // Clean the static instance of the log service
    cleanLoggerService();

    // Clean both the BOM and service layers
    FacSupervisor& lInstance = instance();
    lInstance.cleanPersistentBomLayer();
    lInstance.cleanCloneBomLayer();
    lInstance.cleanServiceLayer();
  }

}
