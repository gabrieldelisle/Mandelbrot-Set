cmake_minimum_required(VERSION 3.6)
project(HW1)



#add_definitions(-DMAP)


#### Parallel target
find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
#file(GLOB_RECURSE SOURCE_FILES "*.c" "*.h")
file(GLOB SOURCE_FILES_PARALLEL  parallel.c)
add_executable(HW1_P ${SOURCE_FILES_PARALLEL})
target_link_libraries(HW1_P ${MPI_C_LIBRARIES})
#set_property(TARGET HW1 PROPERTY C_STANDARD 99)





#### Serial target
file(GLOB SOURCE_FILES  serial.c)
add_executable(HW1 ${SOURCE_FILES})
#set_property(TARGET HW1 PROPERTY C_STANDARD 99)