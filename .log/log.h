#ifndef LOGGER_946336
#define LOGGER_946336

#include <iostream>
#include <sstream> // std::ostringstream

// NEEDSWORK -
// Does not support iomanip functions, etc. There is currently no way to
// manually flush the stream.

class Logger
{
public:
    enum Level {SILENT = 0, CRITICAL, ERROR, WARNING, DEBUG, INFO};

    // Name gives additional information. If not given or set to the empty
    // string, it is not included in the final output.
    // sink is where output is sent.
    Logger(Level l, std::string name = "", bool no_newlines = false,
           std::ostream& sink = std::cerr);
    // Buffer contents are not copied
    Logger(const Logger &src, const std::string &name = "");
    ~Logger();

    // We might play with defaults here so that you don't _have_ to give up
    // implementation details or display all that extra metadata
    Logger &log(const std::string &msg, const char *function,
                const char *file, const int line, const Level l);
    Logger &log(const char *msg, const char *function,
                const char *file, const int line, const Level l);

    const std::string& name() const;
    Logger &rename(const std::string &name);

    Level level() const;

    template<typename T>
    friend Logger& operator<< (Logger& logger, const T &to_log);

protected:
    Level level_;
    std::ostream& sink_;
    std::ostringstream buffer;

    std::string name_;

    // Controls whether or not we output a leading newline
    bool need_newline_;

    // Disables all attempts to insert sane newlines during logging.
    // Client must manually format.
    // Defaults to false (Module will insert newlines)
    static bool no_newlines_;

    // If we attempted to start a line of logging that wouldn't be displayed
    // under our logging level, eat all logging input until a line of
    // sufficiently high important is started.
    bool active_;
};

// Use these to start new log entries.
#define INFO(logger, msg) logger.log(msg, __FUNCTION__, __FILE__, __LINE__,\
        Logger::Level::INFO)
#define DEBUG(logger, msg) logger.log(msg, __FUNCTION__, __FILE__, __LINE__,\
        Logger::Level::DEBUG)
#define WARNING(logger, msg) logger.log(msg, __FUNCTION__, __FILE__, __LINE__,\
        Logger::Level::WARNING)
#define ERROR(logger, msg) logger.log(msg, __FUNCTION__, __FILE__, __LINE__,\
        Logger::Level::ERROR)
#define CRITICAL(logger, msg) logger.log(msg, __FUNCTION__, __FILE__,\
        __LINE__, Logger::Level::CRITICAL)

// Consider overloading for Logger::done instead of specializing the template
template<typename T>
Logger& operator<< (Logger& logger, const T &to_log)
{
    if (logger.active_) {
        logger.sink_ << to_log;
    }
    return logger;
}


#endif

