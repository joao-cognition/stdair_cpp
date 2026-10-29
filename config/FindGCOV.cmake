# Find the gcov code coverage utility.
#
#  GCOV_EXECUTABLE - Full path to the gcov binary
#  GCOV_VERSION    - The gcov version, e.g., 11.4.0
#  GCOV_FOUND      - Whether gcov has been found

find_program (GCOV_EXECUTABLE NAMES gcov
  DOC "Path to the gcov code coverage utility")

if (GCOV_EXECUTABLE)
  execute_process (COMMAND ${GCOV_EXECUTABLE} --version
    OUTPUT_VARIABLE _gcov_version_output
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  string (REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" GCOV_VERSION
    "${_gcov_version_output}")
endif (GCOV_EXECUTABLE)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (GCOV
  REQUIRED_VARS GCOV_EXECUTABLE
  VERSION_VAR GCOV_VERSION)

mark_as_advanced (GCOV_EXECUTABLE)
