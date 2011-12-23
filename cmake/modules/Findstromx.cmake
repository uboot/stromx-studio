# Sets the following variables:
#  STROMX_FOUND       - Set to false, or undefined, if stromx isn't found.
#  STROMX_INCLUDE_DIR - The stromx include directory.
#  STROMX_LIBRARY     - The stromx library to link against.

set(STROMX_POSSIBLE_INCLUDE_DIRS
    ~/include
)

find_path(STROMX_INCLUDE_DIR 
    NAMES stromx/core/Core.h
    PATHS ${STROMX_POSSIBLE_INCLUDE_DIRS}
)

set(STROMX_POSSIBLE_LIBRARY_DIRS
    ~/lib
)

find_library(STROMX_CORE_LIBRARY
    NAMES stromx_core
    PATHS ${STROMX_POSSIBLE_LIBRARY_DIRS}
)

find_library(STROMX_BASE_LIBRARY
    NAMES stromx_base
    PATHS ${STROMX_POSSIBLE_LIBRARY_DIRS}
)

if (STROMX_INCLUDE_DIR AND STROMX_CORE_LIBRARY)
   set(STROMX_FOUND TRUE)
endif (STROMX_INCLUDE_DIR AND STROMX_CORE_LIBRARY)

if (STROMX_FOUND)
   if (NOT stromx_FIND_QUIETLY)
      message(STATUS "Found stromx: ${STROMX_LIBRARY}")
   endif (NOT stromx_FIND_QUIETLY)
else (STROMX_FOUND)
   if (stromx_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find stromx")
   endif (stromx_FIND_REQUIRED)
endif (STROMX_FOUND)
