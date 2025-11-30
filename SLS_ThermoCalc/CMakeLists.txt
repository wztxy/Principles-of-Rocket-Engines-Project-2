cmake_minimum_required(VERSION 3.16)

project(SLS_ThermoCalc VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# UI 文件搜索路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# Qt 组件
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 源文件
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/thermo_core.c
    src/propellant_db.c
    src/matrix_solver.c
)

# 头文件
set(HEADERS
    include/mainwindow.h
    include/thermo_core.h
    include/propellant_db.h
    include/matrix_solver.h
    include/constants.h
)

# UI 文件
set(UI_FILES
    ui/mainwindow.ui
)

# 资源文件
set(RESOURCES
    resources/resources.qrc
)

# 平台特定设置
if(WIN32)
    # Windows: 创建 GUI 应用 (不显示控制台)
    set(CMAKE_WIN32_EXECUTABLE ON)
    
    # 应用图标 (可选)
    # set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
    # list(APPEND SOURCES ${APP_ICON_RESOURCE_WINDOWS})
endif()

if(APPLE)
    # macOS: 创建 App Bundle
    set(CMAKE_MACOSX_BUNDLE ON)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.sls.thermocalc")
    
    # 应用图标 (可选)
    # set(MACOSX_BUNDLE_ICON_FILE AppIcon.icns)
    # set(APP_ICON_MACOS "${CMAKE_CURRENT_SOURCE_DIR}/resources/AppIcon.icns")
    # set_source_files_properties(${APP_ICON_MACOS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    # list(APPEND SOURCES ${APP_ICON_MACOS})
endif()

# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCES}
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
)

# 输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# 安装规则
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# 复制预设文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/presets
    DESTINATION share/${PROJECT_NAME}
)
