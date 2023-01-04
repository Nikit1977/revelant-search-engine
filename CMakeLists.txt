cmake_minimum_required(VERSION 3.24)

project(search_engine VERSION 0.5.1)

#config version file
configure_file(includes/version.h.in version.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#sets for autogen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(${PROJECT_NAME}_lib STATIC src/ConverterJSON.cpp src/InvertedIndex.cpp src/SearchServer.cpp src/TextIndexingTask.cpp)
add_executable(${PROJECT_NAME}_run main.cpp engine.ui)

#include Boost
#BOOST_ROOT set as SYSTEM Environment Variable (e.g. C:\Boost_1_80_0)
find_package(Boost 1.80.0 REQUIRED COMPONENTS json)

#include Qt5
set(CMAKE_PREFIX_PATH "C:/Qt5/5.15.2/mingw81_64/lib/cmake")
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)
#headers path
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/includes)

#linking
target_link_libraries(${PROJECT_NAME}_run PRIVATE ${PROJECT_NAME}_lib)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        Boost::json
        )

if (MINGW)
    target_link_libraries(${PROJECT_NAME}_run PUBLIC ws2_32)
endif()

if (WIN32)
    set(DEBUG_SUFFIX)


    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME}_run POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}_run>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME}_run POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}_run>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets Concurrent)
        add_custom_command(TARGET ${PROJECT_NAME}_run POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}_run>")
    endforeach (QT_LIB)
endif ()

add_subdirectory(Google_tests)
