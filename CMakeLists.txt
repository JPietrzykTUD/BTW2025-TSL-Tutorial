cmake_minimum_required(VERSION 3.16)
project(BTWTutorial)

include(CheckIncludeFile)

set(BIN_ROOT ${PROJECT_SOURCE_DIR}/bin)
set(CXX_FLAGS_DEBUG -O0 -g)
set(CXX_FLAGS_RELEASE -O2)

if (DEFINED GENERATE_TSL)
  if (DEFINED LOCAL_TSL_PATH)
    set(TSL_PATH ${LOCAL_TSL_PATH})
  else()
    set(TSL_PATH ${PROJECT_SOURCE_DIR}/3rdparty/tslgen)
  endif()
  include(${TSL_PATH}/tsl.cmake)
  create_tsl(
    USE_CONCEPTS
    TSLGENERATOR_DIRECTORY ${TSL_PATH}
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tsl
  )
  set(TSL_LINKAGE_REQUIRED TRUE)
else()
  find_file(HAS_TSL_INSTALLED
    NAMES tsl.hpp
    PATHS /usr/include/tsl
    NO_DEFAULT_PATH  # Prevents searching in default locations
  )
  if (NOT HAS_TSL_INSTALLED)
    message(FATAL_ERROR "TSL is not installed in the system")
  endif()
  set(TSL_LINKAGE_REQUIRED FALSE)
endif()


function(create_target)
  set(oneValueArgs TARGET_NAME)
  set(multiValueArgs SRCS INCLUDE_DIRS COMPILE_DEFS COMPILE_OPTS LINK_OPTS LIBS)
  cmake_parse_arguments(CT "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if((NOT DEFINED CT_TARGET_NAME) OR (NOT CT_TARGET_NAME))
    message(FATAL_ERROR "Target name not provided")
  endif()
  add_executable(${CT_TARGET_NAME} ${CT_SRCS})
  set_target_properties(${CT_TARGET_NAME} PROPERTIES 
    CXX_STANDARD 20 
    CXX_STANDARD_REQUIRED ON
  )
  set_target_properties(${CT_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
  )
  set_target_properties(${CT_TARGET_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
  )
  
  target_include_directories(${CT_TARGET_NAME} PRIVATE ${CT_INCLUDE_DIRS})
  target_compile_definitions(${CT_TARGET_NAME} PRIVATE ${CT_COMPILE_DEFS})
  target_compile_options(
    ${CT_TARGET_NAME} PRIVATE 
      ${CT_COMPILE_OPTS} 
      $<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}> 
      $<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>
  )

  target_link_options(${CT_TARGET_NAME} PRIVATE ${CT_LINK_OPTS})
  if(TSL_LINKAGE_REQUIRED)
    target_include_directories(${CT_TARGET_NAME} PRIVATE ${TSL_INCLUDE_DIRECTORY})
    target_link_libraries(${CT_TARGET_NAME} tsl ${CT_LIBS})
  endif()
endfunction()

if(NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type (Debug or Release)" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

create_target(
  TARGET_NAME filter_agg
  SRCS src/filter_aggregate.cpp
  INCLUDE_DIRS src
)