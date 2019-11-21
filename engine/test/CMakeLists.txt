# set header directories
include_directories(${GAME2048_INC_DIR})
include_directories(${GAME2048_TEST_DIR})
link_directories(${GAME2048_LIB_DIR})

file(GLOB_RECURSE GAME2048_TEST_SOURCES FOLLOW_SYMLINKS *.cpp)
foreach (srcPath ${GAME2048_TEST_SOURCES})
        # set the test source file name
        string(REGEX MATCH "[A-Za-z0-9_-]+\\.cpp$" srcFilename "${srcPath}")

        # set the test source file relative directory
        string(REPLACE "${srcFilename}" "" srcDir "${srcPath}")
        string(REPLACE "${GAME2048_TEST_DIR}/" "" srcDir "${srcDir}")

        # set the test source file target name
        string(REGEX MATCH "^[A-Za-z0-9_-]+" targetName "${srcFilename}")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
           set(targetName "${targetName}_debug")
        endif()

        # add the test executable
        add_executable(${targetName} ${srcPath})
        target_compile_features(${targetName} PUBLIC cxx_std_11)

        # add the linked libraries
        target_link_libraries(${targetName} ${TARGET_LIB})

        # if (${OPERATING_SYSTEM} STREQUAL "Linux")
        #     target_link_libraries(${targetName} ${TARGET_LIB} rt pthread)
        # else()
        #     target_link_libraries(${targetName} ${TARGET_LIB})
        # endif()

        set_target_properties(
            ${targetName} PROPERTIES
            OUTPUT_NAME ${targetName}
            RUNTIME_OUTPUT_DIRECTORY "${GAME2048_TEST_BIN_DIR}/${srcDir}"
        )

        # install the test executable
        install(
            TARGETS ${targetName}
            DESTINATION "${GAME2048_TEST_INSTALL_DIR}/${srcDir}"
        )

endforeach(srcPath)