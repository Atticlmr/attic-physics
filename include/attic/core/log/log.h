#pragma once
#include <cstdarg>
#include <cstdio>
#include <ctime>

namespace attic {
namespace log {

enum Level { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3 };

class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void set_level(Level level) { level_ = level; }
    Level get_level() const { return level_; }
    void set_output(FILE* file) { output_ = file; }
    FILE* get_output() const { return output_; }

    void log(Level level, const char* file, int line, const char* fmt, ...) {
        if (level < level_) return;
        const char* level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};
        char time_buf[32];
        time_t now = time(nullptr);
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        FILE* out = output_;
        fprintf(out, "[%s] [%s] ", time_buf, level_str[level]);
        if (level == DEBUG) {
            const char* filename = file;
            for (const char* p = file; *p; ++p)
                if (*p == '/' || *p == '\\') filename = p + 1;
            fprintf(out, "[%s:%d] ", filename, line);
        }
        va_list args;
        va_start(args, fmt);
        vfprintf(out, fmt, args);
        va_end(args);
        fprintf(out, "\n");
        fflush(out);
    }

private:
    Logger() : level_(INFO), output_(stderr) {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Level level_;
    FILE* output_;
};

inline void set_level(Level level) { Logger::instance().set_level(level); }
inline Level get_level() { return Logger::instance().get_level(); }
inline void set_output(FILE* file) { Logger::instance().set_output(file); }

inline void log_message(Level level, const char* file, int line, const char* fmt, ...) {
    if (level < Logger::instance().get_level()) return;
    const char* level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    char time_buf[32];
    time_t now = time(nullptr);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    FILE* out = Logger::instance().get_output();
    fprintf(out, "[%s] [%s] ", time_buf, level_str[level]);
    if (level == DEBUG) {
        const char* filename = file;
        for (const char* p = file; *p; ++p)
            if (*p == '/' || *p == '\\') filename = p + 1;
        fprintf(out, "[%s:%d] ", filename, line);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);
    fprintf(out, "\n");
    fflush(out);
}

} // namespace log
} // namespace attic

#define ATTIC_LOG_DEBUG(fmt, ...) attic::log::log_message(attic::log::DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define ATTIC_LOG_INFO(fmt, ...) attic::log::log_message(attic::log::INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define ATTIC_LOG_WARN(fmt, ...) attic::log::log_message(attic::log::WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define ATTIC_LOG_ERROR(fmt, ...) attic::log::log_message(attic::log::ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ATTIC_DEBUG ATTIC_LOG_DEBUG
#define ATTIC_INFO ATTIC_LOG_INFO
#define ATTIC_WARN ATTIC_LOG_WARN
#define ATTIC_ERROR ATTIC_LOG_ERROR
