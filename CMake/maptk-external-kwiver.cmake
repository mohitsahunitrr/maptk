# kwiver External Project

message(STATUS "Configuring external KWIVER")

list(APPEND MAPTK_DEPENDENCIES kwiver)

ExternalProject_Add(kwiver
  DEPENDS ${KWIVER_DEPENDENCIES}
  PREFIX ${MAPTK_BINARY_DIR}
  SOURCE_DIR ${MAPTK_EXTERNAL_DIR}/kwiver
  BINARY_DIR ${MAPTK_EXTERNAL_DIR}/kwiver-build
  STAMP_DIR ${MAPTK_STAMP_DIR}
  GIT_REPOSITORY "https://github.com/Kitware/kwiver.git"
  GIT_TAG 73221e7457eed931c1aa0213bfd79cfca09209f9
  #GIT_SHALLOW 1
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=ON
    -Dfletch_DIR:PATH=${fletch_DIR}
    -DKWIVER_ENABLE_ARROWS:BOOL=ON
    -DKWIVER_ENABLE_BURNOUT:BOOL=OFF
    -DKWIVER_ENABLE_CERES:BOOL=ON
    -DKWIVER_ENABLE_CUDA:BOOL=OFF
    -DKWIVER_ENABLE_C_BINDINGS:BOOL=OFF
    -DKWIVER_ENABLE_DARKNET:BOOL=OFF
    -DKWIVER_ENABLE_DLL_WARNINGS:BOOL=OFF
    -DKWIVER_ENABLE_DOCS:BOOL=OFF
    -DKWIVER_ENABLE_EXAMPLES:BOOL=OFF
    -DKWIVER_ENABLE_EXTRAS:BOOL=ON
    -DKWIVER_ENABLE_KPF:BOOL=OFF
    -DKWIVER_ENABLE_LOG4CPLUS:BOOL=ON
    -DKWIVER_ENABLE_LOG4CXX:BOOL=OFF
    -DKWIVER_ENABLE_MATLAB:BOOL=OFF
    -DKWIVER_ENABLE_OPENCV:BOOL=ON
    -DKWIVER_ENABLE_PROCESSES:BOOL=ON
    -DKWIVER_ENABLE_PROJ:BOOL=ON
    -DKWIVER_ENABLE_RightTrack:BOOL=OFF
    -DKWIVER_ENABLE_SPROKIT:BOOL=ON
    -DKWIVER_ENABLE_TESTS:BOOL=${MAPTK_ENABLE_TESTING}
    -DKWIVER_ENABLE_TOOLS:BOOL=ON
    -DKWIVER_ENABLE_TRACK_ORACLE:BOOL=OFF
    -DKWIVER_ENABLE_UUID:BOOL=OFF
    -DKWIVER_ENABLE_VISCL:BOOL=OFF
    -DKWIVER_ENABLE_VXL:BOOL=ON
    -DKWIVER_INSTALL_SET_UP_SCRIPT:BOOL=ON
    -DKWIVER_TEST_ADD_TARGETS:BOOL=OFF
    -DKWIVER_USE_BUILD_TREE:BOOL=OFF
    -DKWIVER_USE_CONFIGURATION_SUBDIRECTORY:BOOL=OFF
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    ${CMAKE_CXX_COMPILER_LAUNCHER_FLAG}
    ${CMAKE_C_COMPILER_LAUNCHER_FLAG}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DMAKECOMMAND:STRING=${MAKECOMMAND}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}
  INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
)

set(KWIVER_DIR "${MAPTK_EXTERNAL_DIR}/kwiver-build")
set(kwiver_DIR "${MAPTK_EXTERNAL_DIR}/kwiver-build" CACHE PATH "Location of KWIVER" FORCE)
