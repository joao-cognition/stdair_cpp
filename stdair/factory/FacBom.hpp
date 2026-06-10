#ifndef __STDAIR_FAC_FACBOM_HPP
#define __STDAIR_FAC_FACBOM_HPP

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
  class FacBom : public FacAbstract {

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
     * @return FacBom&
     */
    static FacBom& instance();

    /**
     * Create a BOM object, given a key or not.
     */
    BOM& create ();
    BOM& create (const Key_T&);
    BOM& create (const BOM&);
    
  protected:
    /**
     * Default Constructor.
     */
    FacBom() {}

  public:
    /**
     * Destructor.
     */
    ~FacBom() {
      clean();
    }

    // Copy is not allowed (singleton)
    FacBom (const FacBom&) = delete;
    FacBom& operator= (const FacBom&) = delete;

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
  template <typename BOM> FacBom<BOM>& FacBom<BOM>::instance () {
    static FacBom inst;
    static bool lRegistered =
      (FacSupervisor::instance().registerPersistentBomFactory (&inst), true);
    (void) lRegistered;
    return inst;
  }

  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> void FacBom<BOM>::clean () {
    // Empty the pool: unique_ptr handles the deletion of the objects
    _pool.clear();
  }
  
  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> BOM& FacBom<BOM>::create () {
    Key_T lKey;
    return instance().create (lKey);
  }
  
  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> BOM& FacBom<BOM>::create (const Key_T& iKey) {
    BomUniquePtr_T lBom_ptr (new BOM (iKey));
    BOM& lBom = *lBom_ptr;
    _pool.push_back (std::move (lBom_ptr));
    return lBom;
  }

  // ////////////////////////////////////////////////////////////////////
  template <typename BOM> BOM& FacBom<BOM>::create (const BOM& iBom) {
    BomUniquePtr_T lBom_ptr (new BOM (iBom));
    BOM& lBom = *lBom_ptr;
    _pool.push_back (std::move (lBom_ptr));
    return lBom;
  }
  
}
#endif // __STDAIR_FAC_FACBOM_HPP
