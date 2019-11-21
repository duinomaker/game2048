# set header directories
include_directories(${GAME2048_INC_DIR})

#################################################################################
#       Build the library
#################################################################################

# set the source file
file(GLOB_RECURSE LIB_SOURCES FOLLOW_SYMLINKS *.cpp)
list(REMOVE_ITEM LIB_SOURCES ${GAME2048_SRC_DIR}/game2048.cpp)

# build a static library
add_library(${TARGET_LIB} STATIC ${LIB_SOURCES})

target_compile_features(${TARGET_LIB} PUBLIC cxx_std_11)

# add the linked libraries
if (${OPERATING_SYSTEM} STREQUAL "Linux")
    target_link_libraries(${TARGET_LIB} rt pthread)
endif()

set_target_properties(
    ${TARGET_LIB} PROPERTIES
    VERSION ${GAME2048_VERSION}
    SOVERSION ${GAME2048_VERSION_MAJOR}
    LIBRARY_OUTPUT_DIRECTORY ${GAME2048_LIB_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${GAME2048_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${GAME2048_LIB_DIR}
)

#################################################################################
#       Install the library
#################################################################################

install(
    TARGETS ${TARGET_LIB}
    DESTINATION ${GAME2048_LIB_INSTALL_DIR}
)

add_executable(${TARGET_EXE} game2048.cpp)
target_compile_features(${TARGET_EXE} PUBLIC cxx_std_11)
target_link_libraries(${TARGET_EXE} ${TARGET_LIB})

set_target_properties(
    ${TARGET_EXE} PROPERTIES
    OUTPUT_NAME ${GAME2048_EXE_NAME}
    RUNTIME_OUTPUT_DIRECTORY ${GAME2048_BIN_DIR}
)

install(
    TARGETS ${TARGET_EXE}
    DESTINATION ${GAME2048_BIN_INSTALL_DIR}
)