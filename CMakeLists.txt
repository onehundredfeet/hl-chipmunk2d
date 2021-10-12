cmake_minimum_required(VERSION 3.10)
project (chipmunk)

add_library(chipmunk.hdll SHARED
${CHIPMUNK_ROOT_DIR}/src/chipmunk.c
${CHIPMUNK_ROOT_DIR}/src/cpArbiter.c
${CHIPMUNK_ROOT_DIR}/src/cpArray.c
${CHIPMUNK_ROOT_DIR}/src/cpBBTree.c
${CHIPMUNK_ROOT_DIR}/src/cpBody.c
${CHIPMUNK_ROOT_DIR}/src/cpCollision.c
${CHIPMUNK_ROOT_DIR}/src/cpConstraint.c
${CHIPMUNK_ROOT_DIR}/src/cpDampedRotarySpring.c
${CHIPMUNK_ROOT_DIR}/src/cpDampedSpring.c
${CHIPMUNK_ROOT_DIR}/src/cpGearJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpGrooveJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpHashSet.c
${CHIPMUNK_ROOT_DIR}/src/cpHastySpace.c
${CHIPMUNK_ROOT_DIR}/src/cpMarch.c
${CHIPMUNK_ROOT_DIR}/src/cpPinJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpPivotJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpPolyline.c
${CHIPMUNK_ROOT_DIR}/src/cpPolyShape.c
${CHIPMUNK_ROOT_DIR}/src/cpRatchetJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpRobust.c
${CHIPMUNK_ROOT_DIR}/src/cpRotaryLimitJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpShape.c
${CHIPMUNK_ROOT_DIR}/src/cpSimpleMotor.c
${CHIPMUNK_ROOT_DIR}/src/cpSlideJoint.c
${CHIPMUNK_ROOT_DIR}/src/cpSpace.c
${CHIPMUNK_ROOT_DIR}/src/cpSpaceComponent.c
${CHIPMUNK_ROOT_DIR}/src/cpSpaceDebug.c
${CHIPMUNK_ROOT_DIR}/src/cpSpaceHash.c
${CHIPMUNK_ROOT_DIR}/src/cpSpaceQuery.c
${CHIPMUNK_ROOT_DIR}/src/cpSpaceStep.c
${CHIPMUNK_ROOT_DIR}/src/cpSpatialIndex.c
${CHIPMUNK_ROOT_DIR}/src/cpSweep1D.c
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
${CHIPMUNK_ROOT_DIR}/src
${CHIPMUNK_ROOT_DIR}/include
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
