#ifndef STDAIR_STDAIR_JSON_HPP
#define STDAIR_STDAIR_JSON_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace stdair {

  /**
   * @brief JSON-formatted string.
   *
   */
  class JSONString { 
  public:
    /**
     * Main Constructor.
     */
    explicit JSONString (const std::string& iJsonString) :
      _jsonString (iJsonString) {}
    /**
     * Default constructor.
     */
    explicit JSONString () : _jsonString ("") {}
    
    /**
     * Destructor.
     */
    virtual ~JSONString() {}
    
    /**
     * Get the string value.
     */
    const std::string& getString() const {
      return _jsonString;
    }
    
  protected:
    /**
     * 
     */
    std::string _jsonString;
  };

}
#endif // STDAIR_STDAIR_JSON_HPP
