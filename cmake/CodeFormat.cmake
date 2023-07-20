# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
CodeFormat
-------

Add code format target.

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

#]=======================================================================]

if(TARGET codeformat)
    return()
endif()

find_package(ClangFormat)

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