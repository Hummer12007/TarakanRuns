#if !defined(LOGGING_HPP) || !defined(LOGGING_HPP_INCLUDE_USED)
#  error "Only logging.hpp can include this file"
#endif


#ifndef LOGGING_TEMPLATES_HPP
#define LOGGING_TEMPLATES_HPP

#include <utility>

#include <ctime>


template <typename ...Args>
void Logger::error(Args... args) const {
    this->log(ERROR_LOG, args...);
}

template <typename ...Args>
void Logger::warning(Args... args) const {
    this->log(WARNING_LOG, args...);
}

template <typename ...Args>
void Logger::info(Args... args) const {
    this->log(INFO_LOG, args...);
}

template <typename ...Args>
void Logger::debug(Args... args) const {
    this->log(DEBUG_LOG, args...);
}


template <typename ...Args>
void Logger::log(LogLevel log_level, Args... args) const {
    FormatType used_format_type = (this->format_type == GLOBAL_FORMAT)
                                ? Logger::global_format_type
                                : this->format_type;
    LogLevel used_log_level = (this->log_level == GLOBAL_LOG)
                            ? Logger::global_log_level
                            : this->log_level;

    if (used_log_level < log_level) {
        return;
    }

    if (used_format_type >= VERBOSE_FORMAT) {
        time_t timestamp = std::time(NULL);
        struct tm *timeinfo = localtime(&timestamp);
        char str_time[9];

        strftime(str_time, 9, "%H:%M:%S", timeinfo);
        *(this->p_out_stream) << "[" << str_time << " ";
        this->printLogLevel(log_level);
        *(this->p_out_stream) << " " << this->name << "] ";
    }

    using expander = int[];
    (void) expander{ (*(this->p_out_stream) << std::forward<Args>(args),
                     void(), 0)... };

    *(this->p_out_stream) << std::endl;
}


#endif // !LOGGING_TEMPLATES_HPP
