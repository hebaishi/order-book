add_executable(asio_sample main.cpp)

target_link_libraries(
  asio_sample
  PRIVATE order_book::order_book_options
          order_book::order_book_warnings)

target_link_system_libraries(
  asio_sample
  PRIVATE
    Boost::asio
          )

target_include_directories(asio_sample PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
