IF (NOT Win32)
  ADD_CUSTOM_TARGET(snapshot ${CMAKE_SOURCE_DIR}/tools/snapshot.sh)
  ADD_CUSTOM_TARGET(statistics ${CMAKE_SOURCE_DIR}/tools/statistics.sh -v --show-diff)

  CREATE_GEN_VERSION_TARGET(gen_version
                            ${CMAKE_SOURCE_DIR}/src/kerfuffle/version.h.in
                            ${CMAKE_SOURCE_DIR}/src/kerfuffle/version.h)

  ADD_CUSTOM_TARGET(gen_header
                    ${Python3_EXECUTABLE}
                    ${CMAKE_SOURCE_DIR}/tools/merge.py)

  IF (GEN_HEADER)
    ADD_DEPENDENCIES(gen_header gen_version)
  ENDIF ()
ENDIF ()