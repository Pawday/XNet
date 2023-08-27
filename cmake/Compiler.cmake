function(_cm_cxx_compiler_is_clang OUTPUT)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        set(${OUTPUT} TRUE PARENT_SCOPE)
        return()
    endif()

    set(${OUTPUT} FALSE PARENT_SCOPE)
endfunction()

function(_cm_cxx_compiler_is_clang_gcc OUTPUT)
    _cm_cxx_compiler_is_clang(IS_CLANG)
    if(NOT ${IS_CLANG})
        message(FATAL_ERROR "Cannot check clang's frontend variant is GCC. You are using ${CMAKE_CXX_COMPILER_ID} (NOT Clang)")
    endif()

    if(${CMAKE_CXX_COMPILER_FRONTEND_VARIANT} STREQUAL "GNU")
        set(${OUTPUT} TRUE PARENT_SCOPE)
        return()
    endif()

    set(${OUTPUT} FALSE PARENT_SCOPE)

endfunction()

function(_cm_cxx_compiler_is_clang_msvc OUTPUT)
    _cm_cxx_compiler_is_clang(IS_CLANG)
    if(NOT ${IS_CLANG})
        message(FATAL_ERROR "Cannot check clang's frontend variant is MSVC. You are using ${CMAKE_CXX_COMPILER_ID} (NOT Clang)")
    endif()

    if(${CMAKE_CXX_COMPILER_FRONTEND_VARIANT} STREQUAL "MSVC")
        set(${OUTPUT} TRUE PARENT_SCOPE)
        return()
    endif()

    set(${OUTPUT} FALSE PARENT_SCOPE)

endfunction()


function(_cm_compiler_clang_enable_strict_compilation TARGET)
    _cm_cxx_compiler_is_clang(IS_CLANG)
    if(NOT ${IS_CLANG})
        message(FATAL_ERROR "Cannot enable strict compilation for clang\" You are using ${CMAKE_CXX_COMPILER_ID}")
    endif()

    _cm_cxx_compiler_is_clang_gcc(IS_CLANG_GCC)
    if(${IS_CLANG_GCC})
        target_compile_options(${TARGET} PRIVATE "-Wall"
            "-Wextra"
            "-Werror"
            "-pedantic")
        return()
    endif()

    _cm_cxx_compiler_is_clang_msvc(IS_CLANG_MSVC)
    if(${IS_CLANG_MSVC})
        target_compile_options(${TARGET} PRIVATE "/Wall" "/WX")
        return()
    endif()

    message(FATAL_ERROR "Unhandled clang's ${CMAKE_C_COMPILER_FRONTEND_VARIANT} frontend")

endfunction()

function(cm_compiler_is_known OUTPUT)

    _cm_cxx_compiler_is_clang(IS_CLANG)
    if (${IS_CLANG})
        _cm_cxx_compiler_is_clang_gcc(IS_CLANG_GCC)
        if(${IS_CLANG_GCC})
            set(${OUTPUT} TRUE PARENT_SCOPE)
            return()
        endif()

        _cm_cxx_compiler_is_clang_msvc(IS_CLANG_MSVC)
        if(${IS_CLANG_MSVC})
            set(${OUTPUT} TRUE PARENT_SCOPE)
            return()
        endif()
    endif()


    set(${OUTPUT} FALSE PARENT_SCOPE)

endfunction()


function(cm_compiler_try_enable_strict_compilation TARGET)
    cm_compiler_is_known(COMPILER_IS_KNOWN)
    if(NOT ${COMPILER_IS_KNOWN})
        message(WARNING "Function cm_compiler_try_enable_strict_compilation does nothing: Compiler unknown")
        return()
    endif()

    _cm_cxx_compiler_is_clang(IS_CLANG)
    if(${IS_CLANG})
        _cm_compiler_clang_enable_strict_compilation(${TARGET})
        return()
    endif()

    message(FATAL_ERROR "Unhandled known compiler")

endfunction()
