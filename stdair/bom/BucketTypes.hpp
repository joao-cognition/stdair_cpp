// //////////////////////////////////////////////////////////////////////
#ifndef STDAIR_BOM_BUCKETTYPES_HPP
#define STDAIR_BOM_BUCKETTYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>
// StdAir
#include <stdair/bom/key_types.hpp>

namespace stdair {
  
  // Forward declarations
  class Bucket;

  /** Define the bucket list. */
  using BucketList_T = std::list<Bucket*>;
  
  /** Define the bucket map. */
  using BucketMap_T = std::map<const MapKey_T, Bucket*>;
  
}
#endif // STDAIR_BOM_BUCKETTYPES_HPP

