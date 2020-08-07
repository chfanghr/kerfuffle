IF (ENABLE_COVERAGE)
  FIND_PROGRAM(GCOV_PATH gcov)
  FIND_PROGRAM(GENHTML_PATH genhtml)
  FIND_PROGRAM(LCOV_PATH lcov)

  IF (NOT GCOV_PATH)
    MESSAGE(FATAL_ERROR "gcov not found")
  ENDIF ()

  IF (NOT GENHTML_PATH)
    MESSAGE(FATAL_ERROR "genhtml not found")
  ENDIF ()

  IF (NOT LCOV_PATH)
    MESSAGE(FATAL_ERROR "lcov not found")
  ENDIF ()

  IF (NOT WIN32)
    SET(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    SET(CMAKE_C_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
  ENDIF ()
  # TODO: use per target configuration
ENDIF ()

FUNCTION(COVERAGE_TARGET)
  IF (ENABLE_COVERAGE)
    SET(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    SET(CMAKE_C_FLAGS "-fprofile-instr-generate -fcoverage-mapping")

    # TODO: use per target configuration
    #    #-DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping"
    #    #-DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping"
    #    SET(TARGET_NAME ${ARGV0})
    #    IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #      SET_TARGET_PROPERTIES(${TARGET_NAME} PRIVATE CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    #      SET_TARGET_PROPERTIES(${TARGET_NAME} PRIVATE CMAKE_C_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    #
    #      TARGET_COMPILE_OPTIONS(${TARGET_NAME} BEFORE "-fprofile-instr-generate -fcoverage-mapping")
    #    ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    #      TARGET_COMPILE_OPTIONS(${TARGET_NAME} BEFORE "-fprofile-arcs -ftest-coverage")
    #    ENDIF ()

    MESSAGE(STATUS "Coverage enabled for target ${TARGET_NAME}")
  ENDIF ()
ENDFUNCTION()

