// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <memory>
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
    static FacSTDAIRServiceContext inst;
    static bool lRegistered =
      (FacSupervisor::instance().registerServiceFactory (&inst), true);
    (void) lRegistered;
    return inst;
  }

  // //////////////////////////////////////////////////////////////////////
  STDAIR_ServiceContext& FacSTDAIRServiceContext::create() {
    std::unique_ptr<STDAIR_ServiceContext> aServiceContext_ptr (
      new STDAIR_ServiceContext ());
    STDAIR_ServiceContext& lServiceContext = *aServiceContext_ptr;

    // The new object is added to the service pool
    _pool.push_back (std::move (aServiceContext_ptr));

    return lServiceContext;
  }

}
