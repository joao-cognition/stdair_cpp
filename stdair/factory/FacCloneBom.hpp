#ifndef __STDAIR_FAC_FACCLONEBOM_HPP
#define __STDAIR_FAC_FACCLONEBOM_HPP

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

    /// Deleter granted access to the protected BOM destructor.
    struct BomDeleter {
      void operator() (BOM* ioBom_ptr) const {
        delete ioBom_ptr;
      }
    };

    /// Internal type definitions.
    typedef std::unique_ptr<BOM, BomDeleter> BomUniquePtr_T;
    typedef std::vector<BomUniquePtr_T> BomPool_T;
    typedef typename BOM::Key_T Key_T;


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
    ~FacCloneBom() {
      clean();
    }

    // Copy is not allowed (singleton)
    FacCloneBom (const FacCloneBom&) = delete;
    FacCloneBom& operator= (const FacCloneBom&) = delete;

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
    static FacCloneBom inst;
    static bool lRegistered =
      (FacSupervisor::instance().registerCloneBomFactory (&inst), true);
    (void) lRegistered;
    return inst;
  }

  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> void FacCloneBom<BOM>::clean () {
    // Empty the pool: unique_ptr handles the deletion of the objects
    _pool.clear();
  }
  
  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> BOM& FacCloneBom<BOM>::clone (const BOM& iBom) {
    BomUniquePtr_T lBom_ptr (new BOM (iBom));
    BOM& lBom = *lBom_ptr;
    _pool.push_back (std::move (lBom_ptr));
    return lBom;
  }
  
}
#endif // __STDAIR_FAC_FACCLONEBOM_HPP
