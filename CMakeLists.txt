cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project (merry-go-round)

SET(MERRY_GO_ROUND_DEPENDENCY_PATH "${CMAKE_SOURCE_DIR}/deps")
SET(MERRY_GO_ROUND_DEPENDENCIES "taglib" "brigand")
SET(
    MERRY_GO_ROUND_DEPENDENCY_REPOS
    "https://github.com/taglib/taglib"
    "https://github.com/edouarda/brigand"
)
SET(MERRY_GO_ROUND_DEPENDENCY_LIBRARIES "tag" "")
SET(MERRY_GO_ROUND_DEPENDENCY_REVISION
    "6455671ecef7014ee83e91e1fd4e1f9b93dff53a"
    "523ee302474447c808b688607b7353ffd3e5d4bd"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

LIST(LENGTH MERRY_GO_ROUND_DEPENDENCIES MERRY_GO_ROUND_DEPENDENCY_COUNT)

#
# import ExternalProject module
#
include(ExternalProject)

#
# compiler options
#
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(SUPPORT_LARGE_FILES ON)
if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

#
# compilation configure
#
file(GLOB_RECURSE SOURCE_FILES "native/*.cc" "native/*.hpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

#
# Download and build dependencies
#
MATH(EXPR MERRY_GO_ROUND_DEPENDENCY_COUNT "${MERRY_GO_ROUND_DEPENDENCY_COUNT} - 1")
FOREACH(i RANGE 0 ${MERRY_GO_ROUND_DEPENDENCY_COUNT})
    LIST(GET MERRY_GO_ROUND_DEPENDENCIES            ${i} DEPENDENCY_NAME)
    LIST(GET MERRY_GO_ROUND_DEPENDENCY_REPOS        ${i} DEPENDENCY_REPO)
    LIST(GET MERRY_GO_ROUND_DEPENDENCY_LIBRARIES    ${i} DEPENDENCY_LIBRARY)
    LIST(GET MERRY_GO_ROUND_DEPENDENCY_REVISION    ${i} DEPENDENCY_RIVISION)

    MESSAGE(STATUS "Using dependency '${DEPENDENCY_NAME}' from '${DEPENDENCY_REPO}'")

    SET(DEPENDENCY_PATH "${MERRY_GO_ROUND_DEPENDENCY_PATH}/${DEPENDENCY_NAME}")

    #
    # download and make dependency
    #
    ExternalProject_Add(
        ${DEPENDENCY_NAME}
        PREFIX "${DEPENDENCY_PATH}"
        GIT_REPOSITORY ${DEPENDENCY_REPO}
        GIT_TAG ${DEPENDENCY_RIVISION}
        GIT_SHALLOW true
        INSTALL_DIR "${DEPENDENCY_PATH}"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${DEPENDENCY_PATH}"
        CMAKE_CACHE_ARGS "-DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true"
    )

    IF (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        #
        # force rebuild of dependency source (useful when switching between node/electron)
        #
        ExternalProject_Add_Step(
            ${DEPENDENCY_NAME}
            forcebuild
            COMMAND ${CMAKE_COMMAND} -E echo_append ""
            COMMENT "Forcing build step for ${DEPENDENCY_NAME}"
            DEPENDEES configure
            DEPENDERS build
            ALWAYS 1
        )
    ENDIF ()

    #
    # include dependency
    #
    include_directories(BEFORE ${DEPENDENCY_PATH}/include)

    #
    # add library information only if provided
    #
    STRING(LENGTH "${DEPENDENCY_LIBRARY}" DEPENDENCY_LIBRARY_LENGTH)
    IF(NOT "${DEPENDENCY_LIBRARY}" STREQUAL "")
        TARGET_LINK_DIRECTORIES(${PROJECT_NAME} PUBLIC ${DEPENDENCY_PATH}/lib)
        LIST(APPEND MERRY_GO_ROUND_DEPENDENCY_LIBRARY_FILES ${DEPENDENCY_LIBRARY})
    ENDIF()

    #
    # add dependency (after project has been defined)
    #
    add_dependencies(${PROJECT_NAME} ${DEPENDENCY_NAME})
ENDFOREACH()

include_directories(BEFORE ${CMAKE_JS_INC})

#
# Include N-API wrappers
#
execute_process(
    COMMAND node -p "require('node-addon-api').include"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

#
# link TagLib and cmakejs
#
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${MERRY_GO_ROUND_DEPENDENCY_LIBRARY_FILES})