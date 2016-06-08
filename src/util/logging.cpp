#include "logging.hpp"

#include <string>
#include <iostream>
#include <stdexcept>


FormatType Logger::global_format_type = NORMAL_FORMAT;
LogLevel Logger::global_log_level = WARNING_LOG;


Logger::Logger(std::string const& name)
        : name(name),
          p_out_stream(&std::cerr),
          format_type(GLOBAL_FORMAT),
          log_level(GLOBAL_LOG) {}


void Logger::setGlobalFormatType(FormatType format_type) {
    Logger::global_format_type = format_type;
}

void Logger::setGlobalLogLevel(LogLevel log_level) {
    Logger::global_log_level = log_level;
}


void Logger::setOutStream(std::ostream& out_stream) {
    this->p_out_stream = &out_stream;
}

void Logger::setFormatType(FormatType format_type) {
    this->format_type = format_type;
}

void Logger::setLogLevel(LogLevel log_level) {
    this->log_level = log_level;
}


void Logger::printLogLevel(LogLevel log_level) const {
    switch (log_level) {
        case GLOBAL_LOG:
            throw std::runtime_error("Cannot print for GLOBAL_LOG");
        case ERROR_LOG:
            *(this->p_out_stream) << "ERROR";
            break;
        case WARNING_LOG:
            *(this->p_out_stream) << "WARNING";
            break;
        case INFO_LOG:
            *(this->p_out_stream) << "INFO";
            break;
        case DEBUG_LOG:
            *(this->p_out_stream) << "DEBUG";
            break;
    }
}
