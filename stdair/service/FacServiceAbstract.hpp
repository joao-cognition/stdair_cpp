#ifndef __STDAIR_SVC_FACSERVICEABSTRACT_HPP
#define __STDAIR_SVC_FACSERVICEABSTRACT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <memory>
#include <vector>
// StdAir
#include <stdair/service/ServiceAbstract.hpp>

namespace stdair {

  /** Base class for the (Service) Factory layer. */
  class FacServiceAbstract {
  public:
    
    /** Define the list (pool) of Service objects. */
    typedef std::vector<std::unique_ptr<ServiceAbstract>> ServicePool_T;
    
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
#endif // __STDAIR_SVC_FACSERVICEABSTRACT_HPP
