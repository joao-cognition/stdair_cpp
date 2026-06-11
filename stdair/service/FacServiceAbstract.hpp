#ifndef STDAIR_SVC_FACSERVICEABSTRACT_HPP
#define STDAIR_SVC_FACSERVICEABSTRACT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <vector>

namespace stdair {

  // Forward declarations
  class ServiceAbstract;

  /** Base class for the (Service) Factory layer. */
  class FacServiceAbstract {
  public:
    
    /** Define the list (pool) of Service objects. */
    using ServicePool_T = std::vector<ServiceAbstract*>;
    
    /** Destructor. */
    virtual ~FacServiceAbstract();
    
    /** Destroyed all the object instantiated by this factory. */
    void clean();
    
  protected:
    /** Default Constructor.
        <br>This constructor is protected to ensure the class is abstract. */
    FacServiceAbstract() {}
    
    /** List of instantiated Business Objects*/
    ServicePool_T _pool;
  };
    
}
#endif // STDAIR_SVC_FACSERVICEABSTRACT_HPP
