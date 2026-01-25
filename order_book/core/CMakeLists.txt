add_shared_library(
  NAME core
  SOURCES
    src/core/book.cpp
    src/core/order.cpp
    src/core/match.cpp
    src/core/state/process.cpp
    src/core/message/parse.cpp
    src/core/message/serialize.cpp
    src/core/message/token_parser.cpp
  MODULES
    boost_json
    boost_serialization
)

if(BUILD_TESTING)
  message(AUTHOR_WARNING "Building Tests. Be sure to check out test/constexpr_tests.cpp for constexpr testing")
  add_subdirectory(test)
endif()
