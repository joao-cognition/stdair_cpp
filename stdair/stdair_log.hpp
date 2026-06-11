#ifndef STDAIR_STDAIR_LOG_HPP
#define STDAIR_STDAIR_LOG_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <array>
#include <cstddef>
#include <string_view>

namespace stdair {

  // Forward declarations
  class STDAIR_Service;

  // /////////////// Log /////////////
  /** Level of logs. */
  enum class LogLevel : int {
    Critical = 0,
    Error,
    Notification,
    Warning,
    Debug,
    Verbose,
    LastValue
  };

  /** Labels for the log levels. */
  inline constexpr std::array<std::string_view,
                              static_cast<std::size_t> (LogLevel::LastValue)>
  LogLevelLabels {"C", "E", "N", "W", "D", "V"};

  /** Get the label associated with a given log level. */
  inline constexpr std::string_view getLogLevelLabel (const LogLevel iLevel) {
    return LogLevelLabels.at (static_cast<std::size_t> (iLevel));
  }

  /**
   * Backward-compatible aliases, so that existing code using
   * stdair::LOG::DEBUG and stdair::LOG::EN_LogLevel keeps compiling.
   */
  namespace LOG {
    using EN_LogLevel = LogLevel;
    inline constexpr LogLevel CRITICAL = LogLevel::Critical;
    inline constexpr LogLevel ERROR = LogLevel::Error;
    inline constexpr LogLevel NOTIFICATION = LogLevel::Notification;
    inline constexpr LogLevel WARNING = LogLevel::Warning;
    inline constexpr LogLevel DEBUG = LogLevel::Debug;
    inline constexpr LogLevel VERBOSE = LogLevel::Verbose;
    inline constexpr LogLevel LAST_VALUE = LogLevel::LastValue;
  }

}
#endif // STDAIR_STDAIR_LOG_HPP
