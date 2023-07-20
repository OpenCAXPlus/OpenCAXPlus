# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindClangFormat
-------

Finds the ClangFormat Tool.

Custom Targets
^^^^^^^^^^^^^^^^

This module provides the following custom targets, if found:

``codeformat``
  The ClangFormat custom target to format code according to format file.

``formatcheck``
  The ClangFormat custom target to check code according to format file.

Expected that format file placed at the root of project.

Config Variables
^^^^^^^^^^^^^^^^

``CLANG_FORMAT_CXX_FILE_EXTENSIONS``
  Specify file extension to search for. By default: *.cpp *.h *.cxx *.hxx *.hpp *.cc *.ipp

``CLANG_FORMAT_EXCLUDE_PATTERNS``
  Specify exclude patterns. By default: "/CMakeFiles/" "cmake"

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``ClangFormat_FOUND``
  True if the system has the cppcheck tool.
``ClangFormat_VERSION``
  Version of cppcheck tool.

#]=======================================================================]

if(CLANG_FORMAT_ROOT_DIR)
    find_program(CLANG_FORMAT_BIN
        NAMES clang-format-9 clang-format-8 clang-format-7 clang-format-6.0 clang-format
        PATHS "${CLANG_FORMAT_ROOT_DIR}"
        NO_DEFAULT_PATH)
endif()

if(NOT CLANG_FORMAT_BIN)
    find_program(CLANG_FORMAT_BIN
        NAMES clang-format-9 clang-format-8 clang-format-7 clang-format-6.0 clang-format)
endif()

if(CLANG_FORMAT_BIN)
	execute_process(
            COMMAND ${CLANG_FORMAT_BIN} --version
            OUTPUT_VARIABLE CLANG_FORMAT_VERSION_OUTPUT
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

        if(CLANG_FORMAT_VERSION_OUTPUT MATCHES "^clang-format version .*")
            string(REGEX
                   REPLACE "clang-format version ([.0-9]+).*"
                           "\\1"
                           ClangFormat_VERSION
                           "${CLANG_FORMAT_VERSION_OUTPUT}")
        else()
            message(FATAL "Couldn't recognize ${CLANG_FORMAT_BIN} version")
	endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    ClangFormat
    REQUIRED_VARS CLANG_FORMAT_BIN
    VERSION_VAR ClangFormat_VERSION)

mark_as_advanced(
    CLANG_FORMAT_BIN
    CLANG_FORMAT_VERSION_OUTPUT)

if (CppCheck_FOUND)
    # Find all source files
    set(CLANG_FORMAT_CXX_FILE_EXTENSIONS
            ${CLANG_FORMAT_CXX_FILE_EXTENSIONS}
            *.cpp *.h *.cxx *.hxx *.hpp *.cc *.ipp)
    file(GLOB_RECURSE ALL_SOURCE_FILES ${CLANG_FORMAT_CXX_FILE_EXTENSIONS})

    # Don't include some common build folders
    set(CLANG_FORMAT_EXCLUDE_PATTERNS ${CLANG_FORMAT_EXCLUDE_PATTERNS} "/CMakeFiles/" "cmake")

    # Get all project files file
    foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
        foreach (EXCLUDE_PATTERN ${CLANG_FORMAT_EXCLUDE_PATTERNS})
            string(FIND ${SOURCE_FILE} ${EXCLUDE_PATTERN} EXCLUDE_FOUND)
            if (NOT ${EXCLUDE_FOUND} EQUAL -1)
                list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
            endif ()
        endforeach ()
    endforeach ()

    add_custom_target(codeformat
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running clang-format to change files"
        VERBATIM
        COMMAND ${CLANG_FORMAT_BIN}
        -style=file
        -i
        ${ALL_SOURCE_FILES})

    add_custom_target(formatcheck
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running clang-format to check files"
        VERBATIM
        COMMAND bash -c
        "echo '${ALL_SOURCE_FILES}' | xargs -n 1 -d ';' -I {} bash -c 'diff -u <(cat {}) <(${CLANG_FORMAT_BIN} {})'")
endif()