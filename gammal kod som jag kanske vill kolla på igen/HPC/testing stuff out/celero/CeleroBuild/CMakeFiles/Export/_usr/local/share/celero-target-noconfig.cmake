#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "celero" for configuration ""
set_property(TARGET celero APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(celero PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/usr/local/lib/libcelero.so"
  IMPORTED_SONAME_NOCONFIG "libcelero.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS celero )
list(APPEND _IMPORT_CHECK_FILES_FOR_celero "/usr/local/lib/libcelero.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
