execute_process(COMMAND brew --prefix OUTPUT_VARIABLE BREW_PREFIX)
string(STRIP ${BREW_PREFIX} BREW_PREFIX)
add_library(readline STATIC IMPORTED)
set_property(TARGET readline APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(readline PROPERTIES IMPORTED_LOCATION_NOCONFIG "${BREW_PREFIX}/opt/readline/lib/libreadline.a")
target_include_directories(readline INTERFACE "${BREW_PREFIX}/opt/readline/include")