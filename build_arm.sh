#!/bin/bash
set -e

echo "=== 为嵌入式ARM系统交叉编译 ==="

# 项目目录
PROJECT_DIR=$(cd "$(dirname "$0")" && pwd)
echo "项目目录: $PROJECT_DIR"

# Buildroot工具链
BUILDROOT_DIR="$HOME/Desktop/linuxsystem/qemu-buildroot/buildroot-2024.02.3"
TOOLCHAIN_FILE="$PROJECT_DIR/toolchain-arm.cmake" 

# 检查spdlog
if [ ! -d "$PROJECT_DIR/third_party/spdlog/include" ]; then
    echo "警告: spdlog头文件未找到，尝试下载..."
    mkdir -p "$PROJECT_DIR/third_party"
    cd "$PROJECT_DIR/third_party"
    wget -q https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.tar.gz
    tar -xzf v1.11.0.tar.gz
    mv spdlog-1.11.0 spdlog
    rm v1.11.0.tar.gz
    echo "spdlog已下载到 third_party/spdlog"
fi

# 创建构建目录
BUILD_DIR="$PROJECT_DIR/build-arm"
echo "构建目录: $BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# CMake配置
echo "配置CMake..."
cmake "$PROJECT_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# 编译
echo "编译..."
make -j$(nproc)

echo -e "\n✅ 编译完成！"
echo "可执行文件: $BUILD_DIR/auto_logger_demo"

# 验证
echo -e "\n=== 验证 ==="
echo "1. 文件类型:"
file auto_logger_demo

echo -e "\n2. 大小信息:"
size auto_logger_demo

echo -e "\n3. 库依赖:"
"$BUILDROOT_DIR/output/host/bin/arm-buildroot-linux-gnueabihf-readelf" -d auto_logger_demo | grep NEEDED

echo -e "\n4. 检查pthread链接:"
"$BUILDROOT_DIR/output/host/bin/arm-buildroot-linux-gnueabihf-strings" auto_logger_demo | grep -i "pthread" | head -3

echo -e "\n=== 下一步 ==="
echo "部署到QEMU:"
echo "sudo mount -o loop $BUILDROOT_DIR/output/images/rootfs.ext2 /mnt"
echo "sudo cp $BUILD_DIR/auto_logger_demo /mnt/usr/bin/"
echo "sudo chmod 755 /mnt/usr/bin/auto_logger_demo"
echo "sudo umount /mnt"
