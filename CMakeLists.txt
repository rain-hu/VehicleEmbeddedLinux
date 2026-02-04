cmake_minimum_required(VERSION 3.15)
project(AutoSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 重要：在project()之后立即设置
if(CMAKE_CROSSCOMPILING)
    message(STATUS "交叉编译模式 - 强制使用spdlog内置fmt")

    # 交叉编译：强制使用spdlog内置的fmt
    add_compile_definitions(
        SPDLOG_FMT_EXTERNAL=0
        SPDLOG_COMPILED_LIB=0
        FMT_HEADER_ONLY=1
    )
    
    # 对于嵌入式系统，可能需要禁用Windows相关
    add_compile_definitions(
        FMT_USE_WINDOWS_H=0
    )

    
else()
    message(STATUS "本地编译模式")
    # 本地编译使用外部fmt
    find_package(fmt REQUIRED)
    find_package(spdlog REQUIRED)
    add_definitions(-DSPDLOG_FMT_EXTERNAL=1)
endif()

# 添加可执行文件
add_executable(auto_logger_demo
    src/main.cpp
    src/auto_logger.cpp
    src/vehicle_config.cpp
)

# 包含目录
target_include_directories(auto_logger_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(CMAKE_CROSSCOMPILING)
    # 交叉编译：添加spdlog头文件路径
    target_include_directories(auto_logger_demo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include
    )

    # 关键修改：添加spdlog源文件直接编译
    add_library(spdlog_embedded INTERFACE)
    target_include_directories(spdlog_embedded INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include
    )
    
    # 添加spdlog源码编译
    file(GLOB_RECURSE SPDLOG_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/src/*.cpp
    )
    
    # 过滤掉不需要的文件
    list(FILTER SPDLOG_SOURCES EXCLUDE REGEX ".*/stdout_sinks.*")
    list(FILTER SPDLOG_SOURCES EXCLUDE REGEX ".*/win.*")
    list(FILTER SPDLOG_SOURCES EXCLUDE REGEX ".*/fmt.*")
    
    # 创建静态库
    add_library(spdlog_static STATIC ${SPDLOG_SOURCES})
    target_include_directories(spdlog_static PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include
    )
    target_compile_definitions(spdlog_static PUBLIC
        SPDLOG_FMT_EXTERNAL=0
        SPDLOG_COMPILED_LIB=1  # 关键：启用编译库模式
    )
    
    # 链接spdlog静态库
    target_link_libraries(auto_logger_demo PRIVATE spdlog_static)
    
    # 优化选项
    target_compile_options(auto_logger_demo PRIVATE
        -Os
        -ffunction-sections
        -fdata-sections
        -fexceptions
        -fno-rtti
        -pthread
    )
    
    target_link_options(auto_logger_demo PRIVATE
        -pthread
        -Wl,--gc-sections
    )
    
else()
    # 本地编译：链接库
    target_link_libraries(auto_logger_demo PRIVATE
        spdlog::spdlog
        fmt::fmt
    )
endif()

# 公共链接库
target_link_libraries(auto_logger_demo PRIVATE
    pthread
    m
)

# 交叉编译优化
if(CMAKE_CROSSCOMPILING)
    add_custom_command(TARGET auto_logger_demo POST_BUILD
        COMMAND ${CMAKE_STRIP} "$<TARGET_FILE:auto_logger_demo>"
        COMMENT "去除调试符号"
    )
endif()