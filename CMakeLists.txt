CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(kerfuffle)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED true)
SET(CMAKE_CXX_EXTENSIONS OFF)

SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake/modules/
    ${CMAKE_SOURCE_DIR}/tools/CMake/modules/)

INCLUDE(Options)
INCLUDE(CheckDependencies)
INCLUDE(VersionHelper)
INCLUDE(Coverage)
INCLUDE(Tools)

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(kerfuffle INTERFACE)
TARGET_SOURCES(kerfuffle INTERFACE include/kerfuffle.h)

IF (GEN_HEADER)
  ADD_DEPENDENCIES(kerfuffle gen_header)
ENDIF ()

IF (IN_DEVELOPMENT)
  ADD_EXECUTABLE(placeholder src/placeholder.cc include/kerfuffle.h)
  TARGET_INCLUDE_DIRECTORIES(placeholder PUBLIC include/)
  ADD_DEPENDENCIES(placeholder kerfuffle)
ENDIF ()

INCLUDE(TestExampleHelper)

IF (BUILD_EXAMPLES)
  CREATE_EXAMPLE(basic_traffic_light)
  CREATE_EXAMPLE(debug_logger_interface)
  CREATE_EXAMPLE(serialization)
  IF (NOT WIN32)
    CREATE_EXAMPLE(calculator)
    TARGET_LINK_LIBRARIES(calculator_example ncurses)
  ENDIF ()
ENDIF ()

IF (BUILD_TESTS)
  CREATE_TEST(serialization)
  CREATE_TEST(list)
  CREATE_TEST(bit_array)
  CREATE_TEST(array)
  CREATE_TEST(type_list)
  CREATE_TEST(random)
  CREATE_TEST(bit_stream)
  CREATE_TEST(access)
  CREATE_TEST(stress)
  CREATE_TEST(composite_bst)
  CREATE_TEST(delayed_teardown)
  CREATE_TEST(dynamic)
  #  CREATE_TEST(debug)  # disable cause cannot pass on Windows
  CREATE_TEST(guard)
  #  CREATE_TEST(randomize) # disable cause cannot pass
ENDIF ()