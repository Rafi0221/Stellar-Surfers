cmake_minimum_required(VERSION 3.5)

project(Stellar-Surfers VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Bluetooth Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Bluetooth Core Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h
        mainwindow.ui
        connect/connectwindow.ui
        connect/connectmanager.cpp connect/connectmanager.h
        connect/controllerupdater.h connect/controllerupdater.cpp
        connect/client.cpp connect/client.h
        connect/parser.h connect/parser.cpp
        connect/glupdater.h connect/glupdater.cpp
        opengl/gl.h
        opengl/gl.cpp
        opengl/gl.h
        opengl/gl.cpp
        opengl/openglwindow.h
        opengl/openglwindow.cpp
        play/setupgame.h play/setupgame.cpp
        play/setupwindow.ui
        play/saves/gamesave.h play/saves/gamesave.cpp
        play/saves/datasaver.h play/saves/datasaver.cpp
        play/saves/settingssave.h play/saves/settingssave.cpp
        play/saves/settingssaver.h play/saves/settingssaver.cpp
        play/settings.h play/settings.cpp
        play/settingswindow.ui
        skybox/skybox.h
        skybox/skybox.cpp
        terrain/asteroidcluster.h terrain/asteroidcluster.cpp
        terrain/noisedpatch.h
        terrain/noisedpatch.cpp
        terrain/noisedpatchfactory.h
        terrain/noisedpatchfactory.cpp
        terrain/patch.h
        terrain/patch.cpp
        terrain/patchindices.h
        terrain/patchindices.cpp
        terrain/patchfactory.h
        terrain/patchfactory.cpp
        terrain/planet.h
        terrain/planet.cpp
        terrain/planetlayer.h
        terrain/planetlayer.cpp
        terrain/planetproperties.h
        terrain/quadtreenode.h
        terrain/quadtreenode.cpp
        terrain/terrainface.h
        terrain/terrainface.cpp
        terrain/waterpatch.h
        terrain/waterpatch.cpp
        terrain/waterpatchfactory.h
        terrain/waterpatchfactory.cpp
        game/gamewindow.h
        game/gamewindow.cpp
        game/space.h
        game/space.cpp
        utils/camera.h
        utils/camera.cpp
        utils/colors.h
        utils/colors.cpp
        utils/consts.h
        utils/consts.cpp
        utils/frustum.h
        utils/layeredperlinnoise.h
        utils/layeredperlinnoise.cpp
        utils/model.h
        utils/model.cpp
        utils/noise.h
        utils/obj_loader.h
        utils/obj_loader.cpp
        utils/perlinnoise.h
        utils/perlinnoise.cpp
        utils/shader.h
        utils/shader.cpp
        utils/shadermanager.h
        utils/shadermanager.cpp
        images.qrc
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

target_link_libraries(Stellar-Surfers PRIVATE
    Qt${QT_VERSION_MAJOR}::Bluetooth
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)
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

add_custom_command(
        TARGET Stellar-Surfers POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/media/
                ${CMAKE_BINARY_DIR}/media/)

