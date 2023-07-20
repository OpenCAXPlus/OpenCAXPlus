if(WIN32)
  execute_process(
    COMMAND cmd /C "echo %DATE:~-4%"
    OUTPUT_VARIABLE YEAR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(
    COMMAND cmd /C "echo %DATE:~3,2%%DATE:~0,2%"
    OUTPUT_VARIABLE MONTHDAY
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(
    COMMAND cmd /C "git rev-parse --short HEAD"
    OUTPUT_VARIABLE HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif(UNIX)
  execute_process(
    COMMAND date "+%Y"
    OUTPUT_VARIABLE YEAR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(
    COMMAND date "+%m%d"
    OUTPUT_VARIABLE MONTHDAY
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
  message(SEND_ERROR "Unable to detect date")
  set(${RESULT} UNKNOWN)
endif()

set(PROJECT_VERSION "${YEAR}.${MONTHDAY}.${HASH}")
message(STATUS "${PROJECT_NAME}'s VERSION is " ${PROJECT_VERSION})