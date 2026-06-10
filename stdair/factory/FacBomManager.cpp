// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Inventory.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/SimpleNestingStructure.hpp>
#include <stdair/bom/NestingNode.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>

namespace stdair {
  // ////////////////////////////////////////////////////////////////////
  void FacBomManager::
  resetYieldBasedNestingStructure (const SegmentCabin& iSegmentCabin) {
    const SimpleNestingStructure& lYieldBasedNS =
      BomManager::getObject<SimpleNestingStructure> (iSegmentCabin, YIELD_BASED_NESTING_STRUCTURE_CODE);

    // Browse the list of node and reset each one.
    const NestingNodeList_T& lNestingNodeList =
      BomManager::getList<NestingNode> (lYieldBasedNS);
    for (stdair::NestingNode* lNode_ptr : lNestingNodeList) {
      assert (lNode_ptr != nullptr);

      lNode_ptr->setYield (-1.0);

      // Clear the list of booking classes of the node
      const HolderMap_T& lHolderMap = lNode_ptr->getHolderMap();
      auto itHolder = lHolderMap.find (&typeid (BookingClass));
    
      if (itHolder == lHolderMap.end()) {
        const std::string lName (typeid (BookingClass).name());
        throw NonInitialisedContainerException("Cannot find the holder of type "
                                               + lName + " within: "
                                               + lNode_ptr->describeKey());
      } 
      
      BomHolder<BookingClass>* lBomHolder_ptr = 
        static_cast<BomHolder<BookingClass>*> (itHolder->second);
      assert (lBomHolder_ptr != nullptr);
      
      BookingClassList_T& lBCList = lBomHolder_ptr->_bomList;
      lBCList.clear();
    }
  }

}
