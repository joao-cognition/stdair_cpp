#ifndef INTRUSIVE_BOM_BOMMANAGER_HPP
#define INTRUSIVE_BOM_BOMMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace stdair {

  /** Utility class. */
  class BomManager {
  public:
    /** Display the full BOM tree. */
    static std::string display (const BomRoot&);
    
  private:
    
  };

}
#endif // INTRUSIVE_BOM_BOMMANAGER_HPP
