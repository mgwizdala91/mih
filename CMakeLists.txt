cmake_minimum_required(VERSION 2.6)

OPTION(BUILD_TESTS "Turn off if you dont want to build tests" On)

project (mih)

find_library (HDF5_LIB hdf5)
set (CMAKE_CXX_FLAGS "-DSINGLE_CORE -fopenmp -DNDEBUG")
#set (CMAKE_CXX_FLAGS "-DSINGLE_CORE -O2 -fopenmp -DNDEBUG")
# -Wall -march=native -mtune=native")

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_library(mih src/array32.cpp src/mihasher.cpp src/sparse_hashtable.cpp src/bucket_group.cpp src/reorder.cpp)

add_executable (mih_interface examples/mih_interface.cpp src/reorder.cpp examples/loadVar.cpp examples/saveRes.cpp)
target_link_libraries (mih_interface mih ${HDF5_LIB})

project (linscan)
add_executable (linscan examples/linscan_interface.cpp src/linscan.cpp examples/loadVar.cpp examples/saveRes.cpp)
target_link_libraries (linscan ${HDF5_LIB})


if(BUILD_TESTS)
    # Locate GTest
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    project (tests)
    add_executable(unit_tests test/mih_initialize.cpp test/mih_insert.cpp test/mih_query.cpp)
    target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread mih)
endif()
