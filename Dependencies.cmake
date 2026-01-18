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
    algorithm
    iostreams
    random
    dynamic_bitset
    align
    array
    asio
    assert
    atomic
    bind
    chrono
    concept_check
    config
    container
    container
    container_hash
    container_hash
    context
    conversion
    core
    date_time
    describe
    detail
    endian
    exception
    function
    function_types
    functional
    fusion
    integer
    intrusive
    io
    iterator
    json
    lexical_cast
    move
    mp11
    mpl
    numeric_conversion
    optional
    phoenix
    pool
    predef
    preprocessor
    proto
    range
    ratio
    regex
    serialization
    smart_ptr
    spirit
    static_assert
    system
    thread
    throw_exception
    tokenizer
    tuple
    type_index
    type_traits
    typeof
    unordered
    utility
    variant
    variant2
    winapi
  )

  foreach(LIB IN LISTS BOOST_LIBS)
    if (NOT TARGET "boost_${LIB}")
      cpmaddpackage("gh:boostorg/${LIB}#boost-1.89.0")
    endif()
  endforeach()

endfunction()
