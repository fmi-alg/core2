# - Find libmpfr
# Find the libmpfr includes and library
#
#  LIBMPFR_INCLUDE_DIR - Where to find xercesc include sub-directory.
#  LIBMPFR_LIBRARIES   - List of libraries when using LIBMPFR.
#  LIBMPFR_FOUND       - True if LIBMPFR found.
IF (LIBMPFR_INCLUDE_DIR)
  # Already in cache, be silent.
  SET(LIBMPFR_FIND_QUIETLY TRUE)
ENDIF (LIBMPFR_INCLUDE_DIR)

find_path(LIBMPFR_INCLUDE_DIR
		NAMES mpfr.h
		HINTS ENV MPFR_INC_DIR
				ENV MPFR_DIR
				/usr/include
				/opt/local/include
				/usr/local/include
				/opt/include
		PATH_SUFFIXES include
		DOC "The directory containing the MPFR header files"
		)

# set(LIBMPFR_INCLUDE_DIR /usr/include)

SET(LIBMPFR_NAMES mpfr )
FIND_LIBRARY(LIBMPFR_LIBRARY NAMES ${LIBMPFR_NAMES} )

# Handle the QUIETLY and REQUIRED arguments and set LIBMPFR_FOUND to
# TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	LIBMPFR DEFAULT_MSG
	LIBMPFR_LIBRARY LIBMPFR_INCLUDE_DIR
)

IF(LIBMPFR_FOUND)
	SET(LIBMPFR_LIBRARIES ${LIBMPFR_LIBRARY})
ELSE(LIBMPFR_FOUND)
	SET(LIBMPFR_LIBRARIES )
ENDIF(LIBMPFR_FOUND)

MARK_AS_ADVANCED( LIBMPFR_LIBRARY LIBMPFR_INCLUDE_DIR )
