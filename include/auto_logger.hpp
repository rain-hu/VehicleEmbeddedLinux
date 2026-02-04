#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include <memory>
#include <string>
#include <unordered_map>


namespace AutoLogger {
enum class LogLevel{
    TRACE = spdlog::level::trace,
    DEBUG = spdlog::level::debug,
    INFO = spdlog::level::info,
    WARN = spdlog::level::warn,
    ERROR = spdlog::level::err,
    CRITICAL = spdlog::level::critical
};

class AutoLogger{
public:
    static AutoLogger & getInstance();
    // init log system
    void init( const std::string& log_dir = "./logs", LogLevel level = LogLevel::INFO, size_t max_file_size = 1048576 * 5, size_t max_files = 3);

    // set can id flag
    void setCanId(uint32_t can_id);

    template<typename... Args>
    void logWithCanId(LogLevel level, uint32_t can_id, const std::string& fmt, Args&&... args) {
        if(!logger_) return;
        auto spdlog_level = static_cast<spdlog::level::level_enum>(level);
        // 判断当前日志级别是否应该被记录
        if(logger_->should_log(spdlog_level)) {
            std::string can_prefix = "[CAN:0x" + toHexString(can_id) + "] ";
            logger_->log(spdlog_level, can_prefix + fmt, std::forward<Args>(args)...);
        }
    }

    template<typename... Args>
    void log(LogLevel level, const std::string& fmt, Args&&... args){
        if(!logger_) return;
        logger_->log(static_cast<spdlog::level::level_enum>(level), fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(const std::string& fmt, Args&&... args){
        log(LogLevel::INFO, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(const std::string& fmt, Args&&... args){
        log(LogLevel::ERROR, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(const std::string& fmt, Args&&... args){
        log(LogLevel::WARN, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(const std::string& fmt, Args&&... args){
        log(LogLevel::DEBUG, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void infoCan(uint32_t can_id, const std::string& fmt, Args&&... args){
        logWithCanId(LogLevel::INFO, can_id, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void errorCan(uint32_t can_id, const std::string& fmt, Args&&... args){
        logWithCanId(LogLevel::ERROR, can_id, fmt, std::forward<Args>(args)...);
    }

    void flush();
    void shutdown();

private:
    AutoLogger() = default;
    ~AutoLogger();
    AutoLogger(const AutoLogger&) = delete;
    AutoLogger& operator=(const AutoLogger&) = delete;
    
    std::string toHexString(uint32_t value);

    std::shared_ptr<spdlog::async_logger> logger_;
    uint32_t current_can_id = 0;
    bool initialized_ = false;
};

// 全局便捷访问宏
#define AUTO_LOG_INFO(...) AutoLogger::AutoLogger::getInstance().info(__VA_ARGS__)
#define AUTO_LOG_ERROR(...) AutoLogger::AutoLogger::getInstance().error(__VA_ARGS__)
#define AUTO_LOG_WARN(...) AutoLogger::AutoLogger::getInstance().warn(__VA_ARGS__)
#define AUTO_LOG_DEBUG(...) AutoLogger::AutoLogger::getInstance().debug(__VA_ARGS__)
#define AUTO_LOG_INFO_CAN(...) AutoLogger::AutoLogger::getInstance().infoCan(__VA_ARGS__)
#define AUTO_LOG_ERROR_CAN(...) AutoLogger::AutoLogger::getInstance().errorCan(__VA_ARGS__)
}

