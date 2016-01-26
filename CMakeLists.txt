cmake_minimum_required(VERSION 2.6)

OPTION(BUILD_TESTS "Turn off if you dont want to build tests" On)

project (mih)


find_library (HDF5_LIB hdf5)
set (CMAKE_CXX_FLAGS "-DSINGLE_CORE -fopenmp -DNDEBUG")
#set (CMAKE_CXX_FLAGS "-DSINGLE_CORE -O2 -fopenmp -DNDEBUG")
# -Wall -march=native -mtune=native")

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_executable (mih src/array32.cpp src/mihasher.cpp src/sparse_hashtable.cpp src/bucket_group.cpp interface/mih_interface.cpp src/reorder.cpp interface/loadVar.cpp interface/saveRes.cpp)
target_link_libraries (mih ${HDF5_LIB})

project (linscan)
add_executable (linscan interface/linscan_interface.cpp src/linscan.cpp interface/loadVar.cpp interface/saveRes.cpp)
target_link_libraries (linscan ${HDF5_LIB})


if(BUILD_TESTS)
    # Locate GTest
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    project (tests)
    add_executable(initializeTest test/mih_initialize.cpp src/array32.cpp src/mihasher.cpp src/sparse_hashtable.cpp src/bucket_group.cpp src/reorder.cpp test/mih_insert.cpp test/mih_query.cpp)
    target_link_libraries(initializeTest ${GTEST_LIBRARIES} pthread)
endif()
