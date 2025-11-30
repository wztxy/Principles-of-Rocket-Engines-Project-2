#!/bin/bash
# ==============================================
# SLS ThermoCalc 构建脚本
# 支持 macOS 和 Windows 交叉编译
# ==============================================

set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
BIN_DIR="$PROJECT_DIR/bin"

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 显示帮助
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  macos       构建 macOS 版本 (.app)"
    echo "  windows     构建 Windows 版本 (.exe) - 需要交叉编译工具链"
    echo "  all         构建所有平台版本"
    echo "  clean       清理构建目录"
    echo "  qmake       使用 qmake 构建 (仅当前平台)"
    echo "  help        显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 macos     # 构建 macOS 版本"
    echo "  $0 windows   # 交叉编译 Windows 版本"
    echo "  $0 all       # 构建所有版本"
}

# 清理
clean_build() {
    print_info "清理构建目录..."
    rm -rf "$BUILD_DIR"
    rm -rf "$BIN_DIR"/*.exe
    rm -rf "$BIN_DIR"/*.app
    print_info "清理完成"
}

# 使用 qmake 构建 (当前平台)
build_qmake() {
    print_info "使用 qmake 构建..."
    mkdir -p "$BUILD_DIR/qmake"
    cd "$BUILD_DIR/qmake"
    qmake "$PROJECT_DIR/SLS_ThermoCalc.pro"
    make -j$(sysctl -n hw.ncpu)
    print_info "qmake 构建完成"
}

# 构建 macOS 版本
build_macos() {
    print_info "构建 macOS 版本..."
    
    mkdir -p "$BUILD_DIR/macos"
    cd "$BUILD_DIR/macos"
    
    cmake "$PROJECT_DIR" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
    
    cmake --build . --config Release -j$(sysctl -n hw.ncpu)
    
    print_info "macOS 构建完成: $BIN_DIR/SLS_ThermoCalc.app"
    
    # 可选: 创建 DMG
    # print_info "创建 DMG 安装包..."
    # macdeployqt "$BIN_DIR/SLS_ThermoCalc.app" -dmg
}

# 构建 Windows 版本 (交叉编译)
build_windows() {
    print_info "构建 Windows 版本 (交叉编译)..."
    
    # 检查 MXE 或 mingw-w64
    if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
        MINGW_PREFIX="x86_64-w64-mingw32"
    elif [ -d "/opt/mxe" ]; then
        export PATH="/opt/mxe/usr/bin:$PATH"
        MINGW_PREFIX="x86_64-w64-mingw32.static"
    elif command -v brew &> /dev/null && brew list mingw-w64 &> /dev/null; then
        MINGW_PREFIX="x86_64-w64-mingw32"
    else
        print_error "未找到 Windows 交叉编译工具链!"
        echo ""
        echo "请安装 mingw-w64:"
        echo "  macOS:   brew install mingw-w64"
        echo "  Ubuntu:  sudo apt install mingw-w64"
        echo ""
        echo "或安装 MXE (推荐，包含 Qt):"
        echo "  https://mxe.cc/"
        echo ""
        print_warn "跳过 Windows 构建"
        return 1
    fi
    
    # 检查是否有 Qt for Windows
    QT_WINDOWS_DIR=""
    
    # 尝试找 MXE 中的 Qt
    if [ -d "/opt/mxe/usr/x86_64-w64-mingw32.static/qt6" ]; then
        QT_WINDOWS_DIR="/opt/mxe/usr/x86_64-w64-mingw32.static/qt6"
    elif [ -d "$HOME/Qt/6.*/mingw_64" ]; then
        QT_WINDOWS_DIR=$(ls -d $HOME/Qt/6.*/mingw_64 2>/dev/null | head -1)
    fi
    
    if [ -z "$QT_WINDOWS_DIR" ]; then
        print_error "未找到 Windows 版本的 Qt!"
        echo ""
        echo "Windows 交叉编译需要:"
        echo "1. 安装 MXE 并编译 Qt6:"
        echo "   cd /opt/mxe && make qt6"
        echo ""
        echo "2. 或在 Windows 上直接编译"
        echo ""
        print_warn "跳过 Windows 构建"
        return 1
    fi
    
    mkdir -p "$BUILD_DIR/windows"
    cd "$BUILD_DIR/windows"
    
    cmake "$PROJECT_DIR" \
        -DCMAKE_SYSTEM_NAME=Windows \
        -DCMAKE_C_COMPILER=${MINGW_PREFIX}-gcc \
        -DCMAKE_CXX_COMPILER=${MINGW_PREFIX}-g++ \
        -DCMAKE_RC_COMPILER=${MINGW_PREFIX}-windres \
        -DCMAKE_PREFIX_PATH="$QT_WINDOWS_DIR" \
        -DCMAKE_BUILD_TYPE=Release
    
    cmake --build . --config Release -j$(sysctl -n hw.ncpu)
    
    print_info "Windows 构建完成: $BIN_DIR/SLS_ThermoCalc.exe"
}

# 主程序
case "${1:-help}" in
    macos)
        build_macos
        ;;
    windows)
        build_windows
        ;;
    all)
        build_macos
        build_windows || true
        ;;
    clean)
        clean_build
        ;;
    qmake)
        build_qmake
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        print_error "未知选项: $1"
        show_help
        exit 1
        ;;
esac
