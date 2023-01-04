project(engine_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/1f643f71d4151c3b364c0e9302042f7a6debd439.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_disable_pthreads off)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
enable_testing()

set(TESTS_SRC_FILES test_ConverterJSON.cpp tests_exception.cpp Test_InvertedIndex.cpp Test_SearchServer.cpp)
add_executable(${PROJECT_NAME} ${TESTS_SRC_FILES})

add_library(${PROJECT_NAME}_lib STATIC ../src/ConverterJSON.cpp ../src/InvertedIndex.cpp ../src/SearchServer.cpp ../src/TextIndexingTask.cpp)
#single-thread for tests
target_compile_definitions(${PROJECT_NAME}_lib PRIVATE SINGLE_THREAD)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Concurrent
        Boost::json
        )

target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main ${PROJECT_NAME}_lib)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

if (MINGW)
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
endif()

if (WIN32)
    set(DEBUG_SUFFIX)

if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
endif ()
foreach (QT_LIB Core Widgets Concurrent)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)

endif ()


