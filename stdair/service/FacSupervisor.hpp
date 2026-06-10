#ifndef __STDAIR_SVC_FACSUPERVISOR_HPP
#define __STDAIR_SVC_FACSUPERVISOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <vector>

namespace stdair {

  /// Forward declarations
  class FacAbstract;
  class FacServiceAbstract;

  /**
   * Singleton class to register and clean all Factories.
   */
  class FacSupervisor {
  public:
    /**
     * Define the pool (vector) of factories.
     *
     * The pointers are non-owning: the factories are static-local
     * (Meyer's) singletons whose lifetime is managed by the runtime.
     */
    typedef std::vector<FacAbstract*> PersistentBomFactoryPool_T;
    typedef std::vector<FacAbstract*> CloneBomFactoryPool_T;
    typedef std::vector<FacServiceAbstract*> ServiceFactoryPool_T;

    /**
     * Provide the unique (static) instance of the FacSupervisor object.
     *
     * The singleton is instantiated when first used.
     * @return FacSupervisor&
     */
    static FacSupervisor& instance(); 

    /**
     * Register a newly instantiated persistent factory for the Bom layer.
     *
     * When a concrete Factory is firstly instantiated
     * this factory have to register itself to the FacSupervisor
     * @param FacAbstract* The concrete Factory to register (non-owning).
     */
    void registerPersistentBomFactory (FacAbstract*);

    /**
     * Register a newly instantiated concrete factory for the Bom layer.
     *
     * When a concrete Factory is firstly instantiated
     * this factory have to register itself to the FacSupervisor
     * @param FacAbstract* The concrete Factory to register (non-owning).
     */
    void registerCloneBomFactory (FacAbstract*);

    /**
     * Register a newly instantiated concrete factory for the Service layer.
     *
     * When a concrete Factory is firstly instantiated
     * this factory have to register itself to the FacSupervisor.
     * @param FacServiceAbstract* The concrete Factory to register (non-owning).
     */
    void registerServiceFactory (FacServiceAbstract*);  

    /**
     * Clean all the persistent registered object.
     *
     * Call the clean method of all the instantiated persistent factories
     * for the BomStructure layer.
     */
    void cleanPersistentBomLayer();

    /**
     * Clean all the clone registered object.
     *
     * Call the clean method of all the instantiated factories
     * for the BomStructure layer.
     */
    void cleanCloneBomLayer();

    /**
     * Clean all Service created object.
     *
     * Call the clean method of all the instantiated  factories
     * for the Service layer.
     */
    void cleanServiceLayer();

    /**
     * Delete the static instance of the Logger object.
     */
    static void cleanLoggerService();

    /**
     * Delete the static instance of the DBSessionManager object.
     */
    static void cleanDBSessionManager();

    /**
     * Clean all the layers (BOM and service), as well as the Logger
     * and DBSessionManager static instances.
     */
    static void cleanAll();

    /**
     * Destructor.
     */
    ~FacSupervisor();

    // Copy is not allowed (singleton)
    FacSupervisor (const FacSupervisor&) = delete;
    FacSupervisor& operator= (const FacSupervisor&) = delete;


  protected:
    /**
     * Default Constructor.
     *
     * This constructor is protected to ensure the singleton pattern.
     */
    FacSupervisor() {}

  private:
    /**
     * List of instantiated persistent factory for the BOM layer.
     */
    PersistentBomFactoryPool_T _persistentBomPool;

    /**
     * List of instantiated factories for the BOM layer.
     */
    CloneBomFactoryPool_T _cloneBomPool;

    /**
     * List of instantiated factories for the Service layer.
     */
    ServiceFactoryPool_T _svcPool;

  };
}  
#endif // __STDAIR_SVC_FACSUPERVISOR_HPP
