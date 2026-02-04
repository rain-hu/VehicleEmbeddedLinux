# ARM交叉编译工具链配置
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING TRUE)

# Buildroot工具链路径
set(BUILDROOT_PATH "$ENV{HOME}/Desktop/linuxsystem/qemu-buildroot/buildroot-2024.02.3/output")
set(CMAKE_SYSROOT "${BUILDROOT_PATH}/staging")

# 重要：查找真实的编译器（不是包装器）
if(EXISTS "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-gcc.br_real")
    # 使用.br_real文件（真实编译器）
    set(CMAKE_C_COMPILER "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-gcc.br_real")
    set(CMAKE_CXX_COMPILER "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-g++.br_real")
else()
    # 回退到包装器
    set(CMAKE_C_COMPILER "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-gcc")
    set(CMAKE_CXX_COMPILER "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-g++")
endif()

# 工具
set(CMAKE_AR "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-ar")
set(CMAKE_RANLIB "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-ranlib")
set(CMAKE_STRIP "${BUILDROOT_PATH}/host/bin/arm-buildroot-linux-gnueabihf-strip")

# 编译标志（针对Cortex-A9）
set(CMAKE_C_FLAGS "-march=armv7-a -marm -mfpu=neon -mfloat-abi=hard" CACHE STRING "C flags")
set(CMAKE_CXX_FLAGS "-march=armv7-a -marm -mfpu=neon -mfloat-abi=hard" CACHE STRING "C++ flags")

# 查找路径设置
set(CMAKE_FIND_ROOT_PATH "${CMAKE_SYSROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 设置环境变量，让包装器正常工作
set(ENV{BUILDROOT_SYSROOT} "${CMAKE_SYSROOT}")
set(ENV{HOST_DIR} "${BUILDROOT_PATH}/host")