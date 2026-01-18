function(add_shared_library)
  set(options "")

  set(oneValueArgs NAME ALIAS)

  set(multiValueArgs SOURCES MODULES)
  cmake_parse_arguments(PARSE_ARGV 0 SHARED_LIB
      "${options}" "${oneValueArgs}" "${multiValueArgs}"
  )

  add_library(
    ${SHARED_LIB_NAME}
    SHARED
    ${SHARED_LIB_SOURCES}
  )

  add_library(
    order_book::${SHARED_LIB_NAME}
    ALIAS
    ${SHARED_LIB_NAME}
  )

  target_link_libraries(
    core
    INTERFACE
    order_book_options
    order_book_warnings
  )

  target_link_libraries(
    ${SHARED_LIB_NAME}
    PRIVATE
    ${SHARED_LIB_MODULES}
  )
  generate_export_header(
    ${SHARED_LIB_NAME}
    EXPORT_FILE_NAME public/${SHARED_LIB_NAME}_export.h
  )
  target_compile_definitions(
    ${SHARED_LIB_NAME}
    PRIVATE
    ${SHARED_LIB_NAME}_EXPORTS
  )
  target_include_directories(
    ${SHARED_LIB_NAME}
    ${WARNING_GUARD}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/public>
  )

  target_compile_features(
    ${SHARED_LIB_NAME}
    INTERFACE
    cxx_std_20
  )

  set_target_properties(
    ${SHARED_LIB_NAME}
    PROPERTIES VERSION ${PROJECT_VERSION}
               CXX_VISIBILITY_PRESET hidden
               VISIBILITY_INLINES_HIDDEN YES)

  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${SHARED_LIB_NAME} INTERFACE SAMPLE_LIBRARY_STATIC_DEFINE)
  endif()
endfunction()
