project(GIFLib)

add_library(GIFLib STATIC
  src/dgif_lib.c
  src/egif_lib.c
  src/gifalloc.c
  src/gif_err.c
  src/gif_font.c
  src/gif_hash.c
  src/openbsd-reallocarray.c
  # src/qprintf.c
  # src/quantize.c
  # src/getarg.c
)

target_include_directories(GIFLib PUBLIC include/)
if (NOT MSVC)
  target_compile_options(GIFLib PRIVATE -Wall -Wno-format-truncation)

  if (APPLE)
    if (CMAKE_GENERATOR STREQUAL "Xcode")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../bin/${CMAKE_BUILD_TYPE}")
    endif()
  endif()
endif()

message(STATUS "Building GIFLib library...")