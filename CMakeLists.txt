cmake_minimum_required(VERSION 3.15)


set(PROJECT_NAME perchdb)

project(${PROJECT_NAME} C CXX)


set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)


add_executable(${PROJECT_NAME} main.cpp src/DataEngine.cpp src/Table.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ inc/datatype/)
if (WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE inc/ inc/windows/)
endif (WIN32)
if (UNIX)
    target_include_directories(${PROJECT_NAME} PRIVATE inc/ inc/linux/)
endif (UNIX)
target_link_libraries (${PROJECT_NAME} pthread)

if (DEFINED ${MEM_DEBUG})
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
    target_link_libraries(${PROJECT_NAME} asan)
endif()

set(ALL_TARGETS ${PROJECT_NAME})

