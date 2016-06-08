#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <iostream>


enum FormatType {
    GLOBAL_FORMAT,
    NORMAL_FORMAT,
    VERBOSE_FORMAT
};

enum LogLevel {
    GLOBAL_LOG,
    ERROR_LOG,
    WARNING_LOG,
    INFO_LOG,
    DEBUG_LOG
};


class Logger {
  public:
    Logger(std::string const& name);

    static void setGlobalFormatType(FormatType format_type);
    static void setGlobalLogLevel(LogLevel log_level);

    void setOutStream(std::ostream& out_stream);
    void setFormatType(FormatType format_type);
    void setLogLevel(LogLevel log_level);

    template <typename ...Args> void error(Args... args) const;
    template <typename ...Args> void warning(Args... args) const;
    template <typename ...Args> void info(Args... args) const;
    template <typename ...Args> void debug(Args... args) const;

  private:
    template <typename ...Args>
    void log(LogLevel log_level, Args... args) const;

    void printLogLevel(LogLevel log_level) const;

    static FormatType global_format_type;
    static LogLevel global_log_level;

    std::string name;
    std::ostream *p_out_stream;
    FormatType format_type;
    LogLevel log_level;
};


#define LOGGING_HPP_INCLUDE_USED
#include "logging_template_defs.hpp"
#undef LOGGING_HPP_INCLUDE_USED


#endif // !LOGGING_HPP
