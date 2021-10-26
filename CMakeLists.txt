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


add_library(chipmunk.hdll SHARED
${CHIPMUNK_SRC_DIR}/chipmunk_c.cpp
${CHIPMUNK_SRC_DIR}/cpArbiter.cpp
${CHIPMUNK_SRC_DIR}/cpArray.cpp
${CHIPMUNK_SRC_DIR}/cpBBTree.cpp
${CHIPMUNK_SRC_DIR}/cpBody.cpp
${CHIPMUNK_SRC_DIR}/cpCollision.cpp
${CHIPMUNK_SRC_DIR}/cpConstraint.cpp
${CHIPMUNK_SRC_DIR}/cpDampedRotarySpring.cpp
${CHIPMUNK_SRC_DIR}/cpDampedSpring.cpp
${CHIPMUNK_SRC_DIR}/cpGearJoint.cpp
${CHIPMUNK_SRC_DIR}/cpGrooveJoint.cpp
${CHIPMUNK_SRC_DIR}/cpHashSet.cpp
${CHIPMUNK_SRC_DIR}/cpHastySpace.cpp
${CHIPMUNK_SRC_DIR}/cpMarch.cpp
${CHIPMUNK_SRC_DIR}/cpPinJoint.cpp
${CHIPMUNK_SRC_DIR}/cpPivotJoint.cpp
${CHIPMUNK_SRC_DIR}/cpPolyline.cpp
${CHIPMUNK_SRC_DIR}/cpPolyShape.cpp
${CHIPMUNK_SRC_DIR}/cpRatchetJoint.cpp
${CHIPMUNK_SRC_DIR}/cpRobust.cpp
${CHIPMUNK_SRC_DIR}/cpRotaryLimitJoint.cpp
${CHIPMUNK_SRC_DIR}/cpShape.cpp
${CHIPMUNK_SRC_DIR}/cpSimpleMotor.cpp
${CHIPMUNK_SRC_DIR}/cpSlideJoint.cpp
${CHIPMUNK_SRC_DIR}/cpSpace.cpp
${CHIPMUNK_SRC_DIR}/cpSpaceComponent.cpp
${CHIPMUNK_SRC_DIR}/cpSpaceDebug.cpp
${CHIPMUNK_SRC_DIR}/cpSpaceHash.cpp
${CHIPMUNK_SRC_DIR}/cpSpaceQuery.cpp
${CHIPMUNK_SRC_DIR}/cpSpaceStep.cpp
${CHIPMUNK_SRC_DIR}/cpSpatialIndex.cpp
${CHIPMUNK_SRC_DIR}/cpSweep1D.cpp
src/chipmunk.cpp
)

set_target_properties(chipmunk.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME chipmunk
SUFFIX .hdll
)

set_target_properties(chipmunk.hdll PROPERTIES LINKER_LANGUAGE CXX)

cmake_policy(SET CMP0015 NEW)

target_include_directories(chipmunk.hdll
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

target_link_libraries(chipmunk.hdll ${LIBHL})

install(TARGETS chipmunk.hdll DESTINATION ${HDLL_DESTINATION})
