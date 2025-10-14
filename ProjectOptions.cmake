include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(order_book_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(order_book_setup_options)
  option(order_book_ENABLE_HARDENING "Enable hardening" ON)
  option(order_book_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    order_book_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    order_book_ENABLE_HARDENING
    OFF)

  order_book_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR order_book_PACKAGING_MAINTAINER_MODE)
    option(order_book_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(order_book_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(order_book_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(order_book_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(order_book_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(order_book_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(order_book_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(order_book_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(order_book_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(order_book_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(order_book_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(order_book_ENABLE_PCH "Enable precompiled headers" OFF)
    option(order_book_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(order_book_ENABLE_IPO "Enable IPO/LTO" ON)
    option(order_book_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(order_book_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(order_book_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(order_book_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(order_book_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(order_book_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(order_book_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(order_book_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(order_book_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(order_book_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(order_book_ENABLE_PCH "Enable precompiled headers" OFF)
    option(order_book_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      order_book_ENABLE_IPO
      order_book_WARNINGS_AS_ERRORS
      order_book_ENABLE_USER_LINKER
      order_book_ENABLE_SANITIZER_ADDRESS
      order_book_ENABLE_SANITIZER_LEAK
      order_book_ENABLE_SANITIZER_UNDEFINED
      order_book_ENABLE_SANITIZER_THREAD
      order_book_ENABLE_SANITIZER_MEMORY
      order_book_ENABLE_UNITY_BUILD
      order_book_ENABLE_CLANG_TIDY
      order_book_ENABLE_CPPCHECK
      order_book_ENABLE_COVERAGE
      order_book_ENABLE_PCH
      order_book_ENABLE_CACHE)
  endif()

  order_book_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (order_book_ENABLE_SANITIZER_ADDRESS OR order_book_ENABLE_SANITIZER_THREAD OR order_book_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(order_book_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(order_book_global_options)
  if(order_book_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    order_book_enable_ipo()
  endif()

  order_book_supports_sanitizers()

  if(order_book_ENABLE_HARDENING AND order_book_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR order_book_ENABLE_SANITIZER_UNDEFINED
       OR order_book_ENABLE_SANITIZER_ADDRESS
       OR order_book_ENABLE_SANITIZER_THREAD
       OR order_book_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${order_book_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${order_book_ENABLE_SANITIZER_UNDEFINED}")
    order_book_enable_hardening(order_book_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(order_book_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(order_book_warnings INTERFACE)
  add_library(order_book_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  order_book_set_project_warnings(
    order_book_warnings
    ${order_book_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(order_book_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    order_book_configure_linker(order_book_options)
  endif()

  include(cmake/Sanitizers.cmake)
  order_book_enable_sanitizers(
    order_book_options
    ${order_book_ENABLE_SANITIZER_ADDRESS}
    ${order_book_ENABLE_SANITIZER_LEAK}
    ${order_book_ENABLE_SANITIZER_UNDEFINED}
    ${order_book_ENABLE_SANITIZER_THREAD}
    ${order_book_ENABLE_SANITIZER_MEMORY})

  set_target_properties(order_book_options PROPERTIES UNITY_BUILD ${order_book_ENABLE_UNITY_BUILD})

  if(order_book_ENABLE_PCH)
    target_precompile_headers(
      order_book_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(order_book_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    order_book_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(order_book_ENABLE_CLANG_TIDY)
    order_book_enable_clang_tidy(order_book_options ${order_book_WARNINGS_AS_ERRORS})
  endif()

  if(order_book_ENABLE_CPPCHECK)
    order_book_enable_cppcheck(${order_book_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(order_book_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    order_book_enable_coverage(order_book_options)
  endif()

  if(order_book_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(order_book_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(order_book_ENABLE_HARDENING AND NOT order_book_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR order_book_ENABLE_SANITIZER_UNDEFINED
       OR order_book_ENABLE_SANITIZER_ADDRESS
       OR order_book_ENABLE_SANITIZER_THREAD
       OR order_book_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    order_book_enable_hardening(order_book_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
