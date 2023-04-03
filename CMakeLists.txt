cmake_minimum_required(VERSION 3.5)

project(Stellar-Surfers VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        connect/connectwindow.h
        connect/connectwindow.cpp
        connect/connectwindow.ui
        opengl/gl.h
        opengl/gl.cpp
        opengl/openglwindow.h
        opengl/openglwindow.cpp
        terrain/noisedterrain.h
        terrain/noisedterrain.cpp
        terrain/planet.h
        terrain/planet.cpp
        terrain/sphericalterrain.h
        terrain/sphericalterrain.cpp
        terrain/terrainface.h
        terrain/terrainface.cpp
        terrain/terrainpatch.h
        terrain/terrainpatch.cpp
        terrain/terraintype.h
        terrain/terraintype.cpp
        terrain/quadtreenode.h
        terrain/quadtreenode.cpp
        test/trianglewindow.h
        test/trianglewindow.cpp
        utils/camera.h
        utils/camera.cpp
        utils/perlinnoise.h
        utils/perlinnoise.cpp
        utils/shader.h
        utils/shader.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Stellar-Surfers
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Stellar-Surfers APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Stellar-Surfers SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Stellar-Surfers
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Stellar-Surfers PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Stellar-Surfers PRIVATE Qt6::OpenGL)

set_target_properties(Stellar-Surfers PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Stellar-Surfers
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Stellar-Surfers)
endif()

add_custom_command(
        TARGET Stellar-Surfers POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/shaders/
                ${CMAKE_BINARY_DIR}/shaders/)
