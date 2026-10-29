# Find the lcov/genhtml code coverage reporting utilities.
#
#  LCOV_EXECUTABLE    - Full path to the lcov binary
#  GENHTML_EXECUTABLE - Full path to the genhtml binary
#  LCOV_VERSION       - The lcov version, e.g., 1.15
#  LCOV_FOUND         - Whether lcov and genhtml have been found

find_program (LCOV_EXECUTABLE NAMES lcov
  DOC "Path to the lcov code coverage utility")
find_program (GENHTML_EXECUTABLE NAMES genhtml
  DOC "Path to the genhtml code coverage HTML report generator")

if (LCOV_EXECUTABLE)
  execute_process (COMMAND ${LCOV_EXECUTABLE} --version
    OUTPUT_VARIABLE _lcov_version_output
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  string (REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" LCOV_VERSION
    "${_lcov_version_output}")
endif (LCOV_EXECUTABLE)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (LCOV
  REQUIRED_VARS LCOV_EXECUTABLE GENHTML_EXECUTABLE
  VERSION_VAR LCOV_VERSION)

mark_as_advanced (LCOV_EXECUTABLE GENHTML_EXECUTABLE)
