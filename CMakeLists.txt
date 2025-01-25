cmake_minimum_required(VERSION 3.16)

project(shwet VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES translations/shwet_en_GB.ts)
set(SRC_DIR src)

set(PROJECT_SOURCES
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/window/window.cpp
        ${SRC_DIR}/window/window.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(shwet
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/resources/res.qrc
        src/resources/style.qss
        src/data-structures/quadtree.h src/data-structures/quadtree.cpp
        src/window/boardlayout.h src/window/boardlayout.cpp
        src/canvas/canvas.h src/canvas/canvas.cpp
        src/components/toolbar.h src/components/toolbar.cpp
        src/tools/tool.h src/tools/tool.cpp
        src/controller/controller.h src/controller/controller.cpp
        src/context/applicationcontext.h src/context/applicationcontext.cpp
        src/event/event.h src/event/event.cpp
        src/tools/drawingtool.h src/tools/drawingtool.cpp
        src/item/item.h src/item/item.cpp
        src/item/polygon.h src/item/polygon.cpp
        src/item/properties/stroke.h src/item/properties/stroke.cpp
        src/item/properties/fill.h src/item/properties/fill.cpp
        src/item/properties/font.h src/item/properties/font.cpp
        src/item/properties/itemproperty.h src/item/properties/itemproperty.cpp
        src/item/rectangle.h src/item/rectangle.cpp
        src/item/factory/itemfactory.h src/item/factory/itemfactory.cpp
        src/item/factory/rectanglefactory.h src/item/factory/rectanglefactory.cpp
        src/tools/polygondrawingtool.h src/tools/polygondrawingtool.cpp
        src/tools/rectangletool.h src/tools/rectangletool.cpp
        src/item/ellipse.h src/item/ellipse.cpp
        src/tools/ellipsetool.h src/tools/ellipsetool.cpp
        src/item/factory/ellipsefactory.h src/item/factory/ellipsefactory.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET shwet APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(shwet SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(shwet
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(shwet PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.shwet)
endif()
set_target_properties(shwet PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS shwet
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(shwet)
endif()
