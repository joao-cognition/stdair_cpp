// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <memory>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/BasConst_Event.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/OptimisationNotificationStruct.hpp>
#include <stdair/bom/SnapshotStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/bom/RMEventStruct.hpp>
#include <stdair/bom/BreakPointStruct.hpp>
#include <stdair/bom/EventStruct.hpp>

namespace stdair {

  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct()
    : _eventType (EventType::BKG_REQ), _eventTimeStamp (0) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventType::EN_EventType& iEventType,
                            BookingRequestPtr_T ioRequestPtr)
    : _eventType (iEventType) {

    //
    assert (ioRequestPtr != nullptr);
    const auto lBookingRequest_ptr =
      std::make_shared<BookingRequestStruct> (*ioRequestPtr);
    _payload = lBookingRequest_ptr;
    
    /**
     * Compute and store the number of milliseconds between the
     * date-time of the booking request and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const Duration_T lDuration =
      lBookingRequest_ptr->getRequestDateTime() - DEFAULT_EVENT_OLDEST_DATETIME;
    _eventTimeStamp = lDuration.total_milliseconds();
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventType::EN_EventType& iEventType,
                            CancellationPtr_T ioCancellationPtr)
    : _eventType (iEventType) {

    //
    assert (ioCancellationPtr != nullptr);
    const auto lCancellation_ptr =
      std::make_shared<CancellationStruct> (*ioCancellationPtr);
    _payload = lCancellation_ptr;
    
    /**
     * Compute and store the number of milliseconds between the
     * date-time of the cancellation and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const Duration_T lDuration =
      lCancellation_ptr->getCancellationDateTime()
      - DEFAULT_EVENT_OLDEST_DATETIME;
    _eventTimeStamp = lDuration.total_milliseconds();
  }

  // //////////////////////////////////////////////////////////////////////
  EventStruct::
  EventStruct (const EventType::EN_EventType& iEventType,
	       const DateTime_T& iDCPDate,
	       OptimisationNotificationPtr_T ioOptimisationNotificationPtr)
    : _eventType (iEventType) {

    //
    assert (ioOptimisationNotificationPtr != nullptr);
    _payload = std::make_shared<OptimisationNotificationStruct>
      (*ioOptimisationNotificationPtr);
    
    /**
     * Compute and store the number of milliseconds between the
     * date-time of the booking request and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const Duration_T lDuration = iDCPDate - DEFAULT_EVENT_OLDEST_DATETIME;
    _eventTimeStamp = lDuration.total_milliseconds();
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventType::EN_EventType& iEventType,
                            SnapshotPtr_T ioSnapshotPtr)
    : _eventType (iEventType) {

    //
    assert (ioSnapshotPtr != nullptr);
    const auto lSnapshot_ptr = std::make_shared<SnapshotStruct> (*ioSnapshotPtr);
    _payload = lSnapshot_ptr;
    
    /**
     * Compute and store the number of milliseconds between the
     * date-time of the snapshot and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const Duration_T lDuration =
      lSnapshot_ptr->getSnapshotTime() - DEFAULT_EVENT_OLDEST_DATETIME;
    _eventTimeStamp = lDuration.total_milliseconds();
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventType::EN_EventType& iEventType,
                            RMEventPtr_T ioRMEventPtr)
    : _eventType (iEventType) {

    //
    assert (ioRMEventPtr != nullptr);
    const auto lRMEvent_ptr = std::make_shared<RMEventStruct> (*ioRMEventPtr);
    _payload = lRMEvent_ptr;
    
    /**
     * Compute and store the number of milliseconds between the
     * date-time of the RM event and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const Duration_T lDuration =
      lRMEvent_ptr->getRMEventTime() - DEFAULT_EVENT_OLDEST_DATETIME;
    _eventTimeStamp = lDuration.total_milliseconds();
  }  

  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventType::EN_EventType& iEventType,
                            BreakPointPtr_T ioBreakPointPtr)
    : _eventType (iEventType) {

    //
    assert (ioBreakPointPtr != nullptr);
    const auto lBreakPoint_ptr =
      std::make_shared<BreakPointStruct> (*ioBreakPointPtr);
    _payload = lBreakPoint_ptr;
    
    /**
     * Compute and store the number of milliseconds between the
     * date-time of the RM event and DEFAULT_EVENT_OLDEST_DATETIME
     * (as of Feb. 2011, that date is set to Jan. 1, 2010).
     */
    const Duration_T lDuration =
      lBreakPoint_ptr->getBreakPointTime() - DEFAULT_EVENT_OLDEST_DATETIME;
    _eventTimeStamp = lDuration.total_milliseconds();
  }

  // //////////////////////////////////////////////////////////////////////
  EventStruct::EventStruct (const EventStruct& iEventStruct)
    : _eventType (iEventStruct._eventType),
      _eventTimeStamp (iEventStruct._eventTimeStamp) {

    // Deep-copy the payload, so that each EventStruct owns its own copy
    std::visit ([this](const auto& iPayloadPtr) {
      using PtrType = std::decay_t<decltype (iPayloadPtr)>;
      if constexpr (std::is_same_v<PtrType, std::monostate> == false) {
        if (iPayloadPtr != nullptr) {
          using PayloadType = typename PtrType::element_type;
          _payload = std::make_shared<PayloadType> (*iPayloadPtr);
        }
      }
    }, iEventStruct._payload);
  }
  
  // //////////////////////////////////////////////////////////////////////
  EventStruct::~EventStruct() {
  }

  // //////////////////////////////////////////////////////////////////////
  void EventStruct::fromStream (std::istream& /*ioIn*/) {
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string EventStruct::describe() const {
    std::ostringstream oStr;

    //
    const Duration_T lEventDateTimeDelta =
      boost::posix_time::milliseconds (_eventTimeStamp);
    const DateTime_T lEventDateTime (DEFAULT_EVENT_OLDEST_DATETIME
                                   + lEventDateTimeDelta);

    oStr << lEventDateTime;

    //
    switch (_eventType) {
    case EventType::BKG_REQ: {
      oStr << ", " << EventType::getLabel(_eventType)
	   << ", " << getBookingRequest().describe();
      break;
    }
    case EventType::CX: {
      oStr << ", " << EventType::getLabel(_eventType)
	   << ", " << getCancellation().describe();
      break;
    }
    case EventType::OPT_NOT_4_FD: {
      oStr << ", " << EventType::getLabel(_eventType)
           << ", " << getOptimisationNotificationStruct().describe();
      break;
    }
    case EventType::SNAPSHOT: {
      oStr << ", " << EventType::getLabel(_eventType)
         << ", " << getSnapshotStruct().describe();
      break;
    }
    case EventType::RM: {
      oStr << ", " << EventType::getLabel(_eventType)
         << ", " << getRMEvent().describe();
      break;
    } 
    case EventType::BRK_PT: {
      oStr << ", " << EventType::getLabel(_eventType)
           << ", " << getBreakPoint().describe(); 
      break;
    }
    default: {
      oStr << ", " << _eventType << " (not yet recognised)";
      break;
    }
    }
    
    oStr << "\n";
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  const DateTime_T& EventStruct::getEventTime() const {
    const DateTime_T& lDateTime (DEFAULT_EVENT_OLDEST_DATETIME);

    //
    switch (_eventType) {
    case EventType::BKG_REQ: {
      return getBookingRequest().getRequestDateTime();
    }
    case EventType::CX: {
      return getCancellation().getCancellationDateTime();
    }
    case EventType::OPT_NOT_4_FD: {
      return getOptimisationNotificationStruct().getNotificationDateTime();
    }
    case EventType::SNAPSHOT: {
      return getSnapshotStruct().getSnapshotTime();
    }
    case EventType::RM: {
      return getRMEvent().getRMEventTime();
    } 
    case EventType::BRK_PT: {
      return getBreakPoint().getBreakPointTime();
    }
    default: {
      throw EventException ("Unknown event type: the event date-time "
                            "cannot be retrieved");
    }
    }

    return lDateTime;
  } 

  // //////////////////////////////////////////////////////////////////////
  void EventStruct::incrementEventTimeStamp() {
    // The date-time is counted in milliseconds (1e-3 second). Hence,
    // one thousand (1e3) of attempts correspond to 1 second.
    // Increment the time stamp of one millisecond.
    ++_eventTimeStamp;
  }

}
