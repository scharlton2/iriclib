##cmake_minimum_required (VERSION 2.8)
cmake_minimum_required(VERSION 3.15)

project (
  iriclib
  VERSION 0.2.8
  LANGUAGES CXX C
)

# set standard directory locations
include(GNUInstallDirs)

# Fortran (Not currently used)
if (NOT IRICLIB_FORTRAN_TESTING)
  if (NOT CMAKE_Fortran_COMPILER OR CMAKE_Fortran_COMPILER STREQUAL "CMAKE_Fortran_COMPILER-NOTFOUND")
    option (IRICLIB_FORTRAN_TESTING "Build Fortran test" OFF)
  else()
    option (IRICLIB_FORTRAN_TESTING "Build Fortran test" ON)
  endif()
else()
  if (IRICLIB_FORTRAN_TESTING)
    enable_language(Fortran)
  endif()  
endif()

# cgns (set CMAKE_PREFIX_PATH)
find_path(CGNS_INC cgns_io.h)
find_library(CGNS_LIBRARY cgnsdll)


if(NOT DEFINED FORTRAN_NAMING)
  # set(FORTRAN_NAMING "UNKNOWN")
  # set(FORTRAN_NAMING "LOWERCASE")
  set(FORTRAN_NAMING "LOWERCASE_")  # gfortran and ifort
  # set(FORTRAN_NAMING "LOWERCASE__")
  # set(FORTRAN_NAMING "UPPERCASE")
  # set(FORTRAN_NAMING "UPPERCASE_")
  # set(FORTRAN_NAMING "UPPERCASE__")
  if(WIN32)
    set(FORTRAN_NAMING "UPPERCASE")
  endif()
endif()

SET(iriclib_SOURCES
  error_macros.h
  filelocker.h
  filelocker.cpp
  fortran_macros.h
  iric_ftoc.c
  iriclib.cpp
  iriclib.h
  iriclib_bstream.cpp
  iriclib_bstream.h
  iriclib_cgnsfile.h
  iriclib_cgnsfile_base.cpp
  iriclib_cgnsfile_bc.cpp
  iriclib_cgnsfile_cc.cpp
  iriclib_cgnsfile_complex_cc.cpp
  iriclib_cgnsfile_geo.cpp
  iriclib_cgnsfile_grid.cpp
  iriclib_cgnsfile_sol.cpp
  iriclib_f.h
  iriclib_geo.cpp
  iriclib_global.h
  iriclib_pointmap.cpp
  iriclib_pointmap.h
  iriclib_polygon.cpp
  iriclib_polygon.h
  iriclib_polyline.cpp
  iriclib_polyline.h
  iriclib_riversurvey.cpp
  iriclib_riversurvey.h
  iriclib_single.c
  private/filelocker_impl.h
  private/iriclib_cgnsfile_baseiterativet.h
  private/iriclib_cgnsfile_baseiterativet_detail.h
  private/iriclib_cgnsfile_impl.h
  private/iriclib_cgnsfile_solutionwriter.cpp
  private/iriclib_cgnsfile_solutionwriter.h
  private/iriclib_cgnsfile_solutionwriterdividesolutions.cpp
  private/iriclib_cgnsfile_solutionwriterdividesolutions.h
  private/iriclib_cgnsfile_solutionwriterstandard.cpp
  private/iriclib_cgnsfile_solutionwriterstandard.h
)

# iriclib target
add_library(iriclib SHARED ${iriclib_SOURCES})

# iriclib compile features
target_compile_features(iriclib
  PRIVATE
    cxx_std_11
)

# iriclib compile definitions
target_compile_definitions(iriclib
  PRIVATE
    _CRT_SECURE_NO_DEPRECATE
    _SCL_SECURE_NO_WARNINGS
    IRICLIBDLL_LIBRARY
    ${FORTRAN_NAMING}
)

# iriclib include directories
target_include_directories(iriclib
  PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/private>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    "${CGNS_INC}"
)

# iriclib link libraries
target_link_libraries(iriclib
  PRIVATE
    ${CGNS_LIBRARY}
)

SET(iriclib_Headers
  ${PROJECT_SOURCE_DIR}/filelocker.h
  ${PROJECT_SOURCE_DIR}/iriclib.h
  ${PROJECT_SOURCE_DIR}/iriclib_f.h
  ${PROJECT_SOURCE_DIR}/iriclib_global.h
  ${PROJECT_SOURCE_DIR}/iriclib_pointmap.h
  ${PROJECT_SOURCE_DIR}/iriclib_polygon.h
  ${PROJECT_SOURCE_DIR}/iriclib_polyline.h
  ${PROJECT_SOURCE_DIR}/iriclib_riversurvey.h
)

SET(iriclib_private_Headers
  ${PROJECT_SOURCE_DIR}/private/filelocker_impl.h
)

include(CMakePackageConfigHelpers)
  
configure_package_config_file(iriclibConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/iriclibConfig.cmake
  INSTALL_DESTINATION cmake/iriclib
)

write_basic_package_version_file("iriclibConfigVersion.cmake"
  VERSION ${iriclib}
  COMPATIBILITY SameMajorVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/iriclibConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/iriclibConfigVersion.cmake
  DESTINATION cmake/iriclib
)

# install
install(TARGETS iriclib
  EXPORT   iriclibTargets
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT iriclibTargets
  FILE iriclibTargets.cmake
  NAMESPACE iriclib::
  DESTINATION cmake/iriclib
)

install(FILES ${iriclib_Headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${iriclib_private_Headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/private)
if(MSVC)
  install(FILES ${PROJECT_BINARY_DIR}/Debug/iriclib.pdb
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    CONFIGURATIONS Debug)
endif()

include(CTest)

if (BUILD_TESTING)
  add_subdirectory(unittests_cgnsfile)
  add_subdirectory(unittest_lock)
  add_subdirectory(unittest_cg_open_modify)
  add_subdirectory(unittest_cg_open_read)
endif()
