include(GenerateExportHeader)

add_library(core INTERFACE)

add_library(order_book::core ALIAS core)

target_link_libraries(core INTERFACE order_book_options order_book_warnings)

target_include_directories(core ${WARNING_GUARD} INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(core INTERFACE cxx_std_20)

set_target_properties(
  core
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(core INTERFACE SAMPLE_LIBRARY_STATIC_DEFINE)
endif()
