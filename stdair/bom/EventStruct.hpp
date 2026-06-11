#ifndef STDAIR_BAS_EVENTSTRUCT_HPP
#define STDAIR_BAS_EVENTSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <memory>
#include <string>
#include <variant>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/stdair_event_types.hpp>
#include <stdair/stdair_exceptions.hpp>
#include <stdair/basic/StructAbstract.hpp>
#include <stdair/basic/EventType.hpp>
#include <stdair/bom/EventTypes.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/bom/OptimisationNotificationTypes.hpp>
#include <stdair/bom/SnapshotTypes.hpp>
#include <stdair/bom/CancellationTypes.hpp>
#include <stdair/bom/RMEventTypes.hpp>
#include <stdair/bom/BreakPointTypes.hpp>

namespace stdair {

  /**
   * Structure holding the details of an event.
   *
   * \note No event should be scheduled before the date-time
   *       corresponding to the DEFAULT_EVENT_OLDEST_DATETIME constant
   *       (as of Feb. 2011, that date is set to Jan. 1, 2010). That
   *       constant is specified in the stdair/basic/BasConst.cpp
   *       file. In other words, the simulation should not specified
   *       to start before that date-time.
   */
  struct EventStruct : public StructAbstract {

    /**
     * Payload of the event: one of the event-specific structures.
     */
    using EventPayload_T = std::variant<
        std::monostate,
        std::shared_ptr<BookingRequestStruct>,
        std::shared_ptr<CancellationStruct>,
        std::shared_ptr<OptimisationNotificationStruct>,
        std::shared_ptr<SnapshotStruct>,
        std::shared_ptr<RMEventStruct>,
        std::shared_ptr<BreakPointStruct>
    >;

    // ///////////// Getters ///////////
  public:
    /** Get the event type */
    const EventType::EN_EventType& getEventType() const {
      return _eventType;
    }  

    /** Get the event time stamp */
    const LongDuration_T& getEventTimeStamp() const {
      return _eventTimeStamp;
    }  

    /** Get the event date-time i.e the stamp converted to a date-time format. */
    const DateTime_T& getEventTime () const;

    /**
     * Get a reference on the booking request referred to by event.
     *
     * \note When that event is not of type booking request
     *       (EventType::BKG_REQ), an EventException is thrown.
     */
    const BookingRequestStruct& getBookingRequest() const {
      return getPayload<BookingRequestStruct> ("booking request");
    }

    /**
     * Get a reference on the cancellation referred to by event.
     *
     * \note When that event is not of type cancellation
     *       (EventType::CX), an EventException is thrown.
     */
    const CancellationStruct& getCancellation() const {
      return getPayload<CancellationStruct> ("cancellation");
    }

    /**
     * Get a reference on the optimisation notification referred to by event.
     *
     * \note When that event is not of type optimisation notification
     * for optimisation notification (EventType::OPT_NOT_4_FD), an
     * EventException is thrown.
     */
    const OptimisationNotificationStruct&
    getOptimisationNotificationStruct() const {
      return getPayload<OptimisationNotificationStruct> ("optimisation "
                                                         "notification");
    }
    
    /**
     * Get a reference on the snapshot referred to by event.
     *
     * \note When that event is not of type snapshot
     * (EventType::SNAPSHOT), an EventException is thrown.
     */
    const SnapshotStruct& getSnapshotStruct() const {
      return getPayload<SnapshotStruct> ("snapshot");
    }
    
    /**
     * Get a reference on the RM event referred to by the generic event.
     *
     * \note When that event is not of type RM event
     * (EventType::RM), an EventException is thrown.
     */
    const RMEventStruct& getRMEvent() const {
      return getPayload<RMEventStruct> ("RM event");
    }   

    /**
     * Get a reference on the break point referred to by event.
     *
     * \note When that event is not of type break point
     *       (EventType::BRK_PT), an EventException is thrown.
     */
    const BreakPointStruct& getBreakPoint() const {
      return getPayload<BreakPointStruct> ("break point");
    }

    // ////////// Display methods //////////
  public:
    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

    /** Display of the structure. */
    const std::string describe() const;

    
    // ////////// Constructors and destructors /////////
  public:
    /** Default constructor. */
    EventStruct();
    /** Constructor for events corresponding to booking requests. */
    EventStruct (const EventType::EN_EventType&, BookingRequestPtr_T);
    /** Constructor for events corresponding to cancellations. */
    EventStruct (const EventType::EN_EventType&, CancellationPtr_T);
    /** Constructor for events corresponding to optimisation requests. */
    EventStruct (const EventType::EN_EventType&, const DateTime_T& iDCPDate,
                 OptimisationNotificationPtr_T);
    /** Constructor for events corresponding to snapshot requests. */
    EventStruct (const EventType::EN_EventType&, SnapshotPtr_T);
    /** Constructor for events corresponding to RM events. */
    EventStruct (const EventType::EN_EventType&, RMEventPtr_T);   
    /** Constructor for events corresponding to Break Point events. */
    EventStruct (const EventType::EN_EventType&, BreakPointPtr_T);
    /** Copy constructor. */
    EventStruct (const EventStruct&);

    /** Destructor. */
    ~EventStruct();    

    // ////////// Modifiers /////////
  public:
    /**
     * Increment the date-time stamp which is counted in milliseconds.
     * 
     * This incrementation of one millisecond is needed when the
     * insertion in the event queue failed, that is to say when an
     * event with the exact same time stamp has already been inserted
     * in the queue.
     */
    void incrementEventTimeStamp();
    
    // ////////////////// Helpers //////////////////
  private:
    /**
     * Retrieve the payload of the given type, throwing an
     * EventException when the event does not hold such a payload.
     */
    template <typename PayloadType>
    const PayloadType& getPayload (const char* iPayloadName) const {
      const auto* lPayloadPtr_ptr =
        std::get_if<std::shared_ptr<PayloadType>> (&_payload);
      if (lPayloadPtr_ptr == nullptr || *lPayloadPtr_ptr == nullptr) {
        throw EventException (std::string ("The event does not hold a ")
                              + iPayloadName + " payload");
      }
      return **lPayloadPtr_ptr;
    }

    // ////////////////// Attributes //////////////////
  private:
    /**
     * Event type.
     */
    EventType::EN_EventType _eventType;

    /**
     * Reprentative timestamp of the event, which is the number of
     * milliseconds between the event date-time and the default
     * event date-time (2010-01-01 00:00:00).
     */
    LongDuration_T _eventTimeStamp;
    
    /**
     * Payload of the event (booking request, cancellation,
     * optimisation notification, snapshot, RM event or break point).
     */
    EventPayload_T _payload;
  };

}
#endif // STDAIR_BAS_EVENTSTRUCT_HPP
