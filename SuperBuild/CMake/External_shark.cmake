INCLUDE_ONCE_MACRO(SHARK)

SETUP_SUPERBUILD(SHARK)

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(SHARK BOOST)

package_require_cxx11(SHARK)

ADD_SUPERBUILD_CMAKE_VAR(SHARK Boost_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(SHARK BOOST_LIBRARYDIR)

ExternalProject_Add(SHARK
  PREFIX SHARK
  URL "https://github.com/Shark-ML/Shark/archive/349f29bd71c370e0f88f7fc9aa66fa5c4768fcb0.zip"
  URL_MD5 d6e4310f943e8dda4a0151612b5c62ce
  SOURCE_DIR ${SHARK_SB_SRC}
  BINARY_DIR ${SHARK_SB_BUILD_DIR}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${SHARK_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DBUILD_DOCS:BOOL=OFF
  -DBUILD_EXAMPLES:BOOL=OFF
  -DBUILD_TESTING:BOOL=OFF
  -DENABLE_HDF5:BOOL=OFF
  -DENABLE_CBLAS:BOOL=OFF
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  )

set(_SB_Shark_DIR ${SB_INSTALL_PREFIX}/lib/cmake/Shark)