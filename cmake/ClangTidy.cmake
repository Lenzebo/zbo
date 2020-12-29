
find_program(CLANG_TIDY_COMMAND clang-tidy)

if (CLANG_TIDY_COMMAND)
    message("-- Found clang-tidy: ${CLANG_TIDY_COMMAND}")
else ()
    message("-- Did not find clang-tidy. Disabling per-target support")
endif()

function(target_enable_clang_tidy targetName)
    if (CLANG_TIDY_COMMAND)
        set_target_properties(${targetName} PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    endif()
endfunction()

