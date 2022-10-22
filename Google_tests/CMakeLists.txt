# 'Google_test' is the subproject name
project(Google_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(Google_Tests_run test_ConverterJSON.cpp)

find_package(Boost 1.80.0 REQUIRED COMPONENTS system json)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(Google_Tests_run gtest gtest_main Boost::system
        Boost::json converter)

include(GoogleTest)
gtest_discover_tests(Google_Tests_run)
