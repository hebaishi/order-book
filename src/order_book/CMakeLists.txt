add_executable(order-book main.cpp)
target_link_libraries(
  order-book
  PRIVATE order_book::order_book_warnings
          order_book::order_book_options
          order_book::core
          Catch2::Catch2WithMain)
