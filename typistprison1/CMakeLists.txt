cmake_minimum_required(VERSION 3.5)

project(typistprison VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

set(TS_FILES typistprison_zh_CN.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    fictiontextedit.cpp
    fictiontextedit.h
    fictionviewtab.cpp
    fictionviewtab.h
    plaintextedit.cpp
    plaintextedit.h
    plaintextviewtab.cpp
    plaintextviewtab.h
    markdownviewtab.cpp
    markdownviewtab.h
    customtabwidget.cpp
    customtabwidget.h
    searchWidget.cpp
    searchWidget.h
    searchHighlighter.cpp
    searchHighlighter.h
    textedithelper.cpp
    textedithelper.h
    markdownhighlighter.cpp
    markdownhighlighter.h
    qmarkdowntextedit.cpp
    qmarkdowntextedit.h
    qownlanguagedata.cpp
    qownlanguagedata.h
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(typistprison
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(typistprison SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(typistprison
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Add qmarkdowntextedit
# add_subdirectory(3rdparty/qmarkdowntextedit)
# include_directories(3rdparty/qmarkdowntextedit)

# Link QMarkdownTextEdit library
target_link_libraries(typistprison PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Set properties for the executable
set_target_properties(typistprison PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Install targets
include(GNUInstallDirs)
install(TARGETS typistprison
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_finalize_executable(typistprison)
endif()
