#ifndef MYPROVIDER_TYPES_HPP
#define MYPROVIDER_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <typeinfo>
#include <map>

// Forward declarations
namespace stdair {
  class BomAbstract;
}

namespace myprovider {
  typedef std::map<const std::type_info*, stdair::BomAbstract*> HolderMap_T;  
}  
#endif // MYPROVIDER_TYPES_HPP
