# - Find libcmph
# Find the cmph includes and library
#
#  LIBRT_INCLUDE_DIR - Where to find xercesc include sub-directory.
#  LIBRT_LIBRARIES   - List of libraries when using LIBRT.
#  LIBRT_FOUND       - True if LIBRT found.
IF (LIBRT_INCLUDE_DIR)
  # Already in cache, be silent.
  SET(LIBRT_FIND_QUIETLY TRUE)
ENDIF (LIBRT_INCLUDE_DIR)

set(LIBRT_INCLUDE_DIR /usr/include)

SET(LIBRT_NAMES rt )
FIND_LIBRARY(LIBRT_LIBRARY NAMES ${LIBRT_NAMES} )

# Handle the QUIETLY and REQUIRED arguments and set LIBRT_FOUND to
# TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	LIBRT DEFAULT_MSG
	LIBRT_LIBRARY LIBRT_INCLUDE_DIR
)

IF(LIBRT_FOUND)
	SET( LIBRT_LIBRARIES ${LIBRT_LIBRARY} )
ELSE(LIBRT_FOUND)
	SET(LIBRT_LIBRARIES )
ENDIF(LIBRT_FOUND)

MARK_AS_ADVANCED( LIBRT_LIBRARY LIBRT_INCLUDE_DIR )
