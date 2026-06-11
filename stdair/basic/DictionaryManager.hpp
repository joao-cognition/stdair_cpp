// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BASIC_DICTIONARYMANAGER_HPP
#define STDAIR_BASIC_DICTIONARYMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_maths_types.hpp>

namespace stdair {

  // //////////// Type definitions /////////////////
  /**
   * Dictionary key.
   */
  using DictionaryKey_T = unsigned short;
  
  /**
   * @brief Class wrapper of dictionary business methods.
   */
  class DictionaryManager {
  public:
    // //////////// Business methods /////////////////
    /**
     * Convert from key to value.
     */
    static stdair::Probability_T keyToValue (const DictionaryKey_T);
    
    /**
     * Convert from value to key.
     */
    static DictionaryKey_T valueToKey (const stdair::Probability_T);
  };
}
#endif // STDAIR_BASIC_DICTIONARYMANAGER_HPP
