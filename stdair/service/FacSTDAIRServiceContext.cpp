// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/FacSupervisor.hpp>
#include <stdair/service/FacSTDAIRServiceContext.hpp>
#include <stdair/service/STDAIR_ServiceContext.hpp>

namespace stdair {

  // //////////////////////////////////////////////////////////////////////
  FacSTDAIRServiceContext::~FacSTDAIRServiceContext() {
  }

  // //////////////////////////////////////////////////////////////////////
  FacSTDAIRServiceContext& FacSTDAIRServiceContext::instance() {
    static FacSTDAIRServiceContext lInstance;
    static const bool lRegistered =
      (FacSupervisor::instance().registerServiceFactory (&lInstance), true);
    (void) lRegistered;
    return lInstance;
  }

  // //////////////////////////////////////////////////////////////////////
  STDAIR_ServiceContext& FacSTDAIRServiceContext::create() {
    STDAIR_ServiceContext* aServiceContext_ptr = nullptr;

    aServiceContext_ptr = new STDAIR_ServiceContext ();
    assert (aServiceContext_ptr != nullptr);

    // The new object is added to the Bom pool
    _pool.push_back (aServiceContext_ptr);

    return *aServiceContext_ptr;
  }

}
