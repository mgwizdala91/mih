cmake_minimum_required(VERSION 2.6)

OPTION(BUILD_DEBUG "Should compiler build debug version of applications?" On)
OPTION(USE_SSE "Should compiler use SSE instricts" Off)
OPTION(BUILD_TESTS "Turn off if you dont want to build tests" On)
OPTION(BUILD_EXAMPLES "Turn off if you dont want to build examples" On)

project(mih)

include_directories ("${PROJECT_SOURCE_DIR}/include")

find_library (HDF5_LIB hdf5)

if(BUILD_DEBUG)
    set (CMAKE_CXX_FLAGS "-DSINGLE_CORE -pg -DNDEBUG -std=c++11" )
else()
    set (CMAKE_CXX_FLAGS "-DSINGLE_CORE -O2 -march=native -std=c++11")
endif()
# -Wall -march=native -mtune=native")

if(USE_SSE)
    add_definitions(-DUSE_SSE)
endif()

include_directories(${GTEST_INCLUDE_DIRS})
add_library(mih src/array32.cpp src/mihasher.cpp src/sparse_hashtable.cpp src/bucket_group_stl.cpp src/reorder.cpp)


if(BUILD_TESTS)
    project(mih_examples)


    add_executable (mih_interface examples/mih_interface.cpp src/reorder.cpp examples/loadVar.cpp examples/saveRes.cpp)
    target_link_libraries (mih_interface mih ${HDF5_LIB})

    add_executable (linscan examples/linscan_interface.cpp src/linscan.cpp examples/loadVar.cpp examples/saveRes.cpp)
    target_link_libraries (linscan ${HDF5_LIB})

    add_executable (benchmark examples/benchmark.cpp include/bucket_group_stl.h)
    target_link_libraries (benchmark mih)
endif()

if(BUILD_TESTS)
    # Locate GTest
    find_package(GTest REQUIRED)

    project (mih_tests)
    add_executable(unit_tests test/mih_initialize.cpp test/mih_insert.cpp test/mih_query.cpp)
    target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread mih)
endif()
