cmake_minimum_required(VERSION 3.23)
project(search_engine VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(Boost_INCLUDE_DIR "C:\\MY_programming\\boost_1_79_0")
set(Boost_LIBRARY_DIR "C:\\MY_programming\\boost_1_79_0\\stage\\lib\\cmake")

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRIARY_DIR})

find_package(Boost 1.79 REQUIRED)

set(CMAKE_PREFIX_PATH "C:/Qt5/5.15.2/mingw81_64/lib/cmake")
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(includes)
add_executable(search_engine main.cpp)
add_library(classes STATIC src/ConverterJSON.cpp src/InvertedIndex.cpp)

target_link_libraries(search_engine
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        classes
        )

add_subdirectory(Google_tests)

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
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()


