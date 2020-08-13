/**
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/PatternFormatter.h"
#include "quill/Fmt.h"

namespace quill
{

/**
 * Base class for handlers
 */
class Handler
{
public:
  /**
   * Constructor
   * Uses the default pattern formatter
   */
  Handler() = default;

  /**
   * Destructor
   */
  virtual ~Handler() = default;

  Handler(Handler const&) = delete;
  Handler& operator=(Handler const&) = delete;

  /**
   * Set a custom formatter for this handler
   * @param format_pattern format pattern as QUILL_STRING(...)
   * @param timestamp_format defaults to "%H:%M:%S.%Qns"
   * @param timezone defaults to PatternFormatter::Timezone::LocalTime
   */
  template <typename TConstantString>
  QUILL_ATTRIBUTE_COLD void set_pattern(TConstantString format_pattern,
                                        std::string timestamp_format = std::string{"%H:%M:%S.%Qns"},
                                        Timezone timezone = Timezone::LocalTime)
  {
    _formatter = std::make_unique<PatternFormatter>(format_pattern, timestamp_format, timezone);
  }

  /**
   * Returns the owned formatter by the handler
   * @note: Accessor for backend processing
   * @return reference to the pattern formatter of this handler
   */
  QUILL_ATTRIBUTE_HOT PatternFormatter const& formatter() { return *_formatter; }

  /**
   * Logs a formatted log record to the handler
   * @note: Accessor for backend processing
   * @param formatted_log_record input log record to write
   * @param log_record_timestamp log record timestamp
   * @param log_message_severity the severity of the log message
   */
  QUILL_ATTRIBUTE_HOT virtual void write(fmt::memory_buffer const& formatted_log_record,
                                         std::chrono::nanoseconds log_record_timestamp,
                                         LogLevel log_message_severity) = 0;

  /**
   * Flush the handler synchronising the associated handler with its controlled output sequence.
   */
  QUILL_ATTRIBUTE_HOT virtual void flush() noexcept = 0;

private:
  /**< Owned formatter for this handler, we have to use a pointer here since the PatterFormatter
   * must not be moved or copied. We create the default pattern formatter always on init */
  std::unique_ptr<PatternFormatter> _formatter{std::make_unique<PatternFormatter>()};
};

} // namespace quill