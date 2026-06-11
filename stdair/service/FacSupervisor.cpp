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
    static FacSupervisor lInstance;
    return lInstance;
  }

  // //////////////////////////////////////////////////////////////////////
  FacSupervisor::~FacSupervisor() {
    // The registered factories are Meyer's singletons: they clean their
    // own pools of objects when they are themselves destroyed, at the
    // end of the program.
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
    for (FacAbstract* lCurrentFactory_ptr : _persistentBomPool) {
      assert (lCurrentFactory_ptr != nullptr);
      lCurrentFactory_ptr->clean();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanCloneBomLayer() {
    for (FacAbstract* lCurrentFactory_ptr : _cloneBomPool) {
      assert (lCurrentFactory_ptr != nullptr);
      lCurrentFactory_ptr->clean();
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void FacSupervisor::cleanServiceLayer() {
    for (FacServiceAbstract* lCurrentFactory_ptr : _svcPool) {
      assert (lCurrentFactory_ptr != nullptr);
      lCurrentFactory_ptr->clean();
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

    // Clean both the BOM and service layers.
    FacSupervisor& lInstance = instance();
    lInstance.cleanPersistentBomLayer();
    lInstance.cleanCloneBomLayer();
    lInstance.cleanServiceLayer();
  }

}
