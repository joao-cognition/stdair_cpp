#ifndef STDAIR_FAC_FACCLONEBOM_HPP
#define STDAIR_FAC_FACCLONEBOM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <memory>
#include <string>
#include <vector>
// StdAir 
#include <stdair/factory/FacAbstract.hpp>
#include <stdair/service/FacSupervisor.hpp>
#include <stdair/service/Logger.hpp>

namespace stdair {  

  /**
   * @brief Base class for Factory layer.
   */
  template <typename BOM>
  class FacCloneBom : public FacAbstract {

    /// Internal type definitions.
    /**
     * Deleter granted access to the protected destructor of the BOM
     * objects, thanks to the friendship between the BOM classes and
     * FacCloneBom.
     */
    struct BomDeleter {
      void operator() (BOM* ioBom_ptr) const {
        delete ioBom_ptr;
      }
    };
    using BomUniquePtr_T = std::unique_ptr<BOM, BomDeleter>;
    using BomPool_T = std::vector<BomUniquePtr_T>;
    using Key_T = typename BOM::Key_T;


  public:
    // ///////////// Business methods ////////////
    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used.
     * @return FacCloneBom&
     */
    static FacCloneBom& instance();

    /**
     * Clone a BOM object.
     */
    BOM& clone (const BOM&);
    
  protected:
    /**
     * Default Constructor.
     */
    FacCloneBom() {}

  public:
    /**
     * Destructor.
     */
    ~FacCloneBom() override {
      clean();
    }

    /**
     * Destroyed all the object instantiated by this factory.
     */
    void clean() override;
    

  private:
    // ///////////////////// Attributes //////////////////
    /**
     * List of instantiated Business Objects.
     */
    BomPool_T _pool;
  };


  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> FacCloneBom<BOM>& FacCloneBom<BOM>::instance () {
    static FacCloneBom lInstance;
    static const bool lRegistered =
      (FacSupervisor::instance().registerCloneBomFactory (&lInstance),
       true);
    (void) lRegistered;
    return lInstance;
  }

  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> void FacCloneBom<BOM>::clean () {
    // Destroy all the objects and empty the pool
    _pool.clear();
  }
  
  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> BOM& FacCloneBom<BOM>::clone (const BOM& iBom) {
    _pool.push_back (BomUniquePtr_T (new BOM (iBom)));
    return *_pool.back();
  }
  
}
#endif // STDAIR_FAC_FACCLONEBOM_HPP
