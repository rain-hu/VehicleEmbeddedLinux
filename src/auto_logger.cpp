#include "auto_logger.hpp"
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <iomanip>
#include <sstream>

namespace AutoLogger {

AutoLogger::~AutoLogger() {
    shutdown();
}

AutoLogger& AutoLogger::getInstance(){
   static AutoLogger instance;
   return instance;
};

void AutoLogger::init(const std::string& log_dir, LogLevel level, size_t max_file_size, size_t max_files){
    if (initialized_) return;

    // create sync thread pooling
    spdlog::init_thread_pool(8192, 1);

    // 创建sinks
    std::vector<spdlog::sink_ptr> sinks;

    // control input
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] %v");
    sinks.push_back(console_sink);


    std::string log_file = log_dir + "/auto_system.log";
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file, max_file_size, max_files);
    file_sink->set_level(spdlog::level::trace);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thread %t] %v] ");
    sinks.push_back(file_sink);

    // create async logger
    logger_ = std::make_shared<spdlog::async_logger>("auto_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    

};

void AutoLogger::setCanId(uint32_t can_id) {
    current_can_id = can_id;
};

std::string AutoLogger::toHexString(uint32_t value) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << value;
    return ss.str();
};

void AutoLogger::flush() {
    if (logger_) {
        logger_->flush();
    }
};

void AutoLogger::shutdown() {
    if (initialized_) {
        flush();
        spdlog::drop_all();
        spdlog::shutdown();
        initialized_ = false;
    }
};

};



