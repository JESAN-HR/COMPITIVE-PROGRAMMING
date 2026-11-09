execute_process(
    COMMAND "${PROGRAM}"
    INPUT_FILE "${INPUT_FILE}"
    OUTPUT_VARIABLE actual
    ERROR_VARIABLE error_output
    RESULT_VARIABLE status
    TIMEOUT 10)

if(NOT "${status}" STREQUAL "${EXPECTED_STATUS}")
    message(FATAL_ERROR "${PROGRAM} exited with ${status}, expected ${EXPECTED_STATUS}. Stderr:\n${error_output}")
endif()

file(READ "${EXPECTED_FILE}" expected)
string(REPLACE "\r\n" "\n" actual "${actual}")
string(REPLACE "\r\n" "\n" expected "${expected}")
if(NOT "${actual}" STREQUAL "${expected}")
    message(FATAL_ERROR "Output differed for ${INPUT_FILE}.\nExpected:\n${expected}\nActual:\n${actual}")
endif()
