cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(em_cal)

find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACROS "*.mac")
file(GLOB OPTICS_DATA "*.csv")

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${OPTICS_DATA} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(em_cal sim.cc ${sources} ${headers})
target_link_libraries(em_cal ${Geant4_LIBRARIES})
target_link_libraries(em_cal ${ROOT_LIBRARIES})

add_custom_target(em_cal_main DEPENDS em_cal)
