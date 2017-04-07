#include "log.h"

#include <iostream>

// Logger level -> string
static const char * const levels[] = {
    "NO_PRINT",
    "CRITICAL",
    "ERROR",
    "WARNING",
    "DEBUG",
    "INFO"
};

// By default, we'll try to help
bool Logger::no_newlines_ = false;

Logger::Logger(Logger::Level level, std::string name,
               bool no_newlines, std::ostream& sink)
    : level_(level), sink_(sink), name_(name)
{
    if (!name.empty()) {
        this->buffer << "[" << name << "]";
    }

    // No leading newline until after the first set of output
    this->need_newline_ = false;

    this->no_newlines_ = no_newlines;

    // Start in an inactive state
    this->active_ = false;
}

Logger::Logger(const Logger &src, const std::string &name)
    : sink_(src.sink_)
{
    this->need_newline_ = src.need_newline_;
    this->no_newlines_ = src.no_newlines_;
    this->level_ = src.level_;
    // this->buffer is default constructed
    if (name.empty()) {
        this->name_ = src.name_;
    } else {
        this->name_ = name;
    }
}

Logger::~Logger()
{
    // Trailing newline for nice formatting
    if (!this->no_newlines_) {
        this->sink_ << std::endl;
    }
}

Logger& Logger::log(const std::string &msg, const char *function,
                    const char *file, const int line, const Logger::Level l)
{
    if (this->level_ < l) {
        this->active_ = false;
        return *this;
    } else {
        this->active_ = true;
    }

    if (!this->no_newlines_ and this->need_newline_) {
        this->sink_ << std::endl;
    } else {
        this->need_newline_ = true;
    }

    if (!this->name_.empty()) {
        this->sink_ << "[" << levels[l] << '/' << this->name_ << "]";
    }

    this->sink_ << "[" << file << "]: " << function << ":" << line << " "
                 << msg;
    return *this;
}

Logger& Logger::log(const char *msg, const char *function,
                    const char *file, const int line, const Logger::Level l)
{
    if (this->level_ < l) {
        this->active_ = false;
        return *this;
    } else {
        this->active_ = true;
    }

    if (!this->no_newlines_ and this->need_newline_) {
        this->sink_ << std::endl;
    } else {
        this->need_newline_ = true;
    }

    if (!this->name_.empty()) {
        this->sink_ << "[" << levels[l] << '/' << this->name_ << "]";
    }

    this->sink_ << "[" << file << "]:" << function << ":" << line << ": "
                 << msg;
    return *this;
}

Logger &Logger::rename(const std::string &name)
{
    this->name_ = name;
    return *this;
}

const std::string& Logger::name() const
{
    return this->name_;
}

Logger::Level Logger::level() const
{
    return this->level_;
}

