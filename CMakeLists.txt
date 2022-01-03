cmake_minimum_required(VERSION 3.10)
project (chipmunk)

if (NOT CHIPMUNK_ROOT) 
set(CHIPMUNK_ROOT "ext/chipmunk2d")
endif()

if (NOT CHIPMUNK_SRC_DIR) 
set(CHIPMUNK_SRC_DIR "${CHIPMUNK_ROOT}/src")
endif()

if (NOT CHIPMUNK_INC_DIR) 
set(CHIPMUNK_INC_DIR "${CHIPMUNK_ROOT}/include")
endif()

if (NOT HL_INCLUDE_DIR) 
    set(HL_INCLUDE_DIR "/usr/local/include")
endif()

if (NOT HL_LIB_DIR) 
    set(HL_LIB_DIR "/usr/local/lib")
endif()

if (NOT COMMON_LIB_DIR) 
    set(COMMON_LIB_DIR "/usr/local/lib")
endif()

if (NOT HDLL_DESTINATION) 
    set(HDLL_DESTINATION "/usr/local/lib")
endif()


add_library(chipmunk2d.hdll SHARED
${CHIPMUNK_SRC_DIR}/chipmunk.c
${CHIPMUNK_SRC_DIR}/cpArbiter.c
${CHIPMUNK_SRC_DIR}/cpArray.c
${CHIPMUNK_SRC_DIR}/cpBBTree.c
${CHIPMUNK_SRC_DIR}/cpBody.c
${CHIPMUNK_SRC_DIR}/cpCollision.c
${CHIPMUNK_SRC_DIR}/cpConstraint.c
${CHIPMUNK_SRC_DIR}/cpDampedRotarySpring.c
${CHIPMUNK_SRC_DIR}/cpDampedSpring.c
${CHIPMUNK_SRC_DIR}/cpGearJoint.c
${CHIPMUNK_SRC_DIR}/cpGrooveJoint.c
${CHIPMUNK_SRC_DIR}/cpHashSet.c
${CHIPMUNK_SRC_DIR}/cpHastySpace.c
${CHIPMUNK_SRC_DIR}/cpMarch.c
${CHIPMUNK_SRC_DIR}/cpPinJoint.c
${CHIPMUNK_SRC_DIR}/cpPivotJoint.c
${CHIPMUNK_SRC_DIR}/cpPolyline.c
${CHIPMUNK_SRC_DIR}/cpPolyShape.c
${CHIPMUNK_SRC_DIR}/cpRatchetJoint.c
${CHIPMUNK_SRC_DIR}/cpRobust.c
${CHIPMUNK_SRC_DIR}/cpRotaryLimitJoint.c
${CHIPMUNK_SRC_DIR}/cpShape.c
${CHIPMUNK_SRC_DIR}/cpSimpleMotor.c
${CHIPMUNK_SRC_DIR}/cpSlideJoint.c
${CHIPMUNK_SRC_DIR}/cpSpace.c
${CHIPMUNK_SRC_DIR}/cpSpaceComponent.c
${CHIPMUNK_SRC_DIR}/cpSpaceDebug.c
${CHIPMUNK_SRC_DIR}/cpSpaceHash.c
${CHIPMUNK_SRC_DIR}/cpSpaceQuery.c
${CHIPMUNK_SRC_DIR}/cpSpaceStep.c
${CHIPMUNK_SRC_DIR}/cpSpatialIndex.c
${CHIPMUNK_SRC_DIR}/cpSweep1D.c
src/chipmunk2d.cpp
)

set_target_properties(chipmunk2d.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME chipmunk
SUFFIX .hdll
)

set_target_properties(chipmunk2d.hdll PROPERTIES LINKER_LANGUAGE CXX)

cmake_policy(SET CMP0015 NEW)

target_include_directories(chipmunk2d.hdll
PRIVATE
${HL_INCLUDE_DIR}
${CHIPMUNK_SRC_DIR}
${CHIPMUNK_INC_DIR}
)


if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC  -O3  -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")

    find_library(LIBHL NAMES hl HINTS ${HL_LIB_DIR} )
else()
    find_library(LIBHL NAMES libhl HINTS ${HL_LIB_DIR} )
endif (UNIX)

target_link_libraries(chipmunk2d.hdll ${LIBHL})

install(TARGETS chipmunk2d.hdll DESTINATION ${HDLL_DESTINATION})
