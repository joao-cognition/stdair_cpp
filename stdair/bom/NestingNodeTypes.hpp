// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_NESTINGNODETYPES_HPP
#define STDAIR_BOM_NESTINGNODETYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>

namespace stdair {

  // Forward declarations.
  class NestingNode;

  /** Define the fare family list. */
  using NestingNodeList_T = std::list<NestingNode*>;

  /** Define the fare family map. */
  using NestingNodeMap_T = std::map<const MapKey_T, NestingNode*>;
  
}
#endif // STDAIR_BOM_NESTINGNODETYPES_HPP
