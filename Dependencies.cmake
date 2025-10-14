include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(order_book_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET fmtlib::fmtlib)
    cpmaddpackage("gh:fmtlib/fmt#11.1.4")
  endif()

  if(NOT TARGET spdlog::spdlog)
    cpmaddpackage(
      NAME
      spdlog
      VERSION
      1.15.2
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.8.1")
  endif()

  if(NOT TARGET CLI11::CLI11)
    cpmaddpackage("gh:CLIUtils/CLI11@2.5.0")
  endif()

  if(NOT TARGET ftxui::screen)
    cpmaddpackage("gh:ArthurSonzogni/FTXUI@6.0.2")
  endif()

  if(NOT TARGET tools::tools)
    cpmaddpackage("gh:lefticus/tools#update_build_system")
  endif()

  set(
    BOOST_LIBS
    asio
    align
    date_time
    context
    assert
    algorithm
    array
    config
    core
    bind
    static_assert
    throw_exception
    concept_check
    mp11
    pool
    exception
    preprocessor
    integer
    smart_ptr
    tuple
    type_traits
    predef
    function
    winapi
    system
    io
    iterator
    mpl
    variant2
    detail
    lexical_cast
    fusion
    utility
    container
    range
    optional
    container_hash
    intrusive
    regex
    move
    describe
    conversion
    unordered
    function_types
    typeof
    numeric_conversion
    tokenizer
    functional
  )

  foreach(LIB IN LISTS BOOST_LIBS)
    if (NOT TARGET "boost_${LIB}")
      cpmaddpackage("gh:boostorg/${LIB}#boost-1.89.0")
    endif()
  endforeach()

endfunction()
