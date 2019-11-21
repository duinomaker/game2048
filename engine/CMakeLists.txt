cmake_minimum_required(VERSION 3.10)

#################################################################################
#       Project Informations
#################################################################################

set(GAME2048_VERSION_MAJOR 0)
set(GAME2048_VERSION_MINOR 1)
set(GAME2048_VERSION ${GAME2048_VERSION_MAJOR}.${GAME2048_VERSION_MINOR})

project(game2048 VERSION ${GAME2048_VERSION})

#################################################################################
#       Detect Operating System
#################################################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(OPERATING_SYSTEM "Windows")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OPERATING_SYSTEM "Linux")
endif()

#################################################################################
#      Set Compiler Flags and Definitions
#################################################################################

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(USE_CACHE)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Od)
        add_compile_options(/EHsc)
        add_compile_options(/MTd)
        add_compile_options(/Wall)
        add_compile_options(/DEBUG)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-g)
        add_compile_options(-W)
        add_compile_options(-Wall)
        add_compile_options(-Wunreachable-code)
    endif()
    set(GAME2048_LIB_NAME "libgame2048_debug")
    set(GAME2048_EXE_NAME "game2048_debug")
else()
    add_definitions(-DRELEASE)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Ox)
        add_compile_options(/EHsc)
        add_compile_options(/MT)
        add_compile_options(/Wall)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-O3)
        add_compile_options(-W)
        add_compile_options(-Wall)
        add_compile_options(-Wunreachable-code)
    endif()
    set(GAME2048_LIB_NAME "libgame2048")
    set(GAME2048_EXE_NAME "game2048")
endif()

#################################################################################
#       Project Directories
#################################################################################

# set(TARGET_LIB "libgame2048")
# set(TARGET_EXE "game2048")
set(TARGET_LIB ${GAME2048_LIB_NAME})
set(TARGET_EXE ${GAME2048_EXE_NAME})

set(GAME2048_INC_DIR                ${PROJECT_SOURCE_DIR}/include)
set(GAME2048_SRC_DIR                ${PROJECT_SOURCE_DIR}/src)
set(GAME2048_TEST_DIR               ${PROJECT_SOURCE_DIR}/test)
set(GAME2048_LIB_DIR                ${PROJECT_BINARY_DIR}/lib)
set(GAME2048_BIN_DIR                ${PROJECT_BINARY_DIR}/bin)
set(GAME2048_TEST_BIN_DIR           ${PROJECT_BINARY_DIR}/bin/test)

set(GAME2048_LIB_INSTALL_DIR        ${PROJECT_SOURCE_DIR}/lib)
set(GAME2048_BIN_INSTALL_DIR        ${PROJECT_SOURCE_DIR}/lib)
set(GAME2048_TEST_INSTALL_DIR       ${PROJECT_SOURCE_DIR}/bin/test)

include_directories(${GAME2048_INC_DIR})

#################################################################################
#       Add Sub-Projects
#################################################################################

add_subdirectory(src)
add_subdirectory(test)